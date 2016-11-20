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
	ForAcq arg2;
	MOPL arg3;
	IMUACQ arg4;
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


	if (system("rm -d /home/device/force_fifo") == -1) {
		printf("shell1 could not be run\n");
	}
	if (system("rm -d /home/device/joint_fifo") == -1) {
		printf("shell1 could not be run\n");
	}
	if (system("rm -d /home/device/imu_fifo") == -1) {
		printf("shell2 could not be run\n");
	}
	if (mkfifo(jointpath, 0666) < 0) {
		perror("fail to create joint fifo!\n");
		exit(1);
	}
	if (mkfifo(imupath, 0666) < 0) {
		perror("fail to create imu fifo!\n");
		exit(1);
	}
	if (mkfifo(forcepath, 0666) < 0) {
		perror("fail to create imu fifo!\n");
		exit(1);
	}

	motion_curb_pid = fork();//运动控制进程
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

		ret = pthread_create(&CAN_RD_thread, NULL,
				(void *) &can_recive_pthread, (void *) &arg1);
		if (ret != 0) {
			perror("Thread Creation Failed");
			exit(EXIT_FAILURE);
		}
		ret = pthread_create(&CAL_AD_thread, NULL,
				(void *) &calibrate_ad_pthread, (void *) &arg1);
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

		motion_planning_pid = fork();//运动规划进程
		if (motion_planning_pid < 0) {
			perror("fork error:\n");
			exit(1);
		} else if (motion_planning_pid == NULL) {
			printf(" motion_planning_pid  pid is :%u\n", getpid());
			ret = pthread_create(&MP_thread, NULL, (void *) &motion_planning,
					(void *) &arg3);
			if (ret != 0) {
				perror("Thread force receive Failed");
				exit(EXIT_FAILURE);
			}
			ret = pthread_create(&TP_thread, NULL, (void *) &traj_produce,
					(void *) &arg3);
			if (ret != 0) {
				perror("Thread force receive Failed");
				exit(EXIT_FAILURE);
			}

			ret = pthread_join(MP_thread, &res_recive);
			if (ret != 0) {
				perror("Thread join failed");
				exit(EXIT_FAILURE);
			}
			ret = pthread_join(TP_thread, &res_recive);
			if (ret != 0) {
				perror("Thread join failed");
				exit(EXIT_FAILURE);
			}

		} else {
			force_acquisition_pid = fork();
			if (force_acquisition_pid < 0) {
				perror("fork error:\n");
				exit(1);
			} else if (force_acquisition_pid == NULL) {
				printf("force_acquisition pid is :%u\n", getpid());
				if (force_task_init((ForAcq *) &arg2) != NULL)//初始化子进程资源，不复制父进程。
					perror("task_init error:\n");
				if (force_resource_init((ForAcq *) &arg2) != NULL)//避免附近抢占
					perror("can_init error:\n");

				ret = pthread_create(&FORCE_GET_thread, NULL,
						(void *) &force_get_data_pthread, (void *) &arg2);
				if (ret != 0) {
					perror("Thread force gait Failed");
					exit(EXIT_FAILURE);
				}

				ret = pthread_create(&FORCE_RE_thread, NULL,
						(void *) &force_can_recive_pthread, (void *) &arg2);
				if (ret != 0) {
					perror("Thread force receive Failed");
					exit(EXIT_FAILURE);
				}
				ret = pthread_create(&FORCE_SD_thread, NULL,
						(void *) &force_can_send_pthread, (void *) &arg2);
				if (ret != 0) {
					perror("Thread force gait Failed");
					exit(EXIT_FAILURE);
				}
				ret = pthread_create(&FORCE_FD_thread, NULL,
						(void *) &force_feddata_manage_pthread, (void *) &arg2);
				if (ret != 0) {
					perror("Thread force gait Failed");
					exit(EXIT_FAILURE);
				}

				ret = pthread_join(FORCE_GET_thread, &res_recive);
				if (ret != 0) {
					perror("Thread join failed");
					exit(EXIT_FAILURE);
				}

				ret = pthread_join(FORCE_RE_thread, &res_recive);
				if (ret != 0) {
					perror("Thread join failed");
					exit(EXIT_FAILURE);
				}

				ret = pthread_join(FORCE_SD_thread, &res_recive);
				if (ret != 0) {
					perror("Thread join failed");
					exit(EXIT_FAILURE);
				}
				ret = pthread_join(FORCE_FD_thread, &res_recive);
				if (ret != 0) {
					perror("Thread join failed");
					exit(EXIT_FAILURE);
				}

				force_resource_end(&arg2);
				force_task_end(&arg2);

			} else if (force_acquisition_pid > 0) {
				imu_acquisition_pid = fork();
				if (imu_acquisition_pid < NULL) {
					perror("fail to fork imu acq pid");
					exit(1);
				} else if (imu_acquisition_pid == NULL) {
					printf("This is imu acquisition pid :%u\n", getpid());
					ret = pthread_create(&IMU_acq_thread, NULL,
							(void *) &imu_get_data_pthread, (void*) &arg4);
					if (ret != 0) {
						perror("create  imu Thread gait Failed");
						exit(EXIT_FAILURE);
					}
					ret = pthread_create(&IMU_snd_thread, NULL,
							(void *) &imu_send_data_pthread, (void*) &arg4);
					if (ret != 0) {
						perror("create  imu Thread gait Failed");
						exit(EXIT_FAILURE);
					}

					ret= pthread_create(&IMU_FD_thread, NULL,
									(void *) &imu_feddata_manage_pthread,
									(void*) &arg4);
					if (ret != 0) {
						perror("create  imu Thread gait Failed");
						exit(EXIT_FAILURE);
					}

					pthread_join(IMU_acq_thread, &res_recive);
					if (ret != 0) {
						perror("IMU Thread join failed");
						exit(EXIT_FAILURE);
					}
					pthread_join(IMU_snd_thread, &res_recive);
					if (ret != 0) {
						perror("IMU Thread join failed");
						exit(EXIT_FAILURE);
					}
					pthread_join(IMU_FD_thread, &res_recive);
					if (ret != 0) {
						perror("IMU Thread join failed");
						exit(EXIT_FAILURE);
					}
				} else if (imu_acquisition_pid > 0) {
					//there is parent process
					waitpid(motion_planning_pid, &status, WNOHANG);
					waitpid(motion_curb_pid, &status, WNOHANG);
					waitpid(force_acquisition_pid, &status, WNOHANG);
					waitpid(imu_acquisition_pid, &status, WNOHANG);
				}

			}

		}

	}

	return NULL;
}
