#ifndef SCULL_H_
#define SCULL_H_

#include <linux/cdev.h>

struct scull_dev {
	struct scull_qset * data;	/* pointer to first qset */
	int quantum;			/* the quantum size */
	int qset;			/* the qset (array pointer) size */
	unsigned long size;	/* actual size of data stored in device */
	struct cdev cdev;		/* character device struct */
};

struct scull_qset {
	void ** data;
	struct scull_qset * next;
};
#endif
