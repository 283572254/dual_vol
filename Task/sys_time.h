#ifndef _SYS_TIME_H
#define _SYS_TIME_H

#include "main.h"

/**
 * @brief 系统时基
 * 
 */
void Sys_Tick_Count(void);


/**
 * @description: 获取系统滴答计时
 * @param {*}
 * @return {*}
 */
unsigned short int Get_Sys_Tick(void);
unsigned short int Is_Timeout(unsigned short int start, unsigned short int timeout);

#endif
