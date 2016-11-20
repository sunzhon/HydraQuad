/*
 * JointControler.h
 *
 *  Created on: 2016-4-8
 *      Author: Administrator
 */

#ifndef JOINTCONTROLER_H_
#define JOINTCONTROLER_H_
#include <stdlib.h>
#include <math.h>

#define CONT_UP_LIMIT 500;
#define CONT_DOWN_LIMIT -500;
#define Joint1_Zero 520
#define Joint2_Zero 515
#define Joint3_Zero 520

#define NUM_PARA 12

int PID_Controler(unsigned int * hope, unsigned int * feed,unsigned int * control);



#endif /* JOINTCONTROLER_H_ */
