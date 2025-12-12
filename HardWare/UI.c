/**
 * @file UI.c
 * @brief 用户界面管理模块实现 - 修复版
 * @details 
 *   问题修复：
 *   - 改变方向时不再发送参数（只显示更新）
 *   - 确保发送逻辑只在确认输入时触发
 */

#include "UI.h"
#include "OLED.h"
#include "NRF_APP.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* ================= 全局参数 ================= */

uint16_t ui_motor2_speed = 300;
uint32_t ui_motor1_pos   = 0;
uint8_t  ui_motor1_dir   = 0;
uint8_t  ui_motor2_dir   = 0;
uint32_t ui_motor1_total = 0;

uint32_t ui_motor1_2_speed   = 0;   // motor1.2速度
uint8_t  ui_motor1_2_dir   = 0;   // motor1.2方向
uint32_t ui_motor1_2_total = 0;   // motor1.2总位置



/* ================= 页面定义 ================= */



/* ================= 页面状态变量 ================= */

static UI_Page_t UI_Page        = PAGE_MAIN;
static uint8_t   main_cursor    = 0;
static uint8_t   setting_cursor = 0;

static uint8_t   edit_mode      = 0;
static char      input_buf[12]  = {0};
static uint8_t   input_len      = 0;
static uint8_t   dirty          = 1;

static NrfPacket_t tx_pkt;

/* ================= 常量定义 ================= */

#define MAIN_MENU_ITEMS      3
#define SETTING_ITEMS        3
#define MOTOR1_POS_MIN       0
#define MOTOR1_POS_MAX       999999
#define MOTOR2_SPEED_MIN     1
#define MOTOR2_SPEED_MAX     3000


/* ================= 内部函数声明 ================= */

static void ClearInput(void);
static void SendParam(void);
static void RenderMain(void);
static void RenderSetting(uint8_t motor);
//static void RenderInfo(void);
static void UpdateDisplay(void);

/* ================= 工具函数实现 ================= */

static void ClearInput(void)
{
    memset(input_buf, 0, sizeof(input_buf));
    input_len = 0;
}

static void ValidateParams(void)
{
    if (ui_motor1_pos > MOTOR1_POS_MAX) 
        ui_motor1_pos = MOTOR1_POS_MAX;
    
    if (ui_motor2_speed < MOTOR2_SPEED_MIN) 
        ui_motor2_speed = MOTOR2_SPEED_MIN;
    if (ui_motor2_speed > MOTOR2_SPEED_MAX) 
        ui_motor2_speed = MOTOR2_SPEED_MAX;
    if (ui_motor1_2_speed < MOTOR2_SPEED_MIN)  // 使用和motor2相同的范围
        ui_motor1_2_speed = MOTOR2_SPEED_MIN;
    if (ui_motor1_2_speed > MOTOR2_SPEED_MAX)
        ui_motor1_2_speed = MOTOR2_SPEED_MAX;
	
    ui_motor1_dir &= 0x01;
    ui_motor2_dir &= 0x01;
	ui_motor1_2_dir &= 0x01;
}

/**
 * @brief 发送参数到接收端
 * @details 📌 关键：只在数值确认时调用，改变方向不调用
 */
static void SendParam(void)
{
    ValidateParams();

    // 清零数据包
    memset(&tx_pkt, 0, sizeof(tx_pkt));

    tx_pkt.run_flag = 1;

    if (UI_Page == PAGE_SET_M1)
    {
        /* ✅ 发送Motor1参数 */
        tx_pkt.motor1_pos = ui_motor1_pos;
        tx_pkt.motor1_dir = ui_motor1_dir;
		tx_pkt.source = 0;
				
        NRF_APP_SendMotor1(&tx_pkt);
    }
    else if (UI_Page == PAGE_SET_M2)
    {
        /* ✅ 发送Motor2参数 */
        tx_pkt.motor2_speed = ui_motor2_speed;
        tx_pkt.motor2_dir   = ui_motor2_dir;
        
        NRF_APP_SendMotor2(&tx_pkt);
    }
	else if (UI_Page == PAGE_SET_M1_2)
    {
        /* ✅ 发送Motor1参数 */
        tx_pkt.motor1_speed = ui_motor1_2_speed;
        tx_pkt.motor1_dir   = ui_motor1_2_dir;
        tx_pkt.source = 1;
        NRF_APP_SendMotor1(&tx_pkt);
    }
	
}

/* ================= OLED 渲染函数 ================= */

