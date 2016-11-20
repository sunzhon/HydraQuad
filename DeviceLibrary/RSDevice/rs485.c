/*
 * rs485.c
 *
 *  Created on: 2016-11-13
 *      Author: SunTao
 */
#include "DeviceLibrary/RSDevice/rs485.h"
static const char rs485path[] = "/dev/ser3";

int rs485init(void) {
	int fd;

	struct termios opt;
	fd = open(rs485path, O_RDWR | O_NOCTTY); //默认为阻塞读方式
	if (fd == -1) {
		perror("open serial 0\n");
		exit(1);
	}

	tcgetattr(fd, &opt);
	cfsetispeed(&opt, B115200);
	cfsetospeed(&opt, B115200);

	if (tcsetattr(fd, TCSANOW, &opt) != 0) {
		perror("tcsetattr error");
		exit(1);
	}

	opt.c_cflag |= (CLOCAL | CREAD);
	opt.c_cflag &= ~CSIZE;
	opt.c_cflag |= CS8;
	opt.c_cflag &= ~PARENB; //无校验
	opt.c_iflag &= ~INPCK;
	opt.c_cflag &= ~CSTOPB; //1位停止位

	/* Raw模式输入成员 */
	//opt.c_iflag = 0;
	/* Raw模式输出成员 */
	//opt.c_oflag = 0;
	/* 本地模式成员 */
	//opt.c_lflag = 0;

	opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //raw模式
	opt.c_oflag &= ~OPOST;

	opt.c_iflag &= ~(INLCR | ICRNL | IGNCR); //屏蔽回车空格
	opt.c_oflag &= ~(ONLCR | OCRNL);

	opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

	//opt.c_iflag &= ~(ICRNL | INLCR);
	opt.c_iflag &= ~(IXON | IXOFF | IXANY); //添加的,软件流控制屏蔽

	//opt.c_iflag &= ~(IXON);

	//opt.c_cflag |= CRTSCTS;
	//opt.c_cflag &= ~CRTSCTS;

	opt.c_cc[VTIME] = 10;
	opt.c_cc[VMIN] = 17;

	tcflush(fd, TCIOFLUSH);

	printf("configure complete\n");

	if (tcsetattr(fd, TCSANOW, &opt) != 0) {
		perror("serial error");
		exit(1);
	}
	return fd;

}
void getimudata(int fd, unsigned int *read_buffer) {
	int status;
	int ret;
	int i;
	const unsigned char write_test[5] = { 0xFA, 0x01, 0x85, 0x00, 0x7E };
	unsigned char buffer[20];
	bzero(buffer, sizeof(buffer)); //类似于memset

	ioctl(fd, TIOCMGET, &status);
	status |= TIOCM_RTS;
	ioctl(fd, TIOCMSET, &status);

	ret = write(fd, write_test, sizeof(write_test));
	if (ret == -1) {
		perror("fail to write imu command to imu \n");
		exit(1);
	}

	usleep(1);

	ioctl(fd, TIOCMGET, &status);
	status &= ~TIOCM_RTS;
	ioctl(fd, TIOCMSET, &status); //set RTS in receive mode
	ret = read(fd, buffer, sizeof(buffer));
	if (ret == -1) {
		perror("fail to read data form imu device \n");
	}
	for (i = 0; i < 16; i++)
		*(read_buffer + i) = (unsigned int) buffer[i];

}
