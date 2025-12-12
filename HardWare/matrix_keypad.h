#ifndef __MATRIX_KEYPAD_H
#define __MATRIX_KEYPAD_H

#include "stm32f10x.h"  // 使用标准外设库

// 键盘配置
#define KEYPAD_ROWS    4  // 4行
#define KEYPAD_COLS    3  // 3列

// 按键值定义
#define KEY_1          1
#define KEY_2          2
#define KEY_3          3
#define KEY_4          4
#define KEY_5          5
#define KEY_6          6
#define KEY_7          7
#define KEY_8          8
#define KEY_9          9
#define KEY_STAR       10  // *
#define KEY_0          11
#define KEY_POUND      12  // #
#define KEY_NO_PRESS   0   // 无按键按下

// 键盘结构体
typedef struct {
    GPIO_TypeDef* row_ports[KEYPAD_ROWS];  // 行端口数组
    uint16_t row_pins[KEYPAD_ROWS];         // 行引脚数组
    GPIO_TypeDef* col_ports[KEYPAD_COLS];   // 列端口数组
    uint16_t col_pins[KEYPAD_COLS];         // 列引脚数组
    uint8_t key_map[KEYPAD_ROWS][KEYPAD_COLS]; // 按键映射表
    uint8_t last_key;                       // 上次按键值
    uint32_t last_press_time;               // 上次按键时间
} MatrixKeypad;

// 函数声明
void Keypad_Init(MatrixKeypad* keypad);
uint8_t Keypad_Scan(MatrixKeypad* keypad);
uint8_t Keypad_GetKey(MatrixKeypad* keypad);
void Keypad_SetDebounceTime(uint32_t time_ms);
void Delay_ms(uint32_t ms);  // 简单的延时函数

#endif


