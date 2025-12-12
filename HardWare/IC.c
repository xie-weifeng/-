#include "stm32f10x.h"                  // Device header


void IC_Init(void)
{	//开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	

	//配置GPIO口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	//输入捕获通道
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1 ;//通道选择
	TIM_ICInitStructure.TIM_ICFilter= 0xF; //滤波器
	TIM_ICInitStructure.TIM_ICPolarity= TIM_ICPolarity_Rising;//极性选择：边缘选择上升
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1 ;//分频器
	TIM_ICInitStructure.TIM_ICSelection= TIM_ICSelection_DirectTI;//选择直连通道
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	//主从模式配置
	TIM_SelectInputTrigger(TIM2,TIM_TS_TI1FP1);//选择从模式触发源
	TIM_SelectSlaveMode(TIM2,TIM_SlaveMode_Reset);//从模式选择复位
	
	
	TIM_Cmd(TIM2,ENABLE);

}

uint32_t IC_Getfreq(void) 
{
    uint32_t capture = TIM_GetCapture1(TIM2);
    if (capture == 0) return 0;
    
    // 使用浮点运算提高精度
    float freq = 72000000.0f / capture;
    return (uint32_t)(freq + 0.5f); // 四舍五入
}

