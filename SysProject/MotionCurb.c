/*
 * MotionCurb.c
 *
 *  Created on: 2016-4-8
 *      Author: SunTao
 */
/*******************************************************************/
/*
 * Description:
 * 	This source file define the SystemInit.h that declarate system source and pthread, This program mainly include
 * four pthread,which all were managed by a child process of the main's process,and several source initial functions,
 *
 * the four pthread were serial and asynchrony ,but the get can data pthread was not lock by sem, it was conditon_locked
 * by pthread_condition_lock, Note that can_receive_pthread can not be lock by condition lock, otherwise it would be
 * entangled into the asynchronous pthread.
 *
 *
 *
 */
#include "SysProject/MotionCurb.h"
#define MYSIGNAL SIGRTMIN+5
#define BUFFER_SIZE PIPE_BUF+1


/*This function have two initialization function,task_init be used to initial sem,lock pthread and so on;
 * the can_init is attributed to source management initial , this function setup driver reg and parameter*/

float hopedata[NUM_DATAS];
FILE *fp[NUM_FILES];
static volatile sig_atomic_t signal_count1 = 1;//原子类型变量，使用全局变量动态内存和标准IO的数据都是不可重入的，除非定义为原子类型

void * gait_data_pthread(void *arg) {
	int j;
	float data[NUM_COLUMNS];
	unsigned int dadata[NUM_COLUMNS];
	//	int status;
	struct sigaction action, old_action;
	int jointfd;
	fd_set jointfds;
	float buffer[BUFFER_SIZE];

	ARG *p = (ARG *) arg;
	action.sa_handler = sig_handler1;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGTERM);//设置执行期间屏蔽的信号
	action.sa_flags |= 0;
	sigaction(MYSIGNAL, NULL, &old_action);
	if (old_action.sa_handler != SIG_IGN) {
		sigaction(MYSIGNAL, &action, NULL);
	} else {
		perror("fail to set MAYSIGNAL for imu pid\n");
	}
	jointfd = open(jointpath, O_RDONLY);
	if (jointfd == -1) {
		perror("fail to open jointpath fifo\n");
		exit(1);
	}
	memset(buffer,0.0,BUFFER_SIZE);
	printf("gait_data_pthread\n");
	while (1) {

		//*******************load joint hope data,this is a interface
		//	for (j = 0; j < NUM_COLUMNS; j++)
		//		data[j] = hopedata[j + NUM_COLUMNS * i];
		//******************//
		usleep(1000);// set system sample time
		//		printf("signal count1 is %d\n",signal_count1);

		if (signal_count1 == 1) {
			FD_ZERO(&jointfds);
			FD_SET(jointfd, &jointfds);
			if (select(jointfd + 1, &jointfds, 0, NULL, NULL) > 0) {
				if (FD_ISSET(jointfd, &jointfds))
					read(jointfd, buffer, BUFFER_SIZE);
				else
					perror("can recive error:\n");
			} else
				perror("recive select  error:\n");
			for(j=0;j<12;j++)
				data[j]=buffer[j];
			signal_count1 = 1;
		} else {
			for (j = 0; j < NUM_COLUMNS; j++)
				data[j] = 2.5;//处于中位
		}
		GaitDataLtransform(data, dadata);//电压值变换为ad区间
		for (j = 0; j < NUM_COLUMNS; j++)//加载期望值
			p->FdSet.data3[j] = dadata[j];
		PID_Controler(dadata, p->FdSet.data2, p->FdSet.data1); //data1 control ,data2 feed .data3 hope
	}
	close(jointfd);
	pthread_exit((void *) 3);
}

void * can_send_pthread(void *arg) {
	int i;
	int status;
	ARG *p = (ARG *) arg;
	printf("send_pthread\n");
	while (1) {
		for (i = 0; i < NUM_ROWS; i++) {
			//4线程间时间同步
			status = sem_wait(&p->sem_A);
			if (status != 0) {
				perror("sem_wait sem_sen:!\n");
				exit(1);
			}
			pthread_mutex_lock(&mutex_cond_send);//收发条件同步
			while (p->FdSet.fflag[NUM_JOINTS] == 0)
				pthread_cond_wait(&cond_send, &mutex_cond_send);
			p->FdSet.fflag[0] = 0x10;//CAN ID
			p->FdSet.fflag[1] = 0x11;
			p->FdSet.fflag[2] = 0x12;
			can_send(&p->FdSet);
			p->FdSet.fflag[NUM_JOINTS] = 0;
			pthread_mutex_unlock(&mutex_cond_send);
			status = sem_post(&p->sem_B);
			if (status != 0) {
				perror("sem_wait sem_sen:!\n");
				exit(1);
			}
		}
	}
	pthread_exit((void *) 2);
}

