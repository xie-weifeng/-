/**
 * @file NRF_APP.c
 * @brief NRF应用层实现 - 发送端
 */

#include "NRF_APP.h"
#include "NRF24L01.h"
#include <string.h>

/* ===== 【新增】反馈数据全局变量 ===== */
volatile uint16_t feedback_current_pos = 0;
volatile uint16_t feedback_target_pos = 0;
volatile uint8_t  feedback_new = 0;

/* ===== 发送函数 ===== */

void NRF_APP_SendParams(NrfPacket_t *pkt)
{
    uint8_t *p = (uint8_t *)pkt;

    for (uint8_t i = 0; i < sizeof(NrfPacket_t); i++)
    {
        NRF24L01_TxPacket[i] = p[i];
    }

    NRF24L01_Send();
}

void NRF_APP_SendMotor1(NrfPacket_t *pkt)
{
    for (uint8_t i = 0; i < sizeof(NrfPacket_t); i++)
    {
        NRF24L01_TxPacket[i] = ((uint8_t *)pkt)[i];
    }

    NRF24L01_SendTo(NRF24L01_RxAddress_P0);
}

void NRF_APP_SendMotor2(NrfPacket_t *pkt)
{
    for (uint8_t i = 0; i < sizeof(NrfPacket_t); i++)
    {
        NRF24L01_TxPacket[i] = ((uint8_t *)pkt)[i];
    }

    NRF24L01_SendTo(NRF24L01_RxAddress_P1);
}

/* ===== 【新增】反馈接收函数 ===== */
void NRF_APP_RX_Feedback(void)
{
    uint8_t status = NRF24L01_Receive();
    
    if (status == 1)
    {
        uint8_t pipe = (NRF24L01_ReadStatus() >> 1) & 0x07;
        
        NrfFeedback_t feedback;
        memcpy(&feedback, NRF24L01_RxPacket, sizeof(NrfFeedback_t));
        
        if (pipe == 0)
        {
            feedback_current_pos = feedback.motor_current_pos;
            feedback_target_pos = feedback.motor_target_pos;
            feedback_new = 1;
        }
    }
}
