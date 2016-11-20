/*
 * IMUAcq.c
 *
 *  Created on: 2016-11-13
 *      Author: SunTao
 */

#include "SysProject/IMUAcq.h"
#define st (SIGRTMIN+5)
#define BUFFER_SIZE PIPE_BUF+1


volatile sig_atomic_t signal_count3;

void * imu_get_data_pthread(IMUACQ * arg) {
	IMUACQ * p;
	int i;
	int rsfd;
	struct sigaction action, old_action;
	p = arg;
	printf("imu get pthread!\n");
	rsfd = rs485init();

	action.sa_handler = sig_handler3;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGTERM);
	action.sa_flags |= 0;// no support SA_RESTART yet
	sigaction(st, NULL, &old_action);
	if (old_action.sa_handler != SIG_IGN) {
		sigaction(st, &action, NULL);
	} else {
		perror("fail to set MAYSIGNAL for imu pid\n");
	}

	while (1) {
		usleep(5);
			getimudata(rsfd, p->imudata);
			for(i=0;i<16;i++)//此for 循环用于测试，屏蔽了getimudata()的采集，因为传感器没有安装
			p->imudata[i]=i;
	}

	close(rsfd);
	pthread_exit((void *) 11);
}
void * imu_send_data_pthread(IMUACQ * arg) {

	IMUACQ * p;
	int i;
	int imufd;
	fd_set fds;
	unsigned int buffer[BUFFER_SIZE];
	p=arg;
	imufd = open(imupath, O_WRONLY );
	if (imufd == -1) {
		perror("fail to open imupath\n");
		exit(1);
	}
	 memset( buffer, 0, BUFFER_SIZE );
	printf("imu send pthread!\n");
	while (1) {
		for(i=0;i<16;i++)
			buffer[i]=p->imudata[i];//load data
		FD_ZERO(&fds);
		FD_SET(imufd,&fds);
		if (select(imufd + 1, 0, &fds, NULL, NULL) > NULL) {
			if (FD_ISSET(imufd, &fds) > 0)
				write(imufd, buffer, BUFFER_SIZE);
			else
			perror("write forcefd error:\n");
		} else
		perror("select write forcefd error:\n");

	}
	close(imufd);

}


void *imu_feddata_manage_pthread(void *arg) {
	IMUACQ *p = (IMUACQ *) arg;
	int i;
	time_t time_of_day;
	if ((p->fp = fopen(IMUPATH, "w")) < 0) {
		perror("open QZFED file error\n");
		exit(errno);
	}
	fprintf(p->fp, "%s\n\n", ctime(&time_of_day));

	printf("imu feddata_manage pthread\n");
	while (1) {

		for (i = 0; i < 16; i++)
			fprintf(p->fp, "%d\t", p->imudata[i]);
			fprintf(p->fp, "\n");

	}
	fclose(p->fp);
	pthread_exit((void *) 16);
}



void sig_handler3(int signum) {
	switch (signum) {
	case SIGUSR1:
		signal_count3 = 1;
		break;
	case SIGUSR2:
		signal_count3 = 2;
		break;
	default:
		signal_count3 = 0;

	}
}
