/*
 * JointDataFile.c
 *
 *  Created on: 2016-4-16
 *      Author: Administrator
 */
#include "DeviceLibrary/FileDevice/JointDataFile.h"


int fed_file_open(FILE *fp[],int count){

		time_t time_of_day;
		int i;

		if ((fp[0] = fopen(QZFFED, "w")) < 0) {
			printf("open QZFED file error\n");
			exit(1);
		}
		if ((fp[1] = fopen(QYFFED, "w")) < 0) {
			printf("open QYFED file error\n");
			exit(1);
		}
		if ((fp[2] = fopen(HZFFED, "w")) < 0) {
			printf("open HZFED file error\n");
			exit(1);
		}
		if ((fp[3] = fopen(HYFFED, "w")) < 0) {
			printf("open HYFED file error\n");
			exit(1);
		}
		for(i=0;i<count;i++){
				fprintf(fp[i], "%s\n", ctime(&time_of_day));
			}
		return NULL;
}

int fed_file_close(FILE *fp[],int count){
	int i;

	for(i=0;i<count;i++){
		fclose(fp[i]);
	}
	return NULL;
}
