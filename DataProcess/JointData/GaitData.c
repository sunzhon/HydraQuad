/*
 * GaitPlanDataLoad.c
 *
 *  Created on: 2016-4-7
 *      Author: Administrator
 */
#include "DataProcess/JointData/GaitData.h"

int GaitDataLoad(float hopeData[], int count) {

	FILE *fin;
	int Max = 100;
	int i;
	float QYData[Max][3];
	float QZData[Max][3];
	float HYData[Max][3];
	float HZData[Max][3];

	fin = fopen(QYF, "r"); // 打开文件按读方式打开
	for (i = 0; i < Max; i++)
		fscanf(fin, "%f %f %f", &QYData[i][0], &QYData[i][1], &QYData[i][2]); // 循环读
	fclose(fin); //关闭文件

	fin = fopen(QZF, "r");
	for (i = 0; i < Max; i++)
		fscanf(fin, "%f %f %f", &QZData[i][0], &QZData[i][1], &QZData[i][2]);
	fclose(fin);

	fin = fopen(HYF, "r");
	for (i = 0; i < Max; i++)
		fscanf(fin, "%f %f %f", &HYData[i][0], &HYData[i][1], &HYData[i][2]);
	fclose(fin);

	fin = fopen(HZF, "r");
	for (i = 0; i < Max; i++)
		fscanf(fin, "%f %f %f", &HZData[i][0], &HZData[i][1], &HZData[i][2]);
	fclose(fin);

	//transform store form
	for (i = 0; i < Max; i++) {
		*(hopeData + 12 * i) = QYData[i][0];
		*(hopeData + 12 * i + 1) = QYData[i][1];
		*(hopeData + 12 * i + 2) = QYData[i][2];
		*(hopeData + 12 * i + 3) = QZData[i][0];
		*(hopeData + 12 * i + 4) = QZData[i][1];
		*(hopeData + 12 * i + 5) = QZData[i][2];
		*(hopeData + 12 * i + 6) = HYData[i][0];
		*(hopeData + 12 * i + 7) = HYData[i][1];
		*(hopeData + 12 * i + 8) = HYData[i][2];
		*(hopeData + 12 * i + 9) = HZData[i][0];
		*(hopeData + 12 * i + 10) = HZData[i][1];
		*(hopeData + 12 * i + 11) = HZData[i][2];
	}

	return NULL;

}

int GaitDataLtransform(float * Value_HOPE, int *DA_HOPE) {

	int i;
	for (i = 0; i < 12; i++) {
		*(Value_HOPE + i) = (*(Value_HOPE + i)) * (UPLIMIT[2][i]
				- DOWNLIMIT[2][i]) / 5.0 + DOWNLIMIT[2][i];
		*(DA_HOPE + i) = (int) (*(Value_HOPE + i));
	}
	return NULL;

}

int FedDataSave(FILE *fp[], int DA[], int AD[]) {

	printf("QY1:%d %d\n", AD[0],DA[0]);
	printf("QY2:%d %d\n", AD[1],DA[1]);
	printf("QY3:%d %d\n", AD[2],DA[2]);

	printf("QZ1:%d %d\n", AD[3],DA[3]);
	printf("QZ2:%d %d\n", AD[4],DA[4]);
	printf("QZ3:%d %d\n", AD[5],DA[5]);

	printf("HY1:%d %d\n", AD[6],DA[6]);
	printf("HY2:%d %d\n", AD[7],DA[7]);
	printf("HY3:%d %d\n", AD[8],DA[8]);

	printf("HZ1:%d %d\n", AD[9],DA[9]);
	printf("HZ2:%d %d\n", AD[10],DA[10]);
	printf("HZ3:%d %d\n", AD[11],DA[11]);

	/*	printf("QY2222222:%d\n",p->data2[1]);
	 printf("QY33333333333333:%d\n",p->data2[2]);
	 printf("\n\n\n");
	 printf("QZ11111111111111111111111:%d\n",p->data2[3]);
	 printf("QZ22222221111111:%d\n",p->data2[4]);
	 printf("QZ333:%d\n",p->data2[5]);
	 printf("\n\n\n");
	 */
	/*
	 printf("QY:%d\t%d\t%d\nQZ:%d\t%d\t%d\nHY:%d\t%d\t%d\nHZ:%d\t%d\t%d\n",
	 p->data2[0], p->data2[1], p->data2[2], p->data2[3], p->data2[4],
	 p->data2[5], p->data2[6], p->data2[7], ,p->data2[8], p->data2[9],
	 p->data2[10], p->data2[11]);
	 */

	fprintf(fp[0], "%d\t %d\t %d\t %d\t %d\t %d\n", DA[0], AD[0], DA[1], AD[1],
			DA[2], AD[2]);
	fprintf(fp[1], "%d\t %d\t %d\t %d\t %d\t %d\n", DA[3], AD[3], DA[4], AD[4],
			DA[5], AD[5]);
	fprintf(fp[2], "%d\t %d\t %d\t %d\t %d\t %d\n", DA[6], AD[6], DA[7], AD[7],
			DA[8], AD[8]);
	fprintf(fp[3], "%d\t %d\t %d\t %d\t %d\t %d\n", DA[9], AD[9], DA[10],
			AD[10], DA[11], AD[11]);
	return NULL;

}