static void RenderMain(void)
{
    OLED_Clear();
    OLED_ShowString(1, 1, "Main Menu");
    
    OLED_ShowString(2, 1, main_cursor == 0 ? "> Motor1" : "  Motor1");
    OLED_ShowString(3, 1, main_cursor == 1 ? "> Motor2" : "  Motor2");
	OLED_ShowString(4, 1, main_cursor == 2 ? "> Motor1.2" : "  Motor1.2");  // 新增行
}

/* 找到 RenderSetting() 函数中的这一段 */

static void RenderSetting(uint8_t motor)
{
    OLED_Clear();
    
    if (motor == 0)
        OLED_ShowString(1, 1, "Set Motor1");
    else if(motor == 1)
        OLED_ShowString(1, 1, "Set Motor2");
	else if(motor == 2)  // 新增motor1.2标题
        OLED_ShowString(1, 1, "Set Motor1.2");
    
    if (setting_cursor == 0)
        OLED_ShowString(2, 1, "> Val:");
    else
        OLED_ShowString(2, 1, "  Val:");
    
    if (edit_mode == 1 && setting_cursor == 0)
    {
        OLED_ShowString(2, 8, input_buf);
    }
    else
    {
         if (motor == 0)
            OLED_ShowNum(2, 8, ui_motor1_pos, 6);      // 位置控制
        else if (motor == 1)
            OLED_ShowNum(2, 8, ui_motor2_speed, 4);    // 速度控制
        else if (motor == 2)
            OLED_ShowNum(2, 8, ui_motor1_2_speed, 6);  // motor1.2速度控制
    }
    
    if (setting_cursor == 1)
        OLED_ShowString(3, 1, "> Dir:");
    else
        OLED_ShowString(3, 1, "  Dir:");
    
     if (motor == 0)
        OLED_ShowNum(3, 8, ui_motor1_dir, 1);
    else if (motor == 1)
        OLED_ShowNum(3, 8, ui_motor2_dir, 1);
    else if (motor == 2)
        OLED_ShowNum(3, 8, ui_motor1_2_dir, 1);  // motor1.2方向
    
    /* ===== 第四行：【改】显示反馈信息 "Pos: 10/1000" ===== */
    if (motor == 0)
    {
        OLED_ShowString(4, 1, "Pos:");
        OLED_ShowNum(4, 5, feedback_current_pos, 4);  /* 当前位置 */
        OLED_ShowString(4, 9, "/");
        OLED_ShowNum(4, 10, 960, 4);  /* 目标位置 */
    }
    else
    {
        if (edit_mode == 0)
            OLED_ShowString(4, 1, "#:Edit *:Back");
        else
            OLED_ShowString(4, 1, "#:OK   *:Cancel");
    }
}

//static void RenderInfo(void)
//{
//    OLED_Clear();
//    
//    if (info_page == 0)
//    {
//        OLED_ShowString(1, 1, "System Info[1/2]");
//        
//        OLED_ShowString(2, 1, "M1 Pos:");
//        OLED_ShowNum(2, 9, ui_motor1_pos, 6);
//        
//        OLED_ShowString(3, 1, "M1 Dir:");
//        OLED_ShowNum(3, 9, ui_motor1_dir, 1);
//        
//        OLED_ShowString(4, 1, "M2 Spd:");
//        OLED_ShowNum(4, 9, ui_motor2_speed, 4);
//    }
//    else if (info_page == 1)
//    {
//        OLED_ShowString(1, 1, "Motor Info [2/2]");
//        
//        OLED_ShowString(2, 1, "M2 Dir:");
//        OLED_ShowNum(2, 9, ui_motor2_dir, 1);
//        
//        OLED_ShowString(3, 1, "Reserved 1:");
//        OLED_ShowNum(3, 13, 0, 4);
//        
//        OLED_ShowString(4, 1, "Reserved 2:");
//        OLED_ShowNum(4, 13, 0, 4);
//    }
//}

static void UpdateDisplay(void)
{
    if (! dirty)
        return;
    
    dirty = 0;
    
    switch (UI_Page)
    {
        case PAGE_MAIN:  
            RenderMain();
            break;
        case PAGE_SET_M1:
            RenderSetting(0);
            break;
        case PAGE_SET_M1_2:  // 新增
            RenderSetting(2);  // motor参数为2
            break;
        default:
            break;
    }
}

/* ================= 初始化函数 ================= */

void UI_Init(void)
{
    UI_Page         = PAGE_MAIN;
    main_cursor     = 0;
    setting_cursor  = 0;
   
    edit_mode       = 0;
    ClearInput();
    dirty           = 1;
    
    UpdateDisplay();
}

/* ================= 按键处理函数 ================= */

