#ifndef FILE_OPS_H_
#define FILE_OPS_H_

#include <linux/fs.h>

/* Method for file_ops structure */
int scull_open(struct inode *, struct file *);

/* File operations structure */
extern struct file_operations fops;

#endif
