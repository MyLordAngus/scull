#ifndef FILE_OPS_H_
#define FILE_OPS_H_

#include <linux/fs.h>

struct scull_dev;

/* Method for file_ops structure */
int scull_open(struct inode *, struct file *);
int scull_release(struct inode * inode, struct file * file);
ssize_t scull_read(struct file *, char __user *, size_t, loff_t *);
ssize_t scull_write(struct file *, const char __user *, size_t, loff_t *);

struct scull_qset * scull_find_qset(struct scull_dev *, int);

/* File operations structure */
extern struct file_operations fops;

#endif
