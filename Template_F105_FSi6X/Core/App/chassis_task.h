#ifndef CHASSIS_TASK_H
#define CHASSIS_TASK_H

#include "main.h"
#include "CAN_receive.h"
#include "remote_control.h"



typedef struct
{
  const motor_measure_t *chassis_motor_measure;
  int16_t angle_set;
  int16_t speed;
  int32_t speed_set;
  int16_t give_current;
	
	uint8_t brak_set;
	uint8_t speed_gear;
	uint8_t speed_flag;
	uint8_t angel_flag;

} chassis_motor_t;


typedef struct
{
  const RC_ctrl_t *chassis_RC;               //底盘使用的遥控器指针, the point to remote control
//  chassis_mode_e chassis_mode;               //state machine. 底盘控制状态机
//  chassis_mode_e last_chassis_mode;          //last state machine.底盘上次控制状态机
  chassis_motor_t motor_chassis[NAME_LIST_LENGHT];          //chassis motor data.底盘电机数据

	uint8_t	SW_num;
	int16_t	 speed;
	uint8_t chaiss_MODE;
} chassis_move_t;

extern const chassis_move_t *get_chassis_move_point(void);


#endif  //chassis_task

