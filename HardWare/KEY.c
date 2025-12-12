#include "stm32f10x.h"
#include "Delay.h" // 确保 Delay.h 包含 Delay_ms()

//初始化端口
void Key_Init()
{
	// 开启 GPIOB 和 GPIOA 时钟 (修正: GPIOA 时钟必须开启)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
    
    // 配置 GPIOB 按键引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; // 上拉输入，低电平触发
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init (GPIOB, &GPIO_InitStructure);
	
	// 配置 GPIOA 按键引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; // 上拉输入，低电平触发
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init (GPIOA, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
    uint8_t KeyNum = 0;

    // K1: PB1
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
    {
        Delay_ms(20); // 第一次消抖
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
        {
            // 移除 while 等待释放循环，改用非阻塞式读取
            KeyNum = 1;
        }
    }
    
    // K2: PB11
    else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
    {
        Delay_ms(20);
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
        {
            KeyNum = 2;
        }
    }
    
    // K3: PB13 (开关)
    else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
    {
        Delay_ms(20);
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
        {
            KeyNum = 3;
        }
    }
    
    // K4: PB3
    else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0)
    {
        Delay_ms(20);
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0)
        {
            KeyNum = 4;
        }
    }
    
    // K5: PB4
    else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0)
    {
        Delay_ms(20);
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0)
        {
            KeyNum = 5;
        }
    }
    
    // K6: PB5
    else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0)
    {
        Delay_ms(20);
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0)
        {
            KeyNum = 6;
        }
    }

    // 注意：GPIOA Pin 6 未在 Key_GetNum() 中被检查，如果它是按键，需要添加对应的逻辑
    
    return KeyNum;
}

