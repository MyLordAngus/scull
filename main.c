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

int quantum = 4000;
int qset = 1000;

/* device file number */
dev_t dev;

static __exit void cleanup_code(void)
{
	int i;

	/* Clean up code */
	printk(KERN_DEBUG "scull cleaning\n");

	for(i = 0; i < nbr_devices; ++i)
		cdev_del(&scull_devices[i].cdev);

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

	memset(scull_devices, 0, nbr_devices * sizeof(struct scull_dev));

	/* Allocate character devices */
	if((retval = alloc_chrdev_region(&dev, 0, nbr_devices, CLASS_NAME))) {
		goto chrdev_fail;
	}

	for(i = 0; i < nbr_devices; ++i) {
		scull_devices[i].data = NULL;
		scull_devices[i].quantum = quantum;
		scull_devices[i].qset = qset;
		scull_devices[i].size = 0;

		/* cdev structure */
		cdev_init(&scull_devices[i].cdev, &fops);

		scull_devices[i].cdev.ops = &fops;
		scull_devices[i].cdev.owner = THIS_MODULE;

		printk(KERN_DEBUG "Init cdev struct, prepare to add it\n");
		devno = MKDEV(MAJOR(dev), MINOR(dev) + i);

		/* add cdev structure, be prepared to handle it !! */
		if(cdev_add(&scull_devices[i].cdev, devno, 1) < 0)
			printk(KERN_ERR "Failed to add scull dev nbr %i\n", i);
	}

	return 0;

chrdev_fail:
	kfree(scull_devices);
kmalloc_fail:
	return retval;
}

module_init(initialization_code);
module_exit(cleanup_code);
