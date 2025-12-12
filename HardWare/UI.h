/**
 * @file UI.h
 * @brief 用户界面管理模块头文件
 */

#ifndef __UI_H
#define __UI_H

#include <stdint.h>

/* ================= 页面定义 ================= */
typedef enum
{
    PAGE_MAIN   = 0,        ///< 主菜单页
    PAGE_SET_M1 = 1,        ///< Motor1 设置页
    PAGE_SET_M2 = 2,        ///< Motor2 设置页
    PAGE_SET_M1_2 = 3         ///Motor1.2 设置页
} UI_Page_t;

/* ================= 常量定义 ================= */
//#define MAIN_MENU_ITEMS      4
//#define SETTING_ITEMS        3
//#define INFO_PAGES           2
//#define INFO_LINES_PER_PAGE  3
//#define MOTOR1_POS_MIN       0
//#define MOTOR1_POS_MAX       999999
//#define MOTOR2_SPEED_MIN     1
//#define MOTOR2_SPEED_MAX     3000

/* ================= 全局参数声明（外部可访问） ================= */
extern uint16_t ui_motor2_speed;
extern uint32_t ui_motor1_pos;
extern uint8_t  ui_motor1_dir;
extern uint8_t  ui_motor2_dir;
extern uint32_t ui_motor1_total;

/* ================= 行程限制参数（新增） ================= */
extern uint32_t ui_motor1_max_travel;
extern uint8_t  ui_travel_limit_enabled;

/* ================= 对外接口 ================= */

/**
 * @brief UI系统初始化
 */
void UI_Init(void);

/**
 * @brief 按键处理函数
 */
void UI_KeyProcess(int key);

/**
 * @brief 获取当前UI页面
 */
uint8_t UI_GetCurrentPage(void);

/**
 * @brief 获取当前编辑模式
 */
uint8_t UI_GetEditMode(void);

#endif
