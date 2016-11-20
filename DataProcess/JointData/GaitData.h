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



//��һ�����ݻ�ṹ������ڶ������֮�䴫��ʱ��ʹ��ȫ�ֱ�����������ں���֮����õ�ʱ�չ�ϵ��ȷ��ʱ��ʹ��ȫ�ֱ���


int FedDataSave(FILE *fp[],unsigned  int hope[],unsigned int feed[],unsigned int control[]);
int GaitDataLtransform(float * Value_HOPE, unsigned int *DA_HOPE);
int GaitDataLoad(float hopeData[],int count);

#endif /* GAITDATA_H_ */
