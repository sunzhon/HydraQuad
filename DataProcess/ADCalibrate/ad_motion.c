/*
 * ad_motion.c
 *
 *  Created on: 2016-4-24
 *      Author: Administrator
 */

#include "DataProcess/ADCalibrate/ad_motion.h"
int ad_limit(unsigned int ad_up[], unsigned int ad_down[]) {
	int i;
	FILE *fin;
	/*
	for (i = 0; i < 12; i++) {
		UPLIMIT[2][i] = ad_up[i];
		DOWNLIMIT[2][i] = ad_down[i];
	}
	if (AD_MOTION) {
		////伸极限
		UPLIMIT[2][0] = 600;
		UPLIMIT[2][1] = 529;
		UPLIMIT[2][2] = 475;

		UPLIMIT[2][3] = 530;
		UPLIMIT[2][4] = 716;
		UPLIMIT[2][5] = 780;

		UPLIMIT[2][6] = 643;
		UPLIMIT[2][7] = 556;
		UPLIMIT[2][8] = 611;

		UPLIMIT[2][9] = 561;
		UPLIMIT[2][10] = 538;
		UPLIMIT[2][11] = 538;

/////缩极限
		DOWNLIMIT[2][0] = 425;
		DOWNLIMIT[2][1] = 294;
		DOWNLIMIT[2][2] = 296;

		DOWNLIMIT[2][3] = 351;
		DOWNLIMIT[2][4] = 436;
		DOWNLIMIT[2][5] = 417;

		DOWNLIMIT[2][6] = 509;
		DOWNLIMIT[2][7] = 331;
		DOWNLIMIT[2][8] = 365;

		DOWNLIMIT[2][9] = 400;
		DOWNLIMIT[2][10] = 310;
		DOWNLIMIT[2][11] = 286;
	}
*/


fin = fopen(SENSOR_ZONE, "r"); // 打开文件按读方式打开
for (i = 0; i < NUM_JOINTS_ZONE; i++)
	fscanf(fin, "%d %d", &DOWNLIMIT[2][i],&UPLIMIT[2][i]); // 循环读
fclose(fin); //关闭文件
/*
for(i=0;i<12;i++)
printf("down %d :%d,up %d :%d\n",i,DOWNLIMIT[2][i],i,UPLIMIT[2][i]);
*/
	return NULL;
}

/*
 * UPLIMIT[2][0] = 540;
		UPLIMIT[2][1] = 535;
		UPLIMIT[2][2] = 500;
		UPLIMIT[2][3] = 535;
		UPLIMIT[2][4] = 532;
		UPLIMIT[2][5] = 526;
		UPLIMIT[2][6] = 524;
		UPLIMIT[2][7] = 550;
		UPLIMIT[2][8] = 518;
		UPLIMIT[2][9] = 626;
		UPLIMIT[2][10] = 536;
		UPLIMIT[2][11] = 542;
		DOWNLIMIT[2][0] = 382;
		DOWNLIMIT[2][1] = 298;
		DOWNLIMIT[2][2] = 247;
		DOWNLIMIT[2][3] = 352;
		DOWNLIMIT[2][4] = 290;
		DOWNLIMIT[2][5] = 270;
		DOWNLIMIT[2][6] = 343;
		DOWNLIMIT[2][7] = 318;
		DOWNLIMIT[2][8] = 273;
		DOWNLIMIT[2][9] = 447;
		DOWNLIMIT[2][10] = 308;
		DOWNLIMIT[2][11] = 289;
 * */
