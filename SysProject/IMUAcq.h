/*
 * IMUAcq.h
 *
 *  Created on: 2016-11-13
 *      Author: SunTao
 */

#ifndef IMUACQ_H_
#define IMUACQ_H_
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/select.h>


#include "DeviceLibrary/RSDevice/rs485.h"
#define IMUPATH "/home/XtaoCan/LPCData/imu.txt"

extern  char imupath[];
extern  char imu_buffer[128];
extern pid_t imu_acquisition_pid;
pthread_t IMU_acq_thread;
pthread_t IMU_snd_thread;
pthread_t IMU_FD_thread;

typedef struct{
	int i;
	FILE * fp;
	unsigned int imudata[16];//协议只需要16个字节,上层一律使用unsigned int ,下层解决数据类型转换
}IMUACQ;


void sig_handler3(int signum);
void * imu_get_data_pthread(IMUACQ * arg);
void * imu_send_data_pthread(IMUACQ * arg);
void *imu_feddata_manage_pthread(void *arg);


#endif /* IMUACQ_H_ */
