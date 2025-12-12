#ifndef __KEYPAD_H
#define __KEYPAD_H

#include "stm32f10x.h" // 标准外设库主头文件

/*
 * 4x3 矩阵键盘引脚定义 (SPL 版本，避开 PA0-PA4, PB6-PB9)
 * R1 (PA6), R2 (PB11), R3 (PB10), R4 (PB0)
 * C1 (PA7), C2 (PA5), C3 (PB1)
 */

// --- 端口和引脚定义 (保持不变) ---
// R1 (标签 2), C2 (标签 1), C1 (标签 3) 使用 GPIOA
#define R1_PORT         GPIOA
#define R1_PIN          GPIO_Pin_6
#define C1_PORT         GPIOA
#define C1_PIN          GPIO_Pin_7
#define C2_PORT         GPIOA
#define C2_PIN          GPIO_Pin_5

// R4 (标签 4), C3 (标签 5) 使用 PB0, PB1
#define R4_PORT         GPIOB
#define R4_PIN          GPIO_Pin_0
#define C3_PORT         GPIOB
#define C3_PIN          GPIO_Pin_1

// R3 (标签 6), R2 (标签 7) 使用 PB10, PB11
#define R3_PORT         GPIOB
#define R3_PIN          GPIO_Pin_10
#define R2_PORT         GPIOB
#define R2_PIN          GPIO_Pin_11


// --- 宏定义：行输出/列输入 (保持不变) ---
#define R1_LOW()        GPIO_ResetBits(R1_PORT, R1_PIN)
#define R2_LOW()        GPIO_ResetBits(R2_PORT, R2_PIN)
#define R3_LOW()        GPIO_ResetBits(R3_PORT, R3_PIN)
#define R4_LOW()        GPIO_ResetBits(R4_PORT, R4_PIN)

#define R1_HIGH()       GPIO_SetBits(R1_PORT, R1_PIN)
#define R2_HIGH()       GPIO_SetBits(R2_PORT, R2_PIN)
#define R3_HIGH()       GPIO_SetBits(R3_PORT, R3_PIN)
#define R4_HIGH()       GPIO_SetBits(R4_PORT, R4_PIN)

#define C1_READ()       GPIO_ReadInputDataBit(C1_PORT, C1_PIN)
#define C2_READ()       GPIO_ReadInputDataBit(C2_PORT, C2_PIN)
#define C3_READ()       GPIO_ReadInputDataBit(C3_PORT, C3_PIN)

// KeyMap 声明 (保持不变)
extern const char KeyMap[4][3]; 

// 矩阵键盘驱动初始化函数 (保持不变)
void KEYPAD_Init(void);

// 矩阵键盘扫描函数 ⚡ 关键修改：返回类型改为 int
// 返回值：-1 (无按键), 0-9 (数字键), '*'/'#' (字符ASCII码)
int KEYPAD_Scan(void); 

#endif /* __KEYPAD_H */


