#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>	/* printk */

#include "scull.h"
#include "scull_init.h"

MODULE_AUTHOR("Amaury Denoyelle <amaury.denoyelle@gmail.com>");
MODULE_LICENSE("GPL");

struct scull_dev * scull_devices;

static int __init initialization_code(void)
{
	/* Initialization code */
	printk(KERN_DEBUG "scull init\n");
	scull_init_module();

	return 0;
}
module_init(initialization_code);

static void __exit cleanup_code(void)
{
	/* Clean up code */
	printk(KERN_DEBUG "scull cleaning\n");
}
module_exit(cleanup_code);
