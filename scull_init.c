#include <linux/fs.h>		/* alloc_chrdev_region */
#include <linux/kernel.h>	/* printk */
#include <linux/slab.h>		/* kmalloc */

#include "scull.h"

extern struct scull_dev * scull_devices;
extern int nbr_devices;

extern int quantum;
extern int qset;

int scull_init_module(dev_t * dev)
{
	int retval = 0, i;

	/* Allocate scull devices structures */
	if((scull_devices = kmalloc(nbr_devices * sizeof(struct scull_dev),
			GFP_KERNEL)) == NULL) {
		printk(KERN_ERR "Cannot allocate scull devices\n");
		retval = -ENOMEM;
		goto ret;
	}

	memset(scull_devices, 0, nbr_devices * sizeof(struct scull_dev));

	for(i = 0; i < nbr_devices; ++i) {
		scull_devices[i].data = NULL;
		scull_devices[i].quantum = quantum;
		scull_devices[i].qset = qset;
		scull_devices[i].size = 0;

		/* Allocate character devices */
		if((retval = alloc_chrdev_region(dev, 0, nbr_devices, "scull"))) {
			goto ret;
		}
	}

ret:
	return retval;
}
