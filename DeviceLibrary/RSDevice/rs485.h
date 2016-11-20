/*
 * rs485.h
 *
 *  Created on: 2016-11-13
 *      Author: SunTao
 */

#ifndef RS485_H_
#define RS485_H_
#include     <stdio.h>
#include     <stdlib.h>
#include     <string.h>
#include     <unistd.h>
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h>
#include     <termios.h>
#include     <errno.h>
#include     <sys/ioctl.h>
#include     <ctype.h>
#include     <time.h>

int rs485init(void);
void getimudata(int fd,unsigned int * read_buffer);
#endif /* RS485_H_ */
