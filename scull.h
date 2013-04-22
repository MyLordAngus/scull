#ifndef SCULL_H_
#define SCULL_H_

#include <linux/cdev.h>
#include <linux/device.h>

#define	SCULL_QSET	1000
#define	SCULL_QUANTUM	4000

struct scull_dev {
	struct scull_qset * data;	/* pointer to first qset */
	int quantum;			/* the quantum size */
	int qset;			/* the qset (array pointer) size */
	unsigned long size;	/* actual size of data stored in device */
	struct cdev cdev;		/* character device struct */
	struct device * dev;		/* dev structure from device_create */
};

struct scull_qset {
	void ** data;
	struct scull_qset * next;
};
#endif
