#include <linux/device.h>	/* device class */
#include <linux/fs.h>		/* unregister_chrdev_region */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/* printk */
#include <linux/slab.h>		/* kfree */

#include "scull.h"
#include "file_ops.h"

#define CLASS_NAME "scull"

MODULE_AUTHOR("Amaury Denoyelle <amaury.denoyelle@gmail.com>");
MODULE_LICENSE("GPL");

struct scull_dev * scull_devices;
int nbr_devices = 4;
struct class * scull_class;

int quantum = 4000;
int qset = 1000;

/* device file number */
dev_t dev;

static __exit void cleanup_code(void)
{
	int i, devno;

	/* Clean up code */
	printk(KERN_DEBUG "scull cleaning\n");

	for(i = 0; i < nbr_devices; ++i) {
		devno = MKDEV(MAJOR(dev), MINOR(dev) + i);
		if(scull_class)
			device_destroy(scull_class, devno);

		if(&scull_devices[i].cdev)
			cdev_del(&scull_devices[i].cdev);
	}

	if(scull_class)
		class_destroy(scull_class);

	unregister_chrdev_region(dev, nbr_devices);

	if(scull_devices)
		kfree(scull_devices);
}

static __init int initialization_code(void)
{
	int retval = 0, i;
	int devno;

	/* Initialization code */
	printk(KERN_DEBUG "scull init\n");

	/* Allocate scull devices structures */
	if((scull_devices = kmalloc(nbr_devices * sizeof(struct scull_dev),
			GFP_KERNEL)) == NULL) {
		printk(KERN_ERR "Cannot allocate scull devices\n");
		retval = -ENOMEM;
		goto kmalloc_fail;
	}

	/* Allocate character devices */
	if((retval = alloc_chrdev_region(&dev, 0, nbr_devices, CLASS_NAME))) {
		goto chrdev_fail;
	}

	/* Create a class device */
	scull_class = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(scull_class)) {
		retval = PTR_ERR(scull_class);
		goto class_create_fail;
	}

	memset(scull_devices, 0, nbr_devices * sizeof(struct scull_dev));

	for(i = 0; i < nbr_devices; ++i) {
		scull_devices[i].data = NULL;
		scull_devices[i].quantum = quantum;
		scull_devices[i].qset = qset;
		scull_devices[i].size = 0;

		/* cdev structure */
		cdev_init(&scull_devices[i].cdev, &fops);

		scull_devices[i].cdev.ops = &fops;
		scull_devices[i].cdev.owner = THIS_MODULE;

		devno = MKDEV(MAJOR(dev), MINOR(dev) + i);

		/* add cdev structure, be prepared to handle it !! */
		if(cdev_add(&scull_devices[i].cdev, devno, 1) < 0)
			printk(KERN_ERR "Failed to add scull dev nbr %i\n", i);

		/* Create entry point in /dev */
		scull_devices[i].dev = device_create(scull_class, NULL, devno,
				NULL, CLASS_NAME "%i", i);
		if(IS_ERR(scull_devices[i].dev)) {
			scull_devices[i].dev = NULL;
			printk(KERN_ERR "Cannot create dev file nbr %i\n", i);
		}

	}

	return 0;

class_create_fail:
	unregister_chrdev_region(dev, nbr_devices);
chrdev_fail:
	kfree(scull_devices);
kmalloc_fail:
	return retval;
}

module_init(initialization_code);
module_exit(cleanup_code);
