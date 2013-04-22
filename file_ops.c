#include "file_ops.h"

#include <linux/fs.h>
#include <linux/kernel.h>	/* macro container_of */
#include <linux/module.h>
#include <linux/slab.h>		/* kmalloc */
#include <linux/uaccess.h>	/* copy_from/to_user */

#include "scull.h"

/* File operations structure */
struct file_operations fops = {
	.owner		= THIS_MODULE,
	.open		= scull_open,
	.release	= scull_release,
	.read		= scull_read,
	.write		= scull_write,
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

ssize_t scull_write(struct file * file, const char __user * buf, size_t count,
		loff_t * off)
{
	int index;	/* The index where the write should begin */
	int rest = 0;	/* The count of item too big for the current quantum */
	int qset_index, quantum_index;

	struct scull_dev * scull_dev;
	struct scull_qset * scull_qset;

	printk(KERN_DEBUG "Writing on scull device\n");
	scull_dev = file->private_data;
	scull_qset = scull_dev->data;

	index = file->f_pos + *off;
	qset_index = index / (scull_dev->quantum * scull_dev->qset);
	quantum_index = index % scull_dev->quantum;

	if(quantum_index + count > scull_dev->quantum) {
		rest = quantum_index + count - scull_dev->quantum;
		count -= rest;
	}

	if(!(scull_qset = scull_find_qset(scull_dev, qset_index)))
		return -ENOMEM;

	/* Allocate the pointer array if needed */
	if(!scull_qset->data) {
		printk(KERN_DEBUG "Allocate qset array pointer\n");

		if((scull_qset->data = kmalloc(scull_dev->qset * sizeof(void *),
						GFP_KERNEL)) == NULL) {
			printk(KERN_ERR "Cannot allocate pointer array\n");
			return -ENOMEM;
		}
		memset(scull_qset->data, 0, scull_dev->qset * sizeof(void *));
	}

	/* Allocate the quantum if needed */
	if(!scull_qset->data[quantum_index]) {
		printk(KERN_DEBUG "Allocate quantum\n");

		if((scull_qset->data[quantum_index] =
				kmalloc(scull_dev->quantum * sizeof(char),
						GFP_KERNEL)) == NULL) {
			printk(KERN_ERR "Cannot allocate quantum\n");
			return -ENOMEM;
		}
		memset(scull_qset->data[quantum_index], 0,
				scull_dev->quantum * sizeof(char));
	}

	printk(KERN_DEBUG "Prepare to write from user\n");
	if(copy_from_user(scull_qset->data[quantum_index], buf, count))
		return -EFAULT;

	printk(KERN_DEBUG "Writing is complete :)\n");

	return count;
}

int scull_release(struct inode * inode, struct file * file)
{
	printk(KERN_DEBUG "Close scull device\n");

	/* Everything's all right!! */
	/* Do not forget to free if you have allocated things in
	 * file->private_data */
	return 0;
}

/** Find the corresponding qset struct given of an index,
 * allocate if needed */
struct scull_qset * scull_find_qset(struct scull_dev * scull_dev,
		int qset_index)
{
	struct scull_qset * new_qset;
	struct scull_qset * scull_qset = scull_dev->data;

	printk(KERN_DEBUG "Searching the qset\n");

	/* If first qset is NULL */
	if(!scull_qset) {
		printk(KERN_DEBUG "Allocating a new qset nbr 0\n");

		if((scull_qset = kmalloc(sizeof(struct scull_qset),
				GFP_KERNEL)) == NULL) {
			printk(KERN_ERR "Cannot allocate qset!!\n");
			return NULL;
		}
		memset(scull_qset, 0, sizeof(struct scull_qset));
		scull_dev->data = scull_qset;
	}

	while(qset_index--) {
		printk(KERN_DEBUG "Allocating a new qset nbr %i\n", qset_index);

		if((new_qset = kmalloc(sizeof(struct scull_qset),
				GFP_KERNEL)) == NULL) {
			printk(KERN_ERR "Cannot allocate qset!!\n");
			return NULL;
		}
		memset(new_qset, 0, sizeof(struct scull_qset));

		scull_qset->next = new_qset;
		scull_qset = new_qset;
	}

	printk(KERN_DEBUG "Found a qset, returning it\n");
	return scull_qset;
}
