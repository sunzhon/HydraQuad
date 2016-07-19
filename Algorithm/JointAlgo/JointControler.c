/*
 * JointControler.c
 *
 *  Created on: 2016-4-8
 *      Author: Administrator
 */
#include "Algorithm/JointAlgo/JointControler.h"

int PID_Controler(int hope[], int feed[], int *manipulation[]) {
	int i;
	int temp;
	int *p;
	for (i = 0; i < 12; i++) {
		p = manipulation[i];
		temp =hope[i] - feed[i];
		*p=10*temp;
	}
	*p=hope[1]-feed[1];
	return NULL;
}


