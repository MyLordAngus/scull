#ifndef FILE_OPS_H_
#define FILE_OPS_H_

#include <linux/fs.h>

/* Method for file_ops structure */
int scull_open(struct inode *, struct file *);
ssize_t scull_read(struct file *, char __user *, size_t, loff_t *);
int scull_release(struct inode * inode, struct file * file);

/* File operations structure */
extern struct file_operations fops;

#endif
