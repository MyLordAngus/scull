#include <linux/fs.h>		/* unregister_chrdev_region */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/* printk */
#include <linux/slab.h>		/* kfree */

#include "scull.h"

MODULE_AUTHOR("Amaury Denoyelle <amaury.denoyelle@gmail.com>");
MODULE_LICENSE("GPL");

struct scull_dev * scull_devices;
int nbr_devices = 4;

int quantum = 4000;
int qset = 1000;

/* device file */
dev_t dev;

static void cleanup_code(void)
{
	/* Clean up code */
	printk(KERN_DEBUG "scull cleaning\n");

	if(scull_devices)
		kfree(scull_devices);

	unregister_chrdev_region(dev, nbr_devices);
}

static int initialization_code(void)
{
	int retval = 0, i;

	/* Initialization code */
	printk(KERN_DEBUG "scull init\n");

	/* Allocate scull devices structures */
	if((scull_devices = kmalloc(nbr_devices * sizeof(struct scull_dev),
			GFP_KERNEL)) == NULL) {
		printk(KERN_ERR "Cannot allocate scull devices\n");
		retval = -ENOMEM;
		goto fail;
	}

	memset(scull_devices, 0, nbr_devices * sizeof(struct scull_dev));

	for(i = 0; i < nbr_devices; ++i) {
		scull_devices[i].data = NULL;
		scull_devices[i].quantum = quantum;
		scull_devices[i].qset = qset;
		scull_devices[i].size = 0;

		/* Allocate character devices */
		if((retval = alloc_chrdev_region(&dev, 0, nbr_devices, "scull"))) {
			goto fail;
		}
	}

	return 0;

fail:
	cleanup_code();
	return retval;
}

module_init(initialization_code);
module_exit(cleanup_code);
