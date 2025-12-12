#include "stm32f10x.h"                  // Device header
#include "PWM.h"



#define DIR_PIN GPIO_Pin_10  // PA10（方向）

  
  void steep_motor_init()
  {	//配置普通GPIO口，控制方向
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//时钟使能
	
	GPIO_InitTypeDef GPIO_InitStructure;					//gpio口初始化
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//复用开漏输出
	GPIO_InitStructure.GPIO_Pin = DIR_PIN  ;					//方向输出口gpioa pin 10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
  }
  
  
  // 设置方向（0正转，1反转）
  void Set_Direction(uint8_t dir) 
{
    if (dir) 
        GPIO_SetBits(GPIOA, DIR_PIN);  // 高电平反转
    else 
        GPIO_ResetBits(GPIOA, DIR_PIN); // 低电平正转
}
  



// 启动电机（通过PWM频率控制速度）
void Start_Motor(uint32_t frequency_hz) 
{
    if (frequency_hz == 0) {
        Stop_Motor();  // 频率为0时停止
        return;
    }
    
    // 计算ARR和PSC（假设系统时钟72MHz）
    uint32_t arr = 1000 - 1;                   // 固定ARR值
    uint32_t psc = (72000000 / ((arr+1) * frequency_hz)) - 1;
    
    // 确保PSC值在有效范围内
    if (psc > 0xFFFF) {
        psc = 0xFFFF;  // 限制最大分频值
    }
    
    PWM_Init(arr, psc);                   // 重初始化定时器
    TIM_SetCompare2(TIM1, arr / 2);  	// 占空比50%
}




 
