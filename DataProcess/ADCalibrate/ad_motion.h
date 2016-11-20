/*
 * ad_motion.h
 *
 *  Created on: 2016-4-24
 *      Author: Administrator
 */

#ifndef AD_MOTION_H_
#define AD_MOTION_H_
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define AD_MOTION 1
#define NUM_JOINTS_ZONE 12
#define NUM_ZONES 3

#define SENSOR_ZONE "/home/XtaoCan/LPCData/5SensorZone.txt"

int ad_limit(unsigned int ad_up[],unsigned int ad_down[]);
unsigned int UPLIMIT[NUM_ZONES][NUM_JOINTS_ZONE];
unsigned int DOWNLIMIT[NUM_ZONES][NUM_JOINTS_ZONE];
#endif /* AD_MOTION_H_ */
