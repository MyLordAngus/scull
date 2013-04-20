#ifndef SCULL_INIT_MODULE_H_
#define SCULL_INIT_MODULE_H_

#include <linux/types.h>	/* dev_t */

/* Allocate scull devices and character module structure */
int scull_init_module(dev_t *);

#endif
