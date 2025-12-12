#include "stm32f10x.h"                  // Device header
#include "flash.h"

#define FLASH_ADDR  0x0800FC00

void Flash_SaveParam(SYS_PARAM *param)
{
    uint16_t *p = (uint16_t *)param;

    FLASH_Unlock();
    FLASH_ErasePage(FLASH_ADDR);

    for(uint8_t i=0;i<sizeof(SYS_PARAM)/2;i++)
    {
        FLASH_ProgramHalfWord(FLASH_ADDR+i*2, p[i]);
    }

    FLASH_Lock();
}

void Flash_LoadParam(SYS_PARAM *param)
{
    uint16_t *p = (uint16_t *)param;

    for(uint8_t i=0;i<sizeof(SYS_PARAM)/2;i++)
    {
        p[i] = *(volatile uint16_t *)(FLASH_ADDR+i*2);
    }
}
