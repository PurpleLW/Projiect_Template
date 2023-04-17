/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       detect_task.c/h
  * @brief      detect error task, judged by receiving data time. provide detect
                hook function, error exist function.
  *             检测错误任务， 通过接收数据时间来判断.提供 检测钩子函数,错误存在函数.
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Aug-30-2022     康沃瑞           1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2022 康沃瑞****************************
  */


#ifndef DETECT_TASK_H
#define DETECT_TASK_H

#include "main.h"


#define DETECT_TASK_INIT_TIME 72
#define DETECT_CONTROL_TIME 15


//错误码以及对应设备顺序
enum errorList
{
    IBUS_TOE = 0,
    CHASSIS_MOTOR1_Dri_TOE,
    CHASSIS_MOTOR2_Dri_TOE,
    CHASSIS_MOTOR1_Str_TOE,
    CHASSIS_MOTOR2_Str_TOE,
    CHASSIS_MOTOR1_Bra_TOE,

	
//    YAW_GIMBAL_MOTOR_TOE,
//    PITCH_GIMBAL_MOTOR_TOE,
//    TRIGGER_MOTOR_TOE,
//    BOARD_GYRO_TOE,
//    BOARD_ACCEL_TOE,
//    BOARD_MAG_TOE,
//    REFEREE_TOE,
//    RM_IMU_TOE,
    ERROR_LIST_LENGHT,
};



typedef __packed struct
{
    uint32_t new_time;
    uint32_t last_time;
    uint32_t lost_time;
    uint32_t work_time;
    uint16_t set_offline_time : 12;
    uint16_t set_online_time : 12;
    uint8_t enable : 1;
    uint8_t priority : 4;
    uint8_t error_exist : 1;
    uint8_t is_lost : 1;
    uint8_t data_is_error : 1;

    float frequency;
    unsigned char (*data_is_error_fun)(void);
    void (*solve_lost_fun)(void);
    void (*solve_data_error_fun)(void);
} error_t;



/**
  * @brief          detect task
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
/**
  * @brief          检测任务
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
extern void detect_task(void const *pvParameters);

/**
  * @brief          get toe error status
  * @param[in]      toe: table of equipment
  * @retval         true (eror) or false (no error)
  */
/**
  * @brief          获取设备对应的错误状态
  * @param[in]      toe:设备目录
  * @retval         true(错误) 或者false(没错误)
  */
extern unsigned char toe_is_error(uint8_t err);

/**
  * @brief          record the time
  * @param[in]      toe: table of equipment
  * @retval         none
  */
/**
  * @brief          记录时间
  * @param[in]      toe:设备目录
  * @retval         none
  */
extern void detect_hook(uint8_t toe);

/**
  * @brief          get error list
  * @param[in]      none
  * @retval         the point of error_list
  */
/**
  * @brief          得到错误列表
  * @param[in]      none
  * @retval         error_list的指针
  */
extern const error_t *get_error_list_point(void);


#endif  //detect_task

