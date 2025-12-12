/**
 * @file stroke_limit.h
 * @brief 电机行程限制管理模块
 * @details 
 *   - 初始位置（Home）检测和记录
 *   - 行程限制设置和验证
 *   - 运动距离监控
 *   - 超限自动保护
 */

#ifndef __STROKE_LIMIT_H
#define __STROKE_LIMIT_H

#include <stdint.h>

/* ===== 行程管理API ===== */

/**
 * @brief 行程管理系统初始化
 */
void Stroke_Init(void);

/**
 * @brief 设置初始位置（Home点）
 * @details 记录当前电机位置作为行程的基准点，运动距离相对此点计算
 * @return 0=成功, 1=已设置过
 */
uint8_t Stroke_SetHome(void);

/**
 * @brief 设置行程限制距离
 * @param[in] limit 限制距离 (0-999999 脉冲)
 */
void Stroke_SetLimit(uint32_t limit);

/**
 * @brief 检查目标位移是否在允许范围内
 * @details 验证：目标位移 <= 初始位置 + 行程限制
 * @param[in] target_pos 目标位移（绝对位置）
 * @return 0=超出范围（禁止运动）, 1=在范围内（允许运动）
 */
uint8_t Stroke_CheckValid(uint32_t target_pos);

/**
 * @brief 更新当前运动距离
 * @details 在运动过程中实时调用，计算相对于初始位置的距离
 * @param[in] current_pos 电机当前位置（绝对值）
 * @return 0=正常, 1=超过限制（需停止）
 */
uint8_t Stroke_UpdateDistance(uint32_t current_pos);

/**
 * @brief 获取当前运动距离
 * @return 相对于初始位置的距离 (脉冲数)
 */
uint32_t Stroke_GetCurrentDistance(void);

/**
 * @brief 获取设置的行程限制
 * @return 行程限制值 (脉冲数)
 */
uint32_t Stroke_GetLimit(void);

/**
 * @brief 检查初始位置是否已设置
 * @return 0=未设置, 1=已设置
 */
uint8_t Stroke_IsHomeSet(void);

/**
 * @brief 清除初始位置设置（用于重新设置）
 */
void Stroke_ClearHome(void);

/**
 * @brief 获取初始位置
 * @return 初始位置值
 */
uint32_t Stroke_GetHomePosition(void);

#endif /* __STROKE_LIMIT_H */

