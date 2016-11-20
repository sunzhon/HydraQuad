/*
 * MotionPlanning.c
 *
 *  Created on: 2016-11-10
 *      Author: SunTao
 */
/*
 * ForceAcq.c
 *
 *  Created on: 2016-11-10
 *      Author: SunTao
 */
#include "SysProject/MotionPlanning.h"
#define MYSIGNAL SIGRTMIN+5
#define BUFFER_SIZE PIPE_BUF+1

void * motion_planning(void * arg) {
	MOPL * p;

	int jointfd;
	int imufd;
	int forcefd;
	int i;
	fd_set fds;
	p = arg;
	unsigned int buffer[BUFFER_SIZE];
	float jointbuffer[BUFFER_SIZE];

	printf("motion planning pthread!\n");
	jointfd = open(jointpath, O_RDWR );//写不要设置阻塞，以用于消除进程相互阻塞
	if (jointfd == -1) {
		perror("fial to open jointpath\n");
		exit(1);
	}
	imufd = open(imupath, O_RDWR);
	if (imufd == -1) {
		perror("fial to open imupath\n");
		exit(1);
	}
	forcefd = open(forcepath, O_RDWR);
	if (forcefd == -1) {
		perror("fial to open forcepath\n");
		exit(1);
	}
	memset(buffer, 0, BUFFER_SIZE );
	memset(jointbuffer,0.0,BUFFER_SIZE);
	sleep(4);//wait for other pid ready
	while (1) {

		usleep(10);// set system sample time
		for (i = 0; i < 12; i++)
			jointbuffer[i] = p->jcdata[i];//load data
		FD_ZERO(&fds);
		FD_SET(jointfd,&fds);
		if (select(jointfd + 1, 0, &fds, NULL, NULL) > NULL) {//允许阻塞
			if (FD_ISSET(jointfd, &fds) > 0)
				write(jointfd, jointbuffer, BUFFER_SIZE);
			else
				perror("write wfd error:\n");
		} else
			perror("select write date error:\n");

		/*
		 printf("p->cpdataQY:%f\t%f\t%f\n", p->jcdata[0], p->jcdata[1],
		 p->jcdata[2]);
		 printf("p->cpdataQZ:%f\t%f\t%f\n", p->jcdata[3], p->jcdata[4],
		 p->jcdata[5]);
		 printf("p->cpdataHY:%f\t%f\t%f\n", p->jcdata[6], p->jcdata[7],
		 p->jcdata[8]);
		 printf("p->cpdataHZ:%f\t%f\t%f\n", p->jcdata[9], p->jcdata[10],
		 p->jcdata[11]);
		 */
		/*
		 if(kill(motion_curb_pid,MYSIGNAL)==-1){
		 perror("fail to kill motion curb\n");
		 exit(1);
		 }

		 if(kill(force_acquisition_pid,MYSIGNAL)==-1){
		 perror("fail to kill force \n");
		 exit(1);
		 }
		 if(kill(imu_acquisition_pid,MYSIGNAL)==-1){
		 perror("fail to kill imu\n");
		 exit(1);
		 }
		 */

		FD_ZERO(&fds);
		FD_SET(forcefd, &fds);
		if (select(forcefd + 1, &fds, 0, NULL, NULL) > 0) {
			if (FD_ISSET(forcefd, &fds))
				read(forcefd, buffer, BUFFER_SIZE);
			else
				perror("read error:\n");
		} else
			perror("select select  error:\n");
		for (i = 0; i < 12; i++)
			p->forcedata[i] = buffer[i];

		FD_ZERO(&fds);
		FD_SET(imufd, &fds);
		if (select(imufd + 1, &fds, 0, NULL, NULL) > 0) {
			if (FD_ISSET(imufd, &fds))
				read(imufd, buffer, BUFFER_SIZE);
			else
				perror("read error:\n");
		} else
			perror("select select  error:\n");
		for (i = 0; i < 16; i++)
			p->imudata[i] = buffer[i];

	}
	close(forcefd);
	close(imufd);
	close(jointfd);
	pthread_exit((void *) 10);

}

