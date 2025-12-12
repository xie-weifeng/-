#include "keypad.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define NUM_ROWS 4

// KeyMap 数组的唯一定义 (保持不变)
const char KeyMap[4][3] = {
    {'1', '2', '3'},    // R1 (标签 2)
    {'4', '5', '6'},    // R2 (标签 7)
    {'7', '8', '9'},    // R3 (标签 6)
    {'*', '0', '#'}     // R4 (标签 4)
};


// 静态数组存储行引脚的端口和引脚号 (保持不变)
typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} RowConfig_t;

static const RowConfig_t RowConfig[NUM_ROWS] = {
    {.port = R1_PORT, .pin = R1_PIN},
    {.port = R2_PORT, .pin = R2_PIN},
    {.port = R3_PORT, .pin = R3_PIN},
    {.port = R4_PORT, .pin = R4_PIN}
};

// 简单的软件延时函数 (保持不变)
static void Delay_Soft(uint32_t t)
{
    while(t--);
}

// KEYPAD_Init 函数 (保持不变)
void KEYPAD_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    // 行引脚 (R) 配置为推挽输出
    GPIO_InitStructure.GPIO_Pin = R1_PIN; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(R1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = R2_PIN | R3_PIN | R4_PIN; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    R1_HIGH(); R2_HIGH(); R3_HIGH(); R4_HIGH();

    // 列引脚 (C) 配置为上拉输入
    GPIO_InitStructure.GPIO_Pin = C1_PIN | C2_PIN; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = C3_PIN; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// All_Rows_High 和 Set_Row_Low 函数 (保持不变)
static void All_Rows_High(void)
{
    R1_HIGH(); R2_HIGH(); R3_HIGH(); R4_HIGH();
}

static void Set_Row_Low(uint8_t row_index)
{
    All_Rows_High();
    GPIO_ResetBits(RowConfig[row_index].port, RowConfig[row_index].pin);
}


/**
  * @brief  矩阵键盘扫描函数
  * @param  None
  * @retval 0-9: 数字键值, *: 特殊字符, #: 特殊字符, -1: 无按键
  */
int KEYPAD_Scan(void) // ⚡ 关键修改：函数返回类型改为 int
{
    uint8_t row_index;
    char key_char = 0; 
    int key_value = 0; // ⚡ 关键修改：使用 int 存储最终值

    // 1. 设置所有行为高电平
    All_Rows_High();

    // 2. 逐行扫描
    for (row_index = 0; row_index < NUM_ROWS; row_index++)
    {
        Set_Row_Low(row_index);
        Delay_Soft(500); // 电平稳定延时

        // 检查列状态 (低电平意味着按键按下)
        if (C1_READ() == Bit_RESET) { key_char = KeyMap[row_index][0]; goto key_found; }
        if (C2_READ() == Bit_RESET) { key_char = KeyMap[row_index][1]; goto key_found; }
        if (C3_READ() == Bit_RESET) { key_char = KeyMap[row_index][2]; goto key_found; }
    }

    // 3. 扫描结束，无按键按下
    return -1; // ⚡ 关键修改：返回 -1 表示无按键

key_found:
    // 4. 根据字符类型进行返回值转换
    if (key_char >= '0' && key_char <= '9')
    {
        // 转换为数值 (0 -> 0, 1 -> 1, ... 9 -> 9)
        key_value = key_char - '0';
    }
    else
    {
        // 特殊键 ('*' 和 '#')
        // 保持为字符本身（ASCII 码）
        key_value = (int)key_char;
    }
    
    // 5. 消抖和等待释放的逻辑
    Delay_Soft(SystemCoreClock / 1000 * 20); // 按键消抖 20ms

    // 等待释放
    while ( (C1_READ() == Bit_RESET) || (C2_READ() == Bit_RESET) ||
            (C3_READ() == Bit_RESET) );

    // 二次消抖
    Delay_Soft(SystemCoreClock / 1000 * 20); // 20ms

    // 恢复所有行高电平
    All_Rows_High();

    return key_value; // 返回转换后的值 (0-9, '*' 或 '#')
}

