#include "matrix_keypad.h"

// 去抖时间（毫秒）
static uint32_t debounce_time = 20;

// 简单的微秒延时函数
static void Delay_us(uint32_t us)
{
    us *= 8;  // 根据CPU频率调整这个值
    while(us--) {
        __NOP();  // 空操作
    }
}

// 简单的毫秒延时函数
void Delay_ms(uint32_t ms)
{
    while(ms--) {
        Delay_us(1000);
    }
}

/**
 * @brief  初始化矩阵键盘
 * @param  keypad: 键盘结构体指针
 * @retval 无
 */
void Keypad_Init(MatrixKeypad* keypad)
{
    // 初始化按键映射表
    uint8_t default_keymap[KEYPAD_ROWS][KEYPAD_COLS] = {
        {KEY_1, KEY_2, KEY_3},
        {KEY_4, KEY_5, KEY_6},
        {KEY_7, KEY_8, KEY_9},
        {KEY_STAR, KEY_0, KEY_POUND}
    };
    
    // 复制默认映射表
    for(int i = 0; i < KEYPAD_ROWS; i++) {
        for(int j = 0; j < KEYPAD_COLS; j++) {
            keypad->key_map[i][j] = default_keymap[i][j];
        }
    }
    
    // 初始化其他变量
    keypad->last_key = KEY_NO_PRESS;
    keypad->last_press_time = 0;
}

/**
 * @brief  扫描矩阵键盘
 * @param  keypad: 键盘结构体指针
 * @retval 扫描到的按键值
 */
uint8_t Keypad_Scan(MatrixKeypad* keypad)
{
    uint8_t row, col;
    uint8_t key_pressed = KEY_NO_PRESS;
    
    // 扫描每一列
    for(col = 0; col < KEYPAD_COLS; col++) {
        // 设置当前列为输出低电平
        GPIO_ResetBits(keypad->col_ports[col], keypad->col_pins[col]);
        
        // 短暂延时，让电平稳定
        Delay_us(10);
        
        // 检查每一行
        for(row = 0; row < KEYPAD_ROWS; row++) {
            // 如果检测到行引脚为低电平（按键按下）
            if(GPIO_ReadInputDataBit(keypad->row_ports[row], keypad->row_pins[row]) == 0) {
                // 再次确认按键是否真的按下（去抖）
                Delay_us(1000);  // 1ms延时去抖
                if(GPIO_ReadInputDataBit(keypad->row_ports[row], keypad->row_pins[row]) == 0) {
                    key_pressed = keypad->key_map[row][col];
                    // 等待按键释放
                    while(GPIO_ReadInputDataBit(keypad->row_ports[row], keypad->row_pins[row]) == 0) {
                        // 空循环，等待释放
                    }
                    // 恢复当前列为高电平
                    GPIO_SetBits(keypad->col_ports[col], keypad->col_pins[col]);
                    return key_pressed;
                }
            }
        }
        
        // 恢复当前列为高电平
        GPIO_SetBits(keypad->col_ports[col], keypad->col_pins[col]);
    }
    
    return KEY_NO_PRESS;
}

/**
 * @brief  获取按键（带去抖功能）
 * @param  keypad: 键盘结构体指针
 * @retval 按键值
 */
uint8_t Keypad_GetKey(MatrixKeypad* keypad)
{
    uint8_t current_key = Keypad_Scan(keypad);
    
    // 简单的去抖处理
    static uint8_t last_stable_key = KEY_NO_PRESS;
    static uint32_t last_change_time = 0;
    
    if(current_key != last_stable_key) {
        if(current_key == KEY_NO_PRESS) {
            // 按键释放
            last_stable_key = KEY_NO_PRESS;
            return KEY_NO_PRESS;
        } else {
            // 新按键按下，检查去抖时间
            uint32_t current_time = 0; // 这里需要系统时间，见下面的说明
            if(current_time - last_change_time > debounce_time) {
                last_stable_key = current_key;
                last_change_time = current_time;
                return current_key;
            }
        }
    }
    
    return KEY_NO_PRESS;
}

/**
 * @brief  设置去抖时间
 * @param  time_ms: 去抖时间（毫秒）
 * @retval 无
 */
void Keypad_SetDebounceTime(uint32_t time_ms)
{
    debounce_time = time_ms;
}


