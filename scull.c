#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");

static int __init initialization_code(void)
{
	/* Initialization code */
	printk(KERN_DEBUG "scull init\n");

	return 0;
}
module_init(initialization_code);

static void __exit cleanup_code(void)
{
	/* Clean up code */
	printk(KERN_DEBUG "scull cleaning\n");
}
module_exit(cleanup_code);
