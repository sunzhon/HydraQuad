/*
 * JointControler.c
 *
 *  Created on: 2016-4-8
 *      Author: Administrator
 */
#include "Algorithm/JointAlgo/JointControler.h"

static int Ex_Err = 0;
static int In_Err = 0;
static int Kp[NUM_PARA] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
static int Kd[NUM_PARA] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };//2016/10/14 将0 修改为1了


int PID_Controler(unsigned int * hope, unsigned int * feed, unsigned int * control) {
	int i;
	int Err;
	int output;
	int up = CONT_UP_LIMIT;
	int down = CONT_DOWN_LIMIT;

	for (i = 0; i < NUM_PARA; i++) {
		Err = (*(hope + i) - *(feed + i)) / 6;//传感器放大倍数
		In_Err = Err - Ex_Err;
		output = Kp[i] * Err + Kd[i] * In_Err;
		Ex_Err = Err;
		output = output > up ? up : output;
		output = output < down ? down : output;
		if ((output > -1) && (output < 1))//设置死区，去干扰
			output = 0;
		switch (i) {
		case 0:
		case 3:
		case 6:
		case 9:
			output += Joint1_Zero;
			break;
		case 1:
		case 4:
		case 7:
		case 10:
			output += Joint2_Zero;
			break;
		case 2:
		case 5:
		case 8:
		case 11:
			output += Joint3_Zero;
			break;
		}
		*(control + i) = output;
	}
	return NULL;
}
