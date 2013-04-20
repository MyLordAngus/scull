#include <linux/fs.h>		/* unregister_chrdev_region */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/* printk */
#include <linux/slab.h>		/* kfree */

#include "scull.h"
#include "scull_init.h"

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
	int retval = 0;

	/* Initialization code */
	printk(KERN_DEBUG "scull init\n");
	if((retval = scull_init_module(&dev)) != 0)
		goto fail;

	return 0;

fail:
	cleanup_code();
	return retval;
}

module_init(initialization_code);
module_exit(cleanup_code);
