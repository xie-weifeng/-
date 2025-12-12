#include "PWM2.h"

// 使用TIM3的通道4 (PB1)
void PWM2_Init(uint16_t arr, uint16_t psc)
{
    // 1. 开启时钟
    // TIM3时钟和GPIOB时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // 2. 配置PB1为复用推挽输出（PWM）
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;      // PB1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 3. 配置定时器时基
    TIM_InternalClockConfig(TIM3); // 使用内部时钟
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = arr;    // 自动重装载寄存器值 (ARR)
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc; // 预分频器值 (PSC)
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // TIM3没有重复计数器，设为0
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    // 4. 配置输出比较 (通道4)
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure); // 结构体初始化为默认值
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // PWM模式1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 输出高电平有效
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 使能输出
    // 初始占空比设置为50%
    TIM_OCInitStructure.TIM_Pulse = arr / 2; // 捕获/比较寄存器值 (CCR4)
    
    // 初始化通道4
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
    
    // 5. 使能预装载寄存器
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); // 使能CCR4的预装载
    TIM_ARRPreloadConfig(TIM3, ENABLE); // 使能ARR的预装载
    
    // TIM3是通用定时器，不需要TIM_CtrlPWMOutputs
    
    // 6. 启动定时器
    TIM_Cmd(TIM3, ENABLE);
}


// 设置通道4的比较值 (占空比)
void PWM2_SetCompare4(uint16_t Compare)
{
    TIM_SetCompare4(TIM3, Compare);
}

// 停止PWM输出
void Stop_Motor2(void)
{
    TIM_Cmd(TIM3, DISABLE);
    // 可选：将PB1设为普通GPIO输出低电平，确保电机停止
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_1);
}


