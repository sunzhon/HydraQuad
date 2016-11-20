/*
 * GaitData.h
 *
 *  Created on: 2016-4-7
 *      Author: Administrator
 */

#ifndef GAITDATA_H_
#define GAITDATA_H_
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "DataProcess/ADCalibrate/ad_motion.h"

#define QZF "/home/XtaoCan/LPCData/5Value_QZ.txt"
#define QYF "/home/XtaoCan/LPCData/5Value_QY.txt"
#define HZF "/home/XtaoCan/LPCData/5Value_HZ.txt"
#define HYF "/home/XtaoCan/LPCData/5Value_HY.txt"



//当一个数据或结构体变量在多个函数之间传递时，使用全局变量。或变量在函数之间调用的时空关系不确定时，使用全局变量


int FedDataSave(FILE *fp[],unsigned  int hope[],unsigned int feed[],unsigned int control[]);
int GaitDataLtransform(float * Value_HOPE, unsigned int *DA_HOPE);
int GaitDataLoad(float hopeData[],int count);

#endif /* GAITDATA_H_ */
