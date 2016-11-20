/*
 * HydraQuad.h
 *
 *  Created on: 2016-4-9
 *      Author: SunTao
 */

#ifndef HYDRAQUAD_H_
#define HYDRAQUAD_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "DeviceLibrary/CANDevice/CanConfigure.h"
#include "DeviceLibrary/CANDevice/CanReciveSend.h"
#include "DeviceLibrary/FileDevice/JointDataFile.h"
#include "SysProject/MotionCurb.h"
#include "SysProject/ForceAcq.h"
#include "SysProject/MotionPlanning.h"
#include "SysProject/IMUAcq.h"


/*task_init()*/
/***HydraQuad be used for process manage*/
pid_t motion_curb_pid;
pid_t motion_planning_pid;
pid_t force_acquisition_pid;
pid_t imu_acquisition_pid;

//IPC �ؽڿ������ݣ����������ݣ�
//�����ܵ�,��ʹ�ùܵ�
 //int pipe_force[2];
 //char force_buffer[128];
//�����ܵ�FIFO���ǶȺ�imuʹ��fifo

char jointpath[]="/home/device/joint_fifo";
char joint_buffer[128];

char imupath[]="/home/device/imu_fifo";
char imu_buffer[128];

char forcepath[]="/home/device/force_fifo";
char force_buffer[128];

#endif /* HYDRAQUAD_H_ */