void * traj_produce(void * arg) {
	MOPL * p;
	float temp1[12];
	float temp2[12];
	p = arg;
	while (1) {
		usleep(10000);
		trajectory_planning(p);
		ikinematic(p->cpdata, temp1);
		cylinder_elongation(temp1, temp2);
		drive_space(temp2, p->jcdata);
	}
	pthread_exit((void *) 11);
}

int trajectory_planning(MOPL * arg) {
	/*

	 QYPx=   0.03503*sin(3.137*x-1.566 ) +  0.01071*sin( 6.289 *x+3.114 ) +  0.002631 *sin( 9.237*x+1.768);
	 QYPz= .01979*sin(1.55*x+0.2175) + 0.01037*sin(6.728 *x -1.953) + 0.003963*sin( 9.427*x+3.263) + 0.02101*sin(3.371*x+0.24);
	 QYPy=0;
	 QYPx=steplength*Px/0.08;
	 QYPz=stepheight*Pz/0.05;

	 QZPx=0.03008*sin( 3.336*x+2.099 ) + 0.0148*sin(7.341*x-2.916 ) + 0.008264 *sin(11.57 *x-1.953) + 0.004544*sin(15.83 *x-1.034) + 0.001876*sin(20.1*x-6.354);
	 QZPz=0.01625*sin(2.125*x-1.094 ) + 0.01407*sin(6.363*x-0.1341) + 0.0104*sin(10.61*x+0.8215) + 0.006319*sin(14.83*x-4.48) + 0.002889*sin(19.09*x-3.488);
	 QZPy=0;

	 */
	MOPL *p;
	p = arg;
	float initpo[3];
	int i;
	for (i = 0; i < 4; i++) {
		foot_initial(i, initpo);
		p->cpdata[3 * i] = initpo[0]+0.05;
		p->cpdata[3 * i + 1] = initpo[1]+.07;
		p->cpdata[3 * i + 2] = initpo[2]+.08;
	}
	return NULL;

}

int ikinematic(float *cposition, float *jposition) {

	float X, Y, A, B, Px, Py, Pz;
	int i;
	float b = .990 / 2;//%单位 m
	float w = .440 / 2;
	float h = .285;//%%%%机身体高度，此参数不准
	float L2 = .360;
	float L3 = .580;
	float a = .020;
	float c = .070;
	float theta1 = 0;
	float theta2 = 0;
	float theta3 = 0;
	for (i = 0; i < 4; i++) {
		Px = *(cposition + 3 * i);
		Py = *(cposition + 3 * i + 1);
		Pz = *(cposition + 3 * i + 2);

		switch (i) {
		case 0:
			X = Py - c + w;
			Y = Pz + h;
			theta1 = asin(-c / sqrt(X * X + Y * Y)) - atan(X / Y);

			A = Py * sin(theta1) - Pz * cos(theta1) - a - h * cos(theta1) - c
					*sin(theta1) + w * sin(theta1);
			B = Px - b;//90度
			break;
		case 1:
			X = Py + c - w;
			Y = Pz + h;
			theta1 = asin(-c / sqrt(X * X + Y * Y)) - atan(X / Y);

			A = Py * sin(theta1) - Pz * cos(theta1) - a - h * cos(theta1) + c
					*sin(theta1) - w * sin(theta1);
			B = Px - b;
			break;
		case 2:

			X = -Py + c - w;
			Y = Pz + h;
			theta1 = asin(-c / sqrt(X * X + Y * Y)) - atan(X / Y);

			A = c * sin(theta1) - Pz * cos(theta1) - Py * sin(theta1) - h
					*cos(theta1) - a - w * sin(theta1);
			B = -Px - b;
			break;
		case 3:

			X = -Py - c + w;
			Y = Pz + h;
			theta1 = asin(-c / sqrt(X * X + Y * Y)) - atan(X / Y);

			A = w * sin(theta1) - Pz * cos(theta1) - Py * sin(theta1) - h
					*cos(theta1) - c * sin(theta1) - a;
			B = -Px - b;
			break;
		default:
			printf("ikinematic fail\n");
			exit(1);
		}

		theta1 = 0.0;
		theta3 = acos((A * A + B * B - L2 * L2 - L3 * L3) / (2 * L2 * L3));
		theta2 = acos((L3 * sin(theta3) * B + (L2 + L3 * cos(theta3)) * A)
				/ (L2 * L2 + L3 * L3 + 2 * L2 * L3 * cos(theta3)));

		*(jposition + 3 * i) = theta1;
		*(jposition + 3 * i + 1) = theta2;
		*(jposition + 3 * i + 2) = theta3;
	}
	return NULL;
}

