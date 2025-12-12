#ifndef __PWM_H
#define __PWM_H

void PWM_Init(uint16_t arr, uint16_t psc);
void PWM_SetCompare2(uint16_t Compare);
void Stop_Motor(void);

#endif
