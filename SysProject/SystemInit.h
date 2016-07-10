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

#define TIMESAMPLE 30000 //5,000����С��usleepʱ�䣬Ҳ��������ͨѶ���ʣ��ĺ���λ��CAN2�ϴ����������ʣ�399֡ÿ�룬PC��CAN�����������ӳ�15�룬
//��usleep ʱ��Ϊ20��000ʱ������С�����ӳ�ͨѶ����ʱ�䡣


/*can_init()*/


/*task_init()*/
pid_t motion_curb_pid;
pid_t AD_calibrate_pid;

//�߳�
pthread_t  CAL_AD_thread,CAN_WR_thread, CAN_RD_thread; // Print_thread;
pthread_t GAIT_DATA_thread,FD_MANA_thread;

//�߳�����������
pthread_mutex_t mutex_cond_send;

pthread_cond_t cond_send;
int send_condition;


//ͬ���ź���


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
