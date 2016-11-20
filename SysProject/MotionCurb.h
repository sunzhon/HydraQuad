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
/* usleep ualarm udelay ���඼���̹߳�������û��u��ͷ���ǽ��̹�����*/

//#define TIMESAMPLE 3000 //5,000����С��usleepʱ�䣬Ҳ��������ͨѶ���ʣ��ĺ���λ��CAN2�ϴ����������ʣ�399֡ÿ�룬PC��CAN�����������ӳ�15�룬
//��usleep ʱ��Ϊ20��000ʱ������С�����ӳ�ͨѶ����ʱ��,CAN���������Եĵ�����ͨѶ����Ϊ76֡ÿ�룬��usleepʱ��Ϊ30,000ʱCAN����61֡ÿ�롣
//
//160��000����43֡ÿ��
//PC104��LPC2129 �ŷ������LED�������ʺ�CANͨѶ��������
// ע��cleanproject Ȼ�� build��project
#define NUM_COLUMNS 12
#define NUM_ROWS 100
#define NUM_LEGS 4
#define NUM_JOINTS 12
#define NUM_FILES 4
#define NUM_DATAS (NUM_ROWS*NUM_COLUMNS)


//�����ܵ�FIFO

extern char jointpath[];
extern char joint_buffer[128];

/*can_init()*/



//�߳�
pthread_t  CAL_AD_thread,CAN_WR_thread, CAN_RD_thread; // Print_thread;
pthread_t GAIT_DATA_thread,FD_MANA_thread;

//�߳�����������
pthread_mutex_t mutex_cond_send;

pthread_cond_t cond_send;

extern pid_t motion_curb_pid;


//ͬ���ź���


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
