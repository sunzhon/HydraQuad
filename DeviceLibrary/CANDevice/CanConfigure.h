/*
 * CanConfigure.h
 *
 *  Created on: 2016-4-8
 *      Author: Administrator
 */

#ifndef CANCONFIGURE_H_
#define CANCONFIGURE_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <devctl.h>
#include <fcntl.h>
#include "DeviceLibrary/CANDevice/advcan.h"

int set_accmask(int fd, /* device descriptor */
int newcode, int newmask);
int set_acccode(int fd, /* device descriptor */
int newcode);
int set_bitrate(int can_fd, /* device descriptor */int baud /* bit rate */);

#endif /* CANCONFIGURE_H_ */
