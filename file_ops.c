#include "file_ops.h"

#include <linux/fs.h>
#include <linux/module.h>

/* File operations structure */
struct file_operations fops = {
	.owner	= THIS_MODULE,
	.open	= scull_open,
};

int scull_open(struct inode * inode, struct file * file)
{
	return 0;
}
