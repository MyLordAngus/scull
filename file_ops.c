#include "file_ops.h"

#include <linux/fs.h>
#include <linux/kernel.h>	/* macro container_of */
#include <linux/module.h>

#include "scull.h"

/* File operations structure */
struct file_operations fops = {
	.owner		= THIS_MODULE,
	.open		= scull_open,
	.release	= scull_release,
	.read		= scull_read,
};

int scull_open(struct inode * inode, struct file * file)
{
	struct scull_dev * scull_dev;

	printk(KERN_DEBUG "Open scull device\n");

	/* Get scull_dev and save it in private data for other methods */
	scull_dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	file->private_data = scull_dev;

	return 0;
}

ssize_t scull_read(struct file * file, char __user * buf, size_t count,
		loff_t * off)
{
	return 0;
}

int scull_release(struct inode * inode, struct file * file)
{
	printk(KERN_DEBUG "Close scull device\n");

	/* Everything's all right!! */
	/* Do not forget to free if you have allocated things in
	 * file->private_data */
	return 0;
}
