#ifndef __PWM2_H
#define __PWM2_H

#include "stm32f10x.h"

// PB1是TIM3的通道4
// 注意：PB1是TIM3的CH4
void PWM2_Init(uint16_t arr, uint16_t psc);
void PWM2_SetCompare4(uint16_t Compare); // 对应TIM3_CH4
void Stop_Motor2(void);

#endif
