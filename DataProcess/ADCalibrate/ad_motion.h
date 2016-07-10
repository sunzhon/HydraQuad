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

int ad_limit(unsigned int ad_up[],unsigned int ad_down[]);
unsigned int UPLIMIT[3][12];
unsigned int DOWNLIMIT[3][12];
#endif /* AD_MOTION_H_ */