int cylinder_elongation(float *jposition, float *hydra_trans) {

	float temp1;
	float temp2;
	float temp3;
	int i;

	for (i = 0; i < 4; i++) {
		temp1 = (*(jposition + 3 * i) + 1.2341);
		temp2 = fabsf(1.9232 - *(jposition + 3 * i + 1));
		temp3 = fabsf(2.4332 - *(jposition + 3 * i + 2));

		temp1 = 0.0918 - 0.0572 * cos(temp1);
		temp2 = 0.0578 - 0.0427 * cos(temp2);
		temp3 = 0.0797 - 0.0572 * cos(temp3);
		temp1 = sqrtf(temp1);
		temp2 = sqrtf(temp2);
		temp3 = sqrtf(temp3);

		*(hydra_trans + 3 * i) = temp1;
		*(hydra_trans + 3 * i + 1) = temp2;
		*(hydra_trans + 3 * i + 2) = temp3;

	}

	return NULL;
}

int drive_space(float *hydra_trans, float *drive_voltage) {
	//The author is suntao
	// DATA is 2015/7/15
	//将液压缸所在杆件的长度转化为液压刚的控制信号，伺服阀控制板的电压值。
	//三个刚的最大生产量分别是100，80，100，控制新号的取值范围是0—5；
	//这里假设伺服阀的伸长量和电压值成正比，不是增量式控制。
	/*
	 L1_min=.220;//这个参数有待验证,髋关节
	 L1_max=.320;//这个参数有待验证，髋关节
	 Lfg_min=.180;//原来185
	 Lfg_max=.255;//原来去250
	 Leh_min=.200;
	 Leh_max=.330;
	 slope1=5.0/(L1_max-L1_min);
	 intercept1=L1_min*slope1;
	 slope2=5.0/(Lfg_max-Lfg_min);
	 intercept2=Lfg_min*slope2;
	 slope3=5.0/(Leh_max-Leh_min);
	 intercept3=Leh_min*slope3; */
	int i;
	float slope[] = { 50.0, 66.667, 38.4615, 50.0, 66.667, 38.4615, 50.0,
			66.667, 38.4615, 50.0, 66.667, 38.4615 };
	float intercept[] = { -11.0, -12.0, -7.6923, -11.0, -12.0, -7.6923, -11.0,
			-12.0, -7.6923, -11.0, -12.0, -7.6923, };
	for (i = 0; i < 12; i++) {
		*(drive_voltage + i) = *(hydra_trans + i) * slope[i] + intercept[i];
	}
	return NULL;
	/*
	 v1=L1*5.0/(L1_max-L1_min)-5*L1_min/(L1_max-L1_min); // 单位 伏
	 v2=Lfg*5.0/(Lfg_max-Lfg_min)-5*Lfg_min/(Lfg_max-Lfg_min);
	 v3=Leh*5.0/(Leh_max-Leh_min)-5*Leh_min/(Leh_max-Leh_min);
	 */
	//%else
	//%    error('input parameter error,which are out of range');
}

int foot_initial(const int leg, float * initialposition) {
	float Px0, Py0, Pz0;
	switch (leg) {
	//  以下是足端在轨迹支撑相中点的位置 不是起始点的位置
	case 0: //前右腿
		Px0 = .480;
		Pz0 = -1.10;
		Py0 = -.220;
		break;

	case 1: //前左腿
		Px0 = .480;
		Pz0 = -1.10;
		Py0 = .220;
		break;
	case 2: //后右腿
		Px0 = -.480;
		Pz0 = -1.10;
		Py0 = -.220;
		break;
	case 3: //后左腿
		Px0 = -.480;
		Pz0 = -1.10;
		Py0 = .220;
		break;
	default:
		exit(1);
		printf("foot initial fail\n");
	}
	*initialposition = Px0;
	*(initialposition + 1) = Py0;
	*(initialposition + 2) = Pz0;
	return NULL;
}

