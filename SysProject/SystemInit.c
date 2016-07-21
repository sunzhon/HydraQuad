/*
 * SystemInit.c
 *
 *  Created on: 2016-4-8
 *      Author: Administrator
 */
#include "SysProject/SystemInit.h"
/*This function have two initialization function,task_init be used to initial sem,lock pthread and so on;
 * the can_init is attributed to source management initial , this function setup driver reg and parameter*/

void * gait_data_pthread(void *arg) {
	int i, j;
	float data[12];
	int dadata[12];
	int status;
	ARG *p = (ARG *) arg;
	printf("gait_data_pthread\n");
	while (1) {
		for (i = 0; i < MAX; i++) {
			status = sem_wait(&p->sem_B);
			if (status != 0) {
				perror("sem_wait sem_sen:!\n");
				exit(1);
			}
			for (j = 0; j < 12; j++)
				data[j] = hopedata[j + 11 * i];

			GaitDataLtransform(data, dadata);

			for (j = 0; j < 12; j++)
				p->FdSet.data1[j] = dadata[j];
			printf("gait_thread!\n");
			status = sem_post(&p->sem_C);
			if (status != 0) {
				perror("sem_wait sem_sen:!\n");
				exit(1);
			}
		}
	}
	pthread_exit((void *) 3);
}

void * can_send_pthread(void *arg) {
	int i;
	int status;
	ARG *p = (ARG *) arg;
	printf("send_pthread\n");
	while (1) {
		for (i = 0; i < MAX; i++) {
			//4线程间时间同步
			status = sem_wait(&p->sem_C);
			if (status != 0) {
				perror("sem_wait sem_sen:!\n");
				exit(1);
			}
			pthread_mutex_lock(&mutex_cond_send);//收发条件同步
			while (p->FdSet.fflag[12] == 0)
				pthread_cond_wait(&cond_send, &mutex_cond_send);
			can_send(&p->FdSet);
			p->FdSet.fflag[12] = 0;
			printf("I am here se\n");
			usleep(TIMESAMPLE);
			pthread_mutex_unlock(&mutex_cond_send);
			status = sem_post(&p->sem_A);
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
		if (p->FdSet.fflag[12] == 1) {//检测是否收到Leg_HZ发送的数据
			printf("I am here re\n");
			pthread_cond_signal(&cond_send);
		}

	}
	pthread_exit((void *) 1);
}

void *feddata_manage_pthread(void *arg) {
	ARG *p = (ARG *) arg;
	int status;
	float sum = 100;
	int zeros = 100;
	int i;
	unsigned int up_limit[12];
	unsigned int down_limit[12];
	p->status = 1;
	for (i = 0; i < 12; i++)
		p->FdSet.fflag[i] = 0;
	printf("feddata_manage pthread\n");
	while (1) {
		status = sem_wait(&p->sem_A);
		if (status != 0) {
			perror("sem_wait sem_sen:!\n");
			exit(1);
		}
		if (p->status != 2) {
			sum = 0;
			for (i = 0; i < 12; i++) {
				sum += (p->FdSet.data2[i] - p->FdSet.fflag[i])
						* (p->FdSet.data2[i] - p->FdSet.fflag[i]);
				p->FdSet.fflag[i] = p->FdSet.data2[i];
			}
			if ((sum < 1000) && (sum > 0))
				zeros = 100 + zeros;
			switch (zeros) {
			case 100:
				p->status = 1;
				for (i = 0; i < 12; i++)
					up_limit[i] = p->FdSet.data2[i];
				break;
			case 200:
				p->status = -1;
				for (i = 0; i < 12; i++)
					down_limit[i] = p->FdSet.data2[i];
				break;
			case 300:
				p->status = 0;
				ad_limit(up_limit, down_limit);
				break;
			default:
				break;
			}
		}

		FedDataSave(fp, p->FdSet.data1, p->FdSet.data2);
		status = sem_post(&p->sem_B);
		if (status != 0) {
			perror("sem_wait sem_sen:!\n");
			exit(1);
		}
	}
	pthread_exit((void *) 4);
}
void *calibrate_ad_pthread(ARG *arg) {

	ARG *p = (ARG *) arg;
	int i;
	int da_up[12];
	int da_down[12];
	int status;
	for (i = 0; i < 12; i++) {
		p->FdSet.fflag[i] = 0;
		da_up[i] = 1000;
		da_down[i] = 0;
	}
	printf("calibrate_ad pthread\n");
	while (p->status != NULL) {
		status = sem_wait(&p->sem_B);
		if (status != 0) {
			perror("sem_wait sem_sen:!\n");
			exit(1);
		}
		switch (p->status) {
		case 1:
			for (i = 0; i < 12; i++)
				p->FdSet.data1[i] = da_up[i];
			break;
		case -1:
			for (i = 0; i < 12; i++)
				p->FdSet.data1[i] = da_down[i];
			break;
		case 0:
			p->status = 2;
			status = sem_post(&p->sem_C);
			if (status != 0) {
				perror("sem_wait sem_sen:!\n");
				exit(1);
			}
			pthread_exit((void *) 6);
			break;
		default:
			break;

		}
		status = sem_post(&p->sem_C);
		if (status != 0) {
			perror("sem_wait sem_sen:!\n");
			exit(1);
		}
		printf("p->status is %d\n", p->status);
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
	status = sem_init(&p->sem_B, NULL, 0);
	if (status != 0) {
		perror("sem_wait B:!\n");
		exit(1);
	}
	status = sem_init(&p->sem_C, NULL, 0);
	if (status != 0) {
		perror("sem_wait C:!\n");
		exit(1);
	}
	status = sem_init(&p->sem_D, NULL, 0);
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
	status = can_init(CAN0, &p->FdSet);
	if (status != 0) {
		perror("can_init:!\n");
		exit(1);
	}
	status = fed_file_open(fp, 4);
	if (status != 0) {
		perror("fed_file_open:!\n");
		exit(1);
	}
	GaitDataLoad(hopedata, 1200);
	printf("resource initiate successful! \n");
	return NULL;
}
int resource_end(ARG *arg) {
	ARG *p;
	p = arg;
	can_end(&p->FdSet);
	fed_file_close(fp, 4);
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
