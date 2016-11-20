/*
 * ForceAcq.h
 *
 *  Created on: 2016-11-10
 *      Author: SunTao
 */

#ifndef FORCEACQ_H_
#define FORCEACQ_H_



#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include "Algorithm/JointAlgo/JointControler.h"
#include "DataProcess/JointData/GaitData.h"
#include "DeviceLibrary/CANDevice/advcan.h"
#include "DeviceLibrary/CANDevice/CanConfigure.h"
#include "DeviceLibrary/CANDevice/CanReciveSend.h"
#include "DeviceLibrary/FileDevice/JointDataFile.h"
#include "DataProcess/ADCalibrate/ad_motion.h"


#define FORCEPATH "/home/XtaoCan/LPCData/force.txt"

#define NewCode 0x20
#define NewMask 0x7fffff

#define Force_para 0x50

//管道
extern  char forcepath[];
extern  char force_buffer[128];

extern pid_t force_acquisition_pid;


/*can_init()*/



//线程
pthread_t FORCE_GET_thread;
pthread_t FORCE_RE_thread;
pthread_t FORCE_SD_thread; // Print_thread;
pthread_t FORCE_FD_thread;


//线程锁和条件锁
pthread_mutex_t mutex_cond_send;

pthread_cond_t cond_send;


//同步信号量


typedef struct{
	sem_t sem_X;
	sem_t sem_Y;
	sem_t sem_Z;
	FILE *fp;
	int status;
	CanFdSet_t FdSet;
}ForAcq;
int force_resource_init(ForAcq *arg);
int force_task_init(ForAcq *arg);
int force_resource_end(ForAcq *arg);
int force_task_end(ForAcq *arg);


void * force_can_send_pthread(void *arg);
void * force_can_recive_pthread(void *arg);
void * force_get_data_pthread(void *arg);
void * force_feddata_manage_pthread(void *arg);
void sig_handler2(int signum);







#endif /* FORCEACQ_H_ */
