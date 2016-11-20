/*
 * ForceAcq.c
 *
 *  Created on: 2016-11-17
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
#include "SysProject/ForceAcq.h"
/*This function have two initialization function,task_init be used to initial sem,lock pthread and so on;*/
/*  the can_init is attributed to source management initial , this function setup driver reg and parameter*/
#define MYSIGNAL SIGRTMIN+5
#define BUFFER_SIZE PIPE_BUF+1
static volatile sig_atomic_t signal_count2;//volatile 告诉编译器不优化该变量。

void * force_get_data_pthread(void *arg) {

	//int status;

	int forcefd;
	int i;
	fd_set forcefds;
	unsigned int buffer[BUFFER_SIZE];

	ForAcq *p = (ForAcq *) arg;
	struct sigaction action, old_action;
	printf("force get data pthread\n");

	action.sa_handler = sig_handler2;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, MYSIGNAL);
	action.sa_flags |= 0;//qnx no spport SA_RESTART yet
	sigaction(MYSIGNAL, NULL, &old_action);
	if (old_action.sa_handler != SIG_IGN) {
		sigaction(MYSIGNAL, &action, NULL);
	} else {
		perror("fail to set MYSIGNAL for force pid\n");
	}

	forcefd = open(forcepath, O_WRONLY);//
	if (forcefd == -1) {
		perror("fail to open force fifo at force_get_data_pthread\n");
		exit(1);
	}
	 memset( buffer, 0, BUFFER_SIZE );
	while (1) {
		usleep(10000);//写入要慢一点，否则force s实时性降低
		for(i=0;i<12;i++)
			buffer[i]=p->FdSet.data2[i];
		FD_ZERO(&forcefds);
		FD_SET(forcefd,&forcefds);
		if (select(forcefd + 1, 0, &forcefds, NULL, NULL) > NULL) {
			if (FD_ISSET(forcefd, &forcefds) > 0)
				write(forcefd, buffer, BUFFER_SIZE);
			else
			perror("write forcefd error:\n");
		} else
		perror("select write forcefd error:\n");

	}
	close(forcefd);

	pthread_exit((void *) 3);
}

void * force_can_send_pthread(void *arg) {

	int status;
	int i;
	ForAcq *p = (ForAcq *) arg;
	printf("force can send pthread\n");
	while (1) {

		status = sem_wait(&p->sem_X);
		 if (status != 0) {
		 perror("sem_wait sem_sen:!\n");
		 exit(1);
		 }
		pthread_mutex_lock(&mutex_cond_send);//收发条件同步
			while (p->FdSet.fflag[12] == 0)
				pthread_cond_wait(&cond_send, &mutex_cond_send);
		p->FdSet.fflag[0] = 0x50;
		p->FdSet.fflag[1] = 0x50;
		p->FdSet.fflag[2] = 0x50;
		for (i = 0; i < 12; i++)
		p->FdSet.data1[i] = 1;
		can_send(&p->FdSet);
		p->FdSet.fflag[12] = 0;
		pthread_mutex_unlock(&mutex_cond_send);
			status = sem_post(&p->sem_Y);
		 if (status != 0) {
		 perror("sem_wait sem_sen:!\n");
		 exit(1);
		 }
	}
	pthread_exit((void *) 2);
}

void * force_can_recive_pthread(void *arg) {
	ForAcq *p = (ForAcq *) arg;
	//	int status;

	printf("force can receive pthread\n");
	while (1) {
		/*	status = sem_wait(&p->sem_Z);
		 if (status != 0) {
		 perror("sem_wait sem_sen:!\n");
		 exit(1);
		 }*/
		can_recive(&p->FdSet);
		if (p->FdSet.fflag[12] == 1) {//检测是否收到Leg_HZ发送的数据
				pthread_cond_signal(&cond_send);
			}

		/*		status = sem_post(&p->sem_X);
		 if (status != 0) {
		 perror("sem_wait sem_sen:!\n");
		 exit(1);
		 }*/
	}
	pthread_exit((void *) 1);
}

void *force_feddata_manage_pthread(void *arg) {
	ForAcq *p = (ForAcq *) arg;
	int status;

	p->status = 1;
	int i;

	printf("force feddata_manage pthread\n");
	while (1) {
		status = sem_wait(&p->sem_Y);
		if (status != 0) {
			perror("sem_wait sem_sen:!\n");
			exit(1);
		}
		for (i = 0; i < 12; i++)
			fprintf(p->fp, "%d\t", p->FdSet.data2[i]);
			fprintf(p->fp, "\n");

		status = sem_post(&p->sem_X);
		if (status != 0) {
			perror("sem_wait sem_sen:!\n");
			exit(1);
		}
	}
	pthread_exit((void *) 4);
}

int force_task_init(ForAcq *arg) {
	ForAcq *p;
	int status;
	p = arg;
	pthread_cond_init(&cond_send, NULL );
	pthread_mutex_init(&mutex_cond_send, NULL);
	status = sem_init(&p->sem_X, NULL, 1);
	if (status != 0) {
		perror("sem_wait X:!\n");
		exit(1);
	}
	status = sem_init(&p->sem_Y, NULL, 1);
	if (status != 0) {
		perror("sem_wait Y:!\n");
		exit(1);
	}
	status = sem_init(&p->sem_Z, NULL, 1);
	if (status != 0) {
		perror("sem_wait Z:!\n");
		exit(1);
	}

	printf("force task initiate successful !\n");
	return NULL;
}

int force_resource_init(ForAcq *arg) {
	ForAcq *p;
	int status;
	time_t time_of_day;
	p = arg;
	p->FdSet.newmask = 0x7fffff;
	p->FdSet.newcode = 0x30;
	p->FdSet.buadrate = 1000;
	status = can_init(CAN1, &p->FdSet);
	if (status != 0) {
		perror("can0_init:!\n");
		exit(1);
	}
	p->FdSet.fflag[0] = Force_para;
	p->FdSet.fflag[1] = Force_para;
	p->FdSet.fflag[2] = Force_para;

	if ((p->fp = fopen(FORCEPATH, "w")) < 0) {
		printf("open QZFED file error\n");
		exit(1);
	}
	fprintf(p->fp, "%s\n\n", ctime(&time_of_day));
	printf("force  resource initiate successful! \n");
	return NULL;
}
int force_resource_end(ForAcq *arg) {
	ForAcq *p;
	p = arg;
	can_end(&p->FdSet);
	fclose(p->fp);
	return NULL;
}
int force_task_end(ForAcq *arg) {
	ForAcq *p;
	p = arg;

	pthread_cond_destroy(&cond_send);
	pthread_mutex_destroy(&mutex_cond_send);

	sem_destroy(&p->sem_X);
	sem_destroy(&p->sem_Y);
	sem_destroy(&p->sem_Z);
	return NULL;
}
void sig_handler2(int signum) {
	switch (signum) {
		case SIGUSR1:
		signal_count2 = 1;
		break;
		case SIGUSR2:
		signal_count2 = 2;
		break;
		default:
		signal_count2 = 0;
	}
}
