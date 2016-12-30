#ifndef __AMP_CTRL_H
#define __AMP_CTRL_H

#include "hdl_include.h"

#define POWER_CTL_GPIO GPIOE
#define POWER_CTL_PORT GPIO_Pin_8

#define SWITCH_IN_GPIO GPIOE
#define SWITCH_IN_PORT GPIO_Pin_7

#define AC_IN_GPIO GPIOE
#define AC_IN_PORT GPIO_Pin_10

#define DC_IN_GPIO GPIOE
#define DC_IN_PORT GPIO_Pin_9

void amp_init();

void amp_task();

#endif