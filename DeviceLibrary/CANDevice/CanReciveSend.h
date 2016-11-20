/*
 * CanReciveSend.h
 *
 *  Created on: 2016-4-7
 *      Author: Administrator
 */

#ifndef CANRECIVESEND_H_
#define CANRECIVESEND_H_
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#define CAN0 0
#define CAN1 1


#include "DeviceLibrary/CANDevice/advcan.h"
#include "DeviceLibrary/CANDevice/CanConfigure.h"

#define WR_PORT "/dev/advcan0"
#define RD_PORT "/dev/advcan1"

#define QY 0x20
#define QZ 0x21
#define HY 0x22
#define HZ 0x23

#define Joint1 0x10
#define Joint2 0x11
#define Joint3 0x12


typedef struct {
	int wrfd;
	fd_set wrfds;
	int buadrate;
	int newcode;
	int newmask;
	unsigned int data1[12];
	unsigned int data2[12];
	unsigned int data3[12];
	int fflag[13];//表示传感器的状态，1是上极限，-1 是下极限；0表示中位
}CanFdSet_t;


int can_recive(CanFdSet_t *FdSet);
int can_send(CanFdSet_t *FdSet);
int can_init(int canport,CanFdSet_t *FdSet);
int can_end(CanFdSet_t *FdSet);
#endif /* CANRECIVESEND_H_ */
