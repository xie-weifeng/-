#ifndef __NRF24L01_H
#define __NRF24L01_H


#include "NRF24L01_Define.h"

/*外部可调用全局数组***********/

extern uint8_t NRF24L01_TxAddress[];
extern uint8_t NRF24L01_TxPacket[];

extern uint8_t NRF24L01_RxAddress[];
extern uint8_t NRF24L01_RxPacket[];


extern uint8_t NRF24L01_TxAddress[5];
extern uint8_t NRF24L01_RxAddress_P0[5];   // ✅ A 通道
extern uint8_t NRF24L01_RxAddress_P1[5];   // ✅ B 通道




uint8_t NRF24L01_SPI_SwapByte(uint8_t Byte);
void NRF24L01_GPIO_Init(void);
void NRF24L01_W_CE(uint8_t BitValue);
void NRF24L01_W_CSN(uint8_t BitValue);	
void NRF24L01_W_SCK(uint8_t BitValue);
void NRF24L01_W_MOSI(uint8_t BitValue);
uint8_t NRF24L01_R_MISO(void);
void NRF24L01_WriteReg(uint8_t RegAdress,uint8_t Data);
uint8_t NRF24L01_ReadReg(uint8_t RegAdress);
void NRF24L01_WriteRegs(uint8_t RegAdress,uint8_t *DataArray/*数组首地址*/,uint8_t Count);
void NRF24L01_ReadRegs(uint8_t RegAdress,uint8_t *DataArray,uint8_t Count);
void NRF24L01_WriteTxPayload(uint8_t *DataArray/*数组首地址*/,uint8_t Count);
void NRF24L01_ReadRxPayload(uint8_t *DataArray,uint8_t Count);
void NRF24L01_FlushTx(void);
void NRF24L01_FlushRx(void);
uint8_t NRF24L01_ReadStatus(void);

void NRF24L01_Init(void);
uint8_t NRF24L01_Send(void);
uint8_t NRF24L01_Receive(void);
void NRF24L01_UpdateRxAddress(void);
uint8_t NRF24L01_Check(void);
uint8_t NRF24L01_SendTo(uint8_t *Addr);


#endif 
