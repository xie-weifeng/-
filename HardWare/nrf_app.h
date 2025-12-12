#ifndef __NRF_APP_H
#define __NRF_APP_H

#include "stm32f10x.h"

/* ===== 无线数据包结构体 ===== */
typedef struct
{
    uint16_t motor1_pos;
    uint8_t  motor1_dir;
	uint16_t motor1_speed;
    uint16_t motor2_speed;
    uint8_t  motor2_dir;
    uint8_t  run_flag;
	uint8_t  source;
} NrfPacket_t;

/* ===== 【新增】反馈数据包结构体 ===== */
typedef struct
{
    uint16_t motor_current_pos;    // 当前实际位置(mm)
    uint16_t motor_target_pos;     // 目标位置(mm)
    uint8_t  motor_state;          // 电机状态
} NrfFeedback_t;

/* ===== 发送接口 ===== */
void NRF_APP_SendParams(NrfPacket_t *pkt);
void NRF_APP_SendMotor1(NrfPacket_t *pkt);
void NRF_APP_SendMotor2(NrfPacket_t *pkt);

/* ===== 【新增】反馈接收接口 ===== */
void NRF_APP_RX_Feedback(void);

/* ===== 【新增】反馈数据 ===== */
extern volatile uint16_t feedback_current_pos;
extern volatile uint16_t feedback_target_pos;
extern volatile uint8_t  feedback_new;

#endif