static void HandleMainMenu(int key)
{
    switch (key)
    {
        case 2: 
            if (main_cursor > 0)
                main_cursor--;
            else
                main_cursor = MAIN_MENU_ITEMS - 1;
            dirty = 1;
            break;
            
        case 8:
            if (main_cursor < MAIN_MENU_ITEMS - 1)
                main_cursor++;
            else
                main_cursor = 0;
            dirty = 1;
            break;
            
        case 35:
            switch (main_cursor)
            {
                case 0:
                    UI_Page = PAGE_SET_M1;
                    break;
                case 1:
                    UI_Page = PAGE_SET_M2;
                    break;
                case 2:
                    UI_Page = PAGE_SET_M1_2;
                    break;
                default:
                    break;
            }
            
            setting_cursor = 0;
           
            edit_mode      = 0;
            ClearInput();
            dirty = 1;
            break;
            
        default:
            break;
    }
}

static void HandleValueEdit(int key, uint8_t motor)
{
    if (key >= 0 && key <= 9)
    {
        if (input_len < sizeof(input_buf) - 1)
        {
            input_buf[input_len++] = '0' + key;
            input_buf[input_len] = '\0';
            dirty = 1;
        }
    }
    else if (key == 35)  /* # 键：确认 */
    {
        int val = atoi(input_buf);
        
        switch (motor)
		{
		case  0:
            ui_motor1_pos = val;
            ui_motor1_total += val;
			break;
        case  1:
            ui_motor2_speed = val;
			break;
		case  2:
		ui_motor1_2_speed = val;
			break;
		}
		
        ValidateParams();
        SendParam();  /* ✅ 确认时才发送 */
        
        edit_mode = 0;
        ClearInput();
        dirty = 1;
    }
    else if (key == 42)  /* * 键：取消 */
    {
        edit_mode = 0;
        ClearInput();
        dirty = 1;
    }
}

static void HandleSettingView(int key, uint8_t motor)
{
    switch (key)
    {
        case 2:
            if (setting_cursor > 0)
                setting_cursor--;
            else
                setting_cursor = SETTING_ITEMS - 1;
            dirty = 1;
            break;
            
        case 8:
            if (setting_cursor < SETTING_ITEMS - 1)
                setting_cursor++;
            else
                setting_cursor = 0;
            dirty = 1;
            break;
            
        case 35:  /* # 键 */
            if (setting_cursor == 0)
            {
                /* 进入数值编辑态 */
                edit_mode = 1;
                ClearInput();
                dirty = 1;
            }
            else if (setting_cursor == 1)
            {
                /* 🆕 修复：只改变方向，不发送参数 */
                if (motor == 0)
                    ui_motor1_dir ^= 1;
                else if(motor == 1)
                    ui_motor2_dir ^= 1;
				else if(motor == 2)
					ui_motor1_2_dir ^= 1;
                ValidateParams();
                /* ❌ 删除这里的 SendParam() 调用 */
                dirty = 1;
            }
            break;
            
        case 42:  /* * 键：返回主菜单 */
            UI_Page = PAGE_MAIN;
            dirty = 1;
            break;
            
        default:
            break;
    }
}

static void HandleSetting(int key, uint8_t motor)
{
    if (edit_mode == 1)
    {
        HandleValueEdit(key, motor);
    }
    else
    {
        HandleSettingView(key, motor);
    }
}

//static void HandleInfo(int key)
//{
//    switch (key)
//    {
//        case 2:
//            if (info_page > 0)
//                info_page--;
//            else
//                info_page = INFO_PAGES - 1;
//            dirty = 1;
//            break;
//            
//        case 8:
//            if (info_page < INFO_PAGES - 1)
//                info_page++;
//            else
//                info_page = 0;
//            dirty = 1;
//            break;
//            
//        case 35:
//        case 42:
//            UI_Page = PAGE_MAIN;
//            info_page = 0;
//            dirty = 1;
//            break;
//            
//        default:  
//            break;
//    }
//}

void UI_KeyProcess(int key)
{
    if (key == -1)
        return;
    
    switch (UI_Page)
    {
        case PAGE_MAIN:
            HandleMainMenu(key);
            break;
            
        case PAGE_SET_M1:
        case PAGE_SET_M2:
		case PAGE_SET_M1_2:
        {
            uint8_t motor;
            if (UI_Page == PAGE_SET_M1)
                motor = 0;
            else if (UI_Page == PAGE_SET_M2)
                motor = 1;
            else  // PAGE_SET_M1_2
                motor = 2;
                
            HandleSetting(key, motor);
            break;
        }
            
          
            
        default:  
            break;
    }
    
    UpdateDisplay();
}

uint8_t UI_GetCurrentPage(void)
{
    return (uint8_t)UI_Page;
}

uint8_t UI_GetEditMode(void)
{
    return edit_mode;
}