void * can_recive_pthread(void *arg) {
	ARG *p = (ARG *) arg;

	printf("receive pthread\n");
	while (1) {
		can_recive(&p->FdSet);//此处切勿将收数据加锁，也不能和发数据同步锁，如此一来条件锁会把所有线程挂起。
		if (p->FdSet.fflag[NUM_JOINTS] == 1) {//检测是否收到Leg_HZ发送的数据
			pthread_cond_signal(&cond_send);
		}


	}
	pthread_exit((void *) 1);
}

void *feddata_manage_pthread(void *arg) {
	ARG *p = (ARG *) arg;
	int status;

	p->status = 1;

	printf("feddata_manage pthread\n");
	while (1) {
		status = sem_wait(&p->sem_B);
		if (status != 0) {
			perror("sem_wait sem_sen:!\n");
			exit(1);
		}

		FedDataSave(fp, p->FdSet.data3, p->FdSet.data2, p->FdSet.data1);//hope,feedback,and control data
		status = sem_post(&p->sem_A);
		if (status != 0) {
			perror("sem_wait sem_sen:!\n");
			exit(1);
		}
	}
	pthread_exit((void *) 4);
}

void *calibrate_ad_pthread(ARG *arg) {
	ARG *p = (ARG *) arg;

	//	int status;

	printf("calibrate_ad pthread\n");
	while (p->status != NULL) {
		p->status = NULL;
		/*		status = sem_wait(&p->sem_B);
		 if (status != 0) {
		 perror("sem_wait sem_sen:!\n");
		 exit(1);
		 }*/
		printf("There is calibrate pthread\n");

		/*		status = sem_post(&p->sem_C);
		 if (status != 0) {
		 perror("sem_wait sem_sen:!\n");
		 exit(1);
		 }*/
	}
	pthread_exit((void *) 6);
}

int task_init(ARG *arg) {
	ARG *p;
	int status;
	p = arg;
	//发送条件
	pthread_cond_init(&cond_send, NULL );
	pthread_mutex_init(&mutex_cond_send, NULL);

	status = sem_init(&p->sem_A, NULL, 1);
	if (status != 0) {
		perror("sem_wait A:!\n");
		exit(1);
	}
	status = sem_init(&p->sem_B, NULL, 1);
	if (status != 0) {
		perror("sem_wait B:!\n");
		exit(1);
	}
	status = sem_init(&p->sem_C, NULL, 1);
	if (status != 0) {
		perror("sem_wait C:!\n");
		exit(1);
	}
	status = sem_init(&p->sem_D, NULL, 1);
	if (status != 0) {
		perror("sem_wait D:!\n");
		exit(1);
	}
	printf("task initiate successful !\n");
	return NULL;
}

int resource_init(ARG *arg) {
	ARG *p;
	int status;
	p = arg;
	p->FdSet.newmask = 0x7fffff;
	p->FdSet.newcode = 0x20;
	p->FdSet.buadrate = 1000;
	status = can_init(CAN0, &p->FdSet);
	if (status != 0) {
		perror("can0_init:!\n");
		exit(1);
	}
	p->FdSet.fflag[0] = Joint1;
	p->FdSet.fflag[1] = Joint2;
	p->FdSet.fflag[2] = Joint3;
	status = fed_file_open(fp, NUM_FILES);
	if (status != 0) {
		perror("fed_file_open fail:!\n");
		exit(1);
	}
	GaitDataLoad(hopedata, NUM_DATAS);
	printf("resource initiate successful! \n");
	return NULL;
}

int resource_end(ARG *arg) {
	ARG *p;
	p = arg;
	can_end(&p->FdSet);
	fed_file_close(fp, NUM_FILES);
	return NULL;
}
int task_end(ARG *arg) {
	ARG *p;
	p = arg;
	sem_destroy(&p->sem_A);
	sem_destroy(&p->sem_B);
	sem_destroy(&p->sem_C);
	sem_destroy(&p->sem_D);

	pthread_cond_destroy(&cond_send);
	pthread_mutex_destroy(&mutex_cond_send);

	return NULL;
}

void sig_handler1(int signum) {
	switch (signum) {
	case SIGUSR1:
		signal_count1 = 1;
		break;
	case SIGUSR2:
		signal_count1 = 2;
		break;
	default:
		signal_count1 = 0;

	}
}
