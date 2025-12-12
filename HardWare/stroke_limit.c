/**
 * @file stroke_limit.c
 * @brief 电机行程限制管理模块实现
 */

#include "stroke_limit.h"

/* ===== 行程管理全局变量 ===== */

static uint32_t stroke_home_position = 0;      ///< 初始位置（Home点）
static uint8_t  stroke_home_set = 0;           ///< 初始位置是否已设置
static uint32_t stroke_limit = 500000;         ///< 行程限制距离（默认500000脉冲）
static uint32_t stroke_current_distance = 0;   ///< 当前运动距离（相对初始位置）

/* ===== 行程管理API实现 ===== */

/**
 * @brief 行程管理系统初始化
 */
void Stroke_Init(void)
{
    stroke_home_position = 0;
    stroke_home_set = 0;
    stroke_limit = 500000;
    stroke_current_distance = 0;
}

/**
 * @brief 设置初始位置（Home点）
 * @return 0=成功, 1=已设置过
 */
uint8_t Stroke_SetHome(void)
{
    if (stroke_home_set == 1)
    {
        return 1;
    }
    
    stroke_home_position = 0;
    stroke_home_set = 1;
    stroke_current_distance = 0;
    
    return 0;
}

/**
 * @brief 设置行程限制距离
 * @param[in] limit 限制距离 (0-999999 脉冲)
 */
void Stroke_SetLimit(uint32_t limit)
{
    if (limit > 999999)
    {
        limit = 999999;
    }
    stroke_limit = limit;
}

/**
 * @brief 检查目标位移是否在允许范围内
 * @param[in] target_pos 目标位移（绝对位置）
 * @return 0=超出范围（禁止运动）, 1=在范围内（允许运动）
 */
uint8_t Stroke_CheckValid(uint32_t target_pos)
{
    if (stroke_home_set == 0)
    {
        return 1;
    }
    
    uint32_t relative_distance;
    
    if (target_pos >= stroke_home_position)
    {
        relative_distance = target_pos - stroke_home_position;
    }
    else
    {
        relative_distance = stroke_home_position - target_pos;
    }
    
    if (relative_distance > stroke_limit)
    {
        return 0;
    }
    
    return 1;
}

/**
 * @brief 更新当前运动距离
 * @param[in] current_pos 电机当前位置（绝对值）
 * @return 0=正常, 1=超过限制（需停止）
 */
uint8_t Stroke_UpdateDistance(uint32_t current_pos)
{
    if (stroke_home_set == 0)
    {
        return 0;
    }
    
    if (current_pos >= stroke_home_position)
    {
        stroke_current_distance = current_pos - stroke_home_position;
    }
    else
    {
        stroke_current_distance = stroke_home_position - current_pos;
    }
    
    if (stroke_current_distance > stroke_limit)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief 获取当前运动距离
 * @return 相对于初始位置的距离 (脉冲数)
 */
uint32_t Stroke_GetCurrentDistance(void)
{
    return stroke_current_distance;
}

/**
 * @brief 获取设置的行程限制
 * @return 行程限制值 (脉冲数)
 */
uint32_t Stroke_GetLimit(void)
{
    return stroke_limit;
}

/**
 * @brief 检查初始位置是否已设置
 * @return 0=未设置, 1=已设置
 */
uint8_t Stroke_IsHomeSet(void)
{
    return stroke_home_set;
}

/**
 * @brief 清除初始位置设置（用于重新设置）
 */
void Stroke_ClearHome(void)
{
    stroke_home_set = 0;
    stroke_current_distance = 0;
}

/**
 * @brief 获取初始位置
 * @return 初始位置值
 */
uint32_t Stroke_GetHomePosition(void)
{
    return stroke_home_position;
}
