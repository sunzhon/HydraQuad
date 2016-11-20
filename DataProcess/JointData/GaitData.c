/*
 * GaitPlanDataLoad.c
 *
 *  Created on: 2016-4-7
 *      Author: Administrator
 */
#include "DataProcess/JointData/GaitData.h"

int GaitDataLoad(float hopeData[], int count) {

	FILE *fin;
	int Max = (int)count/12;
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

int GaitDataLtransform(float * Value_HOPE,unsigned int *DA_HOPE) {

	int i;
	for (i = 0; i < 12; i++) {
		*(Value_HOPE + i) = (*(Value_HOPE + i)) * (UPLIMIT[2][i]
				- DOWNLIMIT[2][i]) / 5.0 + DOWNLIMIT[2][i];
		*(DA_HOPE + i) = (int) (*(Value_HOPE + i));
	}
	return NULL;

}

int FedDataSave(FILE *fp[], unsigned int hope[],unsigned  int feed[],unsigned int control[]) {
	int i;
/*	char joint[12][4]={"QY1","QY2","QY3","QZ1","QZ2","QZ3","HY1","HY2","HY3","HZ1","HZ2","HZ3"};
	//This sentence is used for view joint data control ,it's useful;
	for(i=0;i<12;i++){
		printf("%s(h-f-c):%d %d %d\n", joint[i],hope[i],feed[i],control[i]);
	}
*/
	/*	printf("QY2222222:%d\n",p->feedta2[1]);
	 printf("QY33333333333333:%d\n",p->feedta2[2]);
	 printf("\n\n\n");
	 printf("QZ11111111111111111111111:%d\n",p->feedta2[3]);
	 printf("QZ22222221111111:%d\n",p->feedta2[4]);
	 printf("QZ333:%d\n",p->feedta2[5]);
	 printf("\n\n\n");
	 */
	/*
	 printf("QY:%d\t%d\t%d\nQZ:%d\t%d\t%d\nHY:%d\t%d\t%d\nHZ:%d\t%d\t%d\n",
	 p->feedta2[0], p->feedta2[1], p->feedta2[2], p->feedta2[3], p->feedta2[4],
	 p->feedta2[5], p->feedta2[6], p->feedta2[7], ,p->feedta2[8], p->feedta2[9],
	 p->feedta2[10], p->feedta2[11]);
	 */
for(i=0;i<4;i++){
	fprintf(fp[i], "%d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\n", hope[3*i], feed[3*i],control[3*i], hope[3*i+1], feed[3*i+1],control[3*i+1],hope[3*i+2], feed[3*i+2],control[3*i+2]);
}
	return NULL;

}
