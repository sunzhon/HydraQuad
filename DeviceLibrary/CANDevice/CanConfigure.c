/*
 * CanConfigure.c
 *
 *  Created on: 2016-4-8
 *      Author: SunTao
 */

#include "DeviceLibrary/CANDevice/CanConfigure.h"

int set_accmask(int fd, /* device descriptor */
int newcode, int newmask) {
	Config_par_t cfg;
	volatile Command_par_t cmd;

	cmd.cmd = CMD_STOP;
	ioctl(fd, CAN_IOCTL_COMMAND, &cmd);

	cfg.target = CONF_ACC;
	cfg.val1 = newmask;
	cfg.val2 = newcode;
	ioctl(fd, CAN_IOCTL_CONFIG, &cfg);

	//set single filter
	cfg.target = CONF_ACC_FILTER;
	cfg.val1 = 1;
	ioctl(fd, CAN_IOCTL_CONFIG, &cfg);

	cmd.cmd = CMD_START;
	ioctl(fd, CAN_IOCTL_COMMAND, &cmd);
	return 0;
}

int set_acccode(int fd, /* device descriptor */
int newcode) {
	Config_par_t cfg;
	volatile Command_par_t cmd;
	cmd.cmd = CMD_STOP;
	ioctl(fd, CAN_IOCTL_COMMAND, &cmd);

	cfg.target = CONF_ACCC;
	cfg.val1 = newcode;
	ioctl(fd, CAN_IOCTL_CONFIG, &cfg);

	//set single filter
	cfg.target = CONF_ACC_FILTER;
	cfg.val1 = 1;
	ioctl(fd, CAN_IOCTL_CONFIG, &cfg);

	cmd.cmd = CMD_START;
	ioctl(fd, CAN_IOCTL_COMMAND, &cmd);
	return 0;
}


int set_bitrate(int can_fd, /* device descriptor */int baud /* bit rate */) {
	Config_par_t cfg;
	volatile Command_par_t cmd;

	cmd.cmd = CMD_STOP;
	ioctl(can_fd, CAN_IOCTL_COMMAND, &cmd);

	cfg.target = CONF_TIMING;
	cfg.val1 = baud;
	ioctl(can_fd, CAN_IOCTL_CONFIG, &cfg);

	cmd.cmd = CMD_START;
	ioctl(can_fd, CAN_IOCTL_COMMAND, &cmd);
	return 0;
}
