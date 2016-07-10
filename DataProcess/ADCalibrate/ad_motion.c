/*
 * ad_motion.c
 *
 *  Created on: 2016-4-24
 *      Author: Administrator
 */

#include "DataProcess/ADCalibrate/ad_motion.h"
int ad_limit(unsigned int ad_up[], unsigned int ad_down[]) {
	int i;
	for (i = 0; i < 12; i++) {
		UPLIMIT[2][i] = ad_up[i];
		DOWNLIMIT[2][i] = ad_down[i];
	}
	if (AD_MOTION) {
		UPLIMIT[2][0] = 529;
		UPLIMIT[2][1] = 509;
		UPLIMIT[2][2] = 493;
		UPLIMIT[2][3] = 530;
		UPLIMIT[2][4] = 475;
		UPLIMIT[2][5] = 524;
		UPLIMIT[2][6] = 638;
		UPLIMIT[2][7] = 561;
		UPLIMIT[2][8] = 589;
		UPLIMIT[2][9] = 822;
		UPLIMIT[2][10] = 561;
		UPLIMIT[2][11] = 600;

		DOWNLIMIT[2][0] = 373;
		DOWNLIMIT[2][1] = 298;
		DOWNLIMIT[2][2] = 274;
		DOWNLIMIT[2][3] = 351;
		DOWNLIMIT[2][4] = 280;
		DOWNLIMIT[2][5] = 274;
		DOWNLIMIT[2][6] = 507;
		DOWNLIMIT[2][7] = 325;
		DOWNLIMIT[2][8] = 340;
		DOWNLIMIT[2][9] = 695;
		DOWNLIMIT[2][10] = 325;
		DOWNLIMIT[2][11] = 340;
	}

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
