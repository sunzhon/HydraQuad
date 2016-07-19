/*
 * SystemInit.h
 *
 *  Created on: 2016-4-8
 *      Author: Administrator
 */
/* this file define initial data struct */
#ifndef SYSTEMINIT_H_
#define SYSTEMINIT_H_

#include <pthread.h>
#include <semaphore.h>
#include "Algorithm/JointAlgo/JointControler.h"
#include "DataProcess/JointData/GaitData.h"
#include "DeviceLibrary/CANDevice/advcan.h"
#include "DeviceLibrary/CANDevice/CanConfigure.h"
#include "DeviceLibrary/CANDevice/CanReciveSend.h"
#include "DeviceLibrary/FileDevice/JointDataFile.h"
#include "DataProcess/ADCalibrate/ad_motion.h"



#define NewCode 0x20
#define NewMask 0x7fffff

#define MAX 100

#define TIMESAMPLE 30000 //5,000是最小的usleep时间，也就是最大的通讯速率，四核下位机CAN2上传的数据速率：399帧每秒，PC机CAN调试器接受延迟15秒，
//当usleep 时间为20，000时，是最小的无延迟通讯休眠时间。


/*can_init()*/


/*task_init()*/
pid_t motion_curb_pid;
pid_t AD_calibrate_pid;

//线程
pthread_t  CAL_AD_thread,CAN_WR_thread, CAN_RD_thread; // Print_thread;
pthread_t GAIT_DATA_thread,FD_MANA_thread;

//线程锁和条件锁
pthread_mutex_t mutex_cond_send;

pthread_cond_t cond_send;
int send_condition;


//同步信号量


float hopedata[1200];
FILE *fp[4];

typedef struct{
	sem_t sem_A;
	sem_t sem_B;
	sem_t sem_C;
	sem_t sem_D;
	int status;
	CanFdSet_t FdSet;
}ARG;
int resource_init(ARG *arg);
int task_init(ARG *arg);
int resource_end(ARG *arg);
int task_end(ARG *arg);


void * can_send_pthread(void *arg);
void * can_recive_pthread(void *arg);
void * gait_data_pthread(void *arg);
void *feddata_manage_pthread(void *arg);
void *calibrate_ad_pthread(ARG *arg);



#endif /* SYSTEMINIT_H_ */
