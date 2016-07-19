#include "HydraQuad.h"

/*
 * Main.c
 *
 *  Created on: 2016-4-8
 *      Author: SunTao
 */
//父进程只作为管理子进程
int main(int argc, char *argv[]) {

	ARG arg1;
	int ret;
	int status;
	void *res_recive;
	void *res_send;
	void *res_cali;
	void *res_fem;
	void *res_dal;
	int unsigned ad_up[12] = { 0 };
	int unsigned ad_down[12] = { 0 };

	printf("main start\n");
	//主函数父进程进行资源初始化和任务初始化

	motion_curb_pid = fork();//任务进程
	if (motion_curb_pid < 0) {
		perror("fork error:\n");
		exit(1);
	} else if (motion_curb_pid == NULL) {
		printf(" motion_curb_pid  pid is :%u\n", getpid());

		if (task_init((ARG *) &arg1) != NULL)//初始化子进程资源，不复制父进程。
			perror("task_init error:\n");
		if (resource_init((ARG *) &arg1) != NULL)//避免附近抢占
			perror("can_init error:\n");

		ad_limit(ad_up, ad_down);

		ret = pthread_create(&CAN_WR_thread, NULL, (void *) &can_send_pthread,
				(void *) &arg1);
		if (ret != 0) {
			perror("Thread Creation Failed");
			exit(EXIT_FAILURE);
		}

		ret = pthread_create(&CAN_RD_thread, NULL, (void *) &can_recive_pthread,
				(void *) &arg1);
		if (ret != 0) {
			perror("Thread Creation Failed");
			exit(EXIT_FAILURE);
		}
		ret = pthread_create(&CAL_AD_thread, NULL, (void *) &calibrate_ad_pthread,
				(void *) &arg1);
		if (ret != 0) {
			perror("Thread calibrate_ad_pthread Failed");
			exit(EXIT_FAILURE);
		}
		ret = pthread_create(&FD_MANA_thread, NULL,
				(void *) &feddata_manage_pthread, (void *) &arg1);
		if (ret != 0) {
			perror("Thread calibrate_ad_pthread Failed");
			exit(EXIT_FAILURE);
		}
		ret = pthread_join(CAL_AD_thread, &res_cali);
		if (ret != 0) {
			perror("Thread join failed");
			exit(EXIT_FAILURE);
		}
		ret = pthread_create(&GAIT_DATA_thread, NULL,
				(void *) &gait_data_pthread, (void *) &arg1);
		if (ret != 0) {
			perror("Thread senddata_load_pthread Failed");
			exit(EXIT_FAILURE);
		}
		ret = pthread_join(CAN_WR_thread, &res_send);
		if (ret != 0) {
			perror("Thread join failed");
			exit(EXIT_FAILURE);
		}

		ret = pthread_join(CAN_RD_thread, &res_recive);
		if (ret != 0) {
			perror("Thread join failed");
			exit(EXIT_FAILURE);
		}
		ret = pthread_join(FD_MANA_thread, &res_fem);
		if (ret != 0) {
			perror("Thread join failed");
			exit(EXIT_FAILURE);
		}
		ret = pthread_join(GAIT_DATA_thread, &res_dal);
		if (ret != 0) {
			perror("Thread join failed");
			exit(EXIT_FAILURE);
		}
		resource_end(&arg1);
		task_end(&arg1);
		printf("main end!\n");
		exit(NULL);
	} else {
		waitpid(motion_curb_pid, &status, WNOHANG);
	}
	return NULL;
}
