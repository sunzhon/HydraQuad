/*
 * JointDataFile.h
 *
 *  Created on: 2016-4-16
 *      Author: Administrator
 */

#ifndef JOINTDATAFILE_H_
#define JOINTDATAFILE_H_

#include "stdio.h"
#include "stdlib.h"
#include <time.h>

#define QZFFED "/home/XtaoCan/LPCData/QZFED.txt"
#define QYFFED "/home/XtaoCan/LPCData/QYFED.txt"
#define HZFFED "/home/XtaoCan/LPCData/HZFED.txt"
#define HYFFED "/home/XtaoCan/LPCData/HYFED.txt"

int fed_file_close(FILE *fp[],int count);
int fed_file_open(FILE *fp[],int count);

#endif /* JOINTDATAFILE_H_ */
