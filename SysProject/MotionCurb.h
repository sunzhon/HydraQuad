/*
 * MotionCurb.h
 *
 *  Created on: 2016-4-8
 *      Author: SUnTao
 */
/* this file define initial data struct */
#ifndef MOTIONCURB_H_
#define MOTIONCURB_H_

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



#define NewCode 0x20
#define NewMask 0x7fffff

//#define MAX 100
/* usleep ualarm udelay 此类都是线程挂起函数，没有u开头的是进程挂起函数*/

//#define TIMESAMPLE 3000 //5,000是最小的usleep时间，也就是最大的通讯速率，四核下位机CAN2上传的数据速率：399帧每秒，PC机CAN调试器接受延迟15秒，
//当usleep 时间为20，000时，是最小的无延迟通讯休眠时间,CAN调试器测试的单总线通讯速率为76帧每秒，当usleep时间为30,000时CAN速率61帧每秒。
//
//160，000——43帧每秒
//PC104和LPC2129 伺服板的了LED闪动速率和CAN通讯速率正比
// 注意cleanproject 然后 build　project
#define NUM_COLUMNS 12
#define NUM_ROWS 100
#define NUM_LEGS 4
#define NUM_JOINTS 12
#define NUM_FILES 4
#define NUM_DATAS (NUM_ROWS*NUM_COLUMNS)


//有名管道FIFO

extern char jointpath[];
extern char joint_buffer[128];

/*can_init()*/



//线程
pthread_t  CAL_AD_thread,CAN_WR_thread, CAN_RD_thread; // Print_thread;
pthread_t GAIT_DATA_thread,FD_MANA_thread;

//线程锁和条件锁
pthread_mutex_t mutex_cond_send;

pthread_cond_t cond_send;

extern pid_t motion_curb_pid;


//同步信号量


extern float hopedata[NUM_COLUMNS*NUM_ROWS];
extern FILE *fp[NUM_FILES];

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
void sig_handler1(int signum);



#endif /* SYSTEMINIT_H_ */
