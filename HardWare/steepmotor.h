#ifndef __STEPMOTOR_H
#define __STEPMOTOR_H

#include "stm32f10x.h"

void steep_motor_init(void);
void Set_Direction(uint8_t dir);
void Set_Direction2(uint8_t dir);

void Start_Motor(uint32_t frequency_hz);
void Stop_Motor(void);  // 新增停止函数
void Start_Motor2(uint32_t freq_hz, uint8_t dir);
#endif

