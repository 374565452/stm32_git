#ifndef __POWER_CTL_H
#define __POWER_CTL_H

#include "stm32f10x.h"

void init_power_ctl();

void power_ctl_task();

void create_power_ctl_task();

#endif