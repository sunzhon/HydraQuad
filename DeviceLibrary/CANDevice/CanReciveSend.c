/*
 * CanReciveSend.c
 *
 *  Created on: 2016-4-7
 *      Author: Administrator
 */

#include "DeviceLibrary/CANDevice/CanReciveSend.h"

int can_send(CanFdSet_t *FdSet) {

	int k;
	CanFdSet_t *p;
	p = FdSet;
	canmsg_t WR[3];//三个关节+加一个结束位，一个控制指令
	for (k = 0; k < 3; k++) {
		WR[k].data[0] = (unsigned char) (p->data1[k] / 256);
		WR[k].data[1] = (unsigned char) (p->data1[k] % 256);
		WR[k].data[2] = (unsigned char) (p->data1[k + 3] / 256);
		WR[k].data[3] = (unsigned char) (p->data1[k + 3] % 256);
		WR[k].data[4] = (unsigned char) (p->data1[k + 6] / 256);
		WR[k].data[5] = (unsigned char) (p->data1[k + 6] % 256);
		WR[k].data[6] = (unsigned char) (p->data1[k + 9] / 256);
		WR[k].data[7] = (unsigned char) (p->data1[k + 9] % 256);
		WR[k].flags = 0;
		WR[k].length = 8;
		switch (k) {
		case 0:
			WR[k].id = 0x10;
			break;
		case 1:
			WR[k].id = 0x11;
			break;
		case 2:
			WR[k].id = 0x12;
			break;
		default:
			break;
		}

		FD_ZERO(&p->wfds);
		FD_SET(p->wfd,&p->wfds);
		if (select(p->wfd + 1, 0, &p->wfds, NULL, NULL) > NULL) {
			if (FD_ISSET(p->wfd, &p->wfds) > 0) {
				write(p->wfd, &WR[k], sizeof(WR[k]));
			} else
				perror("write wfd error:\n");
		} else
			perror("select write date error:\n");
	}
	return NULL;
}

int can_recive(CanFdSet_t *FdSet) {
	canmsg_t RD[5];
	CanFdSet_t *p;
	int joint1, joint2, joint3;
	unsigned int got;
	unsigned int readcount;
	unsigned int i;
	p = FdSet;
	FD_ZERO(&p->wfds);
	FD_SET(p->wfd, &p->wfds);
	if (select(p->wfd + 1, &(p->wfds), 0, NULL, NULL) > 0) {
		if (FD_ISSET(p->wfd, &p->wfds)) {
			got = read(p->wfd, RD, 5 * sizeof(canmsg_t));
			if (got > 0) {
				readcount = got / 30;
				for (i = 0; i < min(readcount,4); i++) {
					joint1 = RD[i].data[2] + RD[i].data[3] * 256;
					joint2 = RD[i].data[0] + RD[i].data[1] * 256;
					joint3 = RD[i].data[4] + RD[i].data[5] * 256;
					switch (RD[i].id) {
					case QY:
						p->data2[0] = joint1;
						p->data2[1] = joint2;
						p->data2[2] = joint3;
						break;
					case QZ:
						p->data2[3] = joint1;
						p->data2[4] = joint2;
						p->data2[5] = joint3;
						break;
					case HY:
						p->data2[6] = joint1;
						p->data2[7] = joint2;
						p->data2[8] = joint3;
						break;
					case HZ:
						p->data2[9] = joint1;
						p->data2[10] = joint2;
						p->data2[11] = joint3;
						p->fflag[12]=1;
						break;
					default:
						break;
					}
				}
			} else {
				perror("read error:\n");
			}
		} else {
			perror("ISSET  error:\n");
		}
	} else {
		perror("select  error:\n");
	}
	return NULL;
}

int can_init(int canport, CanFdSet_t *FdSet) {
	int newcode;
	CanFdSet_t *p;
	p = FdSet;
	switch (canport) {
	case CAN0:
		p->newcode = 0x20;//CAN1 只接受0x20,0x21,...
		p->newmask = 0x7fffff;
		p->buadrate = 500;
		if ((p->wfd = open(WR_PORT, O_RDWR)) < 0) {
			perror("open can0 error");
			exit(errno);
		}
		printf("fd_wt is %d\n", p->wfd);
		set_bitrate(p->wfd, p->buadrate);
		newcode = (p->newcode << 21) + (0 < 20);
		set_accmask((p->wfd), newcode, (p->newmask));
		break;
	case CAN1:
		p->newcode = 0x30;//CAN2 只接受0x30,0x31,...
		p->newmask = 0x7fffff;
		p->buadrate = 500;
		if ((p->rfd = open(RD_PORT, O_RDWR)) < 0) {
			perror("open read port error\n");
			exit(errno);
		}
		printf("fd_rd is %d\n", p->rfd);
		set_bitrate(p->rfd, p->buadrate);
		newcode = (p->newcode << 21) + (0 < 20);
		set_accmask((p->rfd), newcode, (p->newmask));
		break;
	}

	return NULL;

}
int can_end(CanFdSet_t *FdSet) {
	CanFdSet_t *p;
	p = FdSet;
	close(p->rfd);
	close(p->wfd);
	return NULL;
}
