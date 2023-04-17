/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       chassis_task.c/h
  * @brief      底盘控制任务
	*
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Aug-24-2022     康沃瑞           1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2022 康沃瑞****************************
  */
	
#include "chassis_task.h"
#include "cmsis_os.h"
#include "can.h"
#include "detect_task.h"


//底盘运动数据
chassis_move_t chassis_move;
static uint32_t greal_time_F=0;
static uint32_t greal_time_B=0;


void check_IN_RC(void);
void CAN_Send_Tx_Fun(void);
void SW_Fun(const uint8_t SW_num);


const chassis_move_t *get_chassis_move_point(void)
{
    return &chassis_move;
}
/**
  * @brief          accroding to the channel value of remote control, calculate chassis vertical and horizontal speed set-point
  *                 
  * @param[out]     vx_set: vertical speed set-point
  * @param[out]     vy_set: horizontal speed set-point
  * @param[out]     chassis_move_rc_to_vector: "chassis_move" valiable point
	* @param[in]
  * @retval         none
  */
/**
  * @brief          根据遥控器通道值，计算纵向和横移速度
  *                 
  * @param[out]     vx_set: 纵向速度指针
  * @param[out]     vy_set: 横向速度指针
  * @param[out]     chassis_move_rc_to_vector: "chassis_move" 变量指针
	* @param[in]
  * @retval         none
  */
void chassis_task(void const * argument)
{
  /* USER CODE BEGIN chassis_task */
  /* Infinite loop */
    chassis_move.motor_chassis[AJN_Motor_flag_1].chassis_motor_measure = get_drive_motor_1_measure_point();
    chassis_move.motor_chassis[AJN_Motor_flag_1].chassis_motor_measure = get_drive_motor_2_measure_point();
    chassis_move.motor_chassis[GL_Steer_flag_1].chassis_motor_measure = get_steering_motor_1_measure_point();
    chassis_move.motor_chassis[GL_Brake_flag].chassis_motor_measure = get_brake_motor_measure_point();
	
		chassis_move.chassis_RC = get_remote_control_point();
  for(;;)
  {
		chassis_move.motor_chassis[AJN_Motor_flag_1].speed_flag = 0xA5;
		chassis_move.motor_chassis[AJN_Motor_flag_2].speed_flag = 0xA5;
		chassis_move.motor_chassis[GL_Steer_flag_1].angel_flag = 0x01;

		if(switch_is_up(chassis_move.chassis_RC->rc.s[0])&&(toe_is_error(IBUS_TOE) == 0))  // 遥控模式
		{
			chassis_move.chaiss_MODE = 0x45;
			if((chassis_move.chassis_RC->rc.ch[3] < -800)&&(chassis_move.chassis_RC->rc.ch[0] == 0))
			{
				if(xTaskGetTickCount() - greal_time_F> 2000)
				{
					chassis_move.motor_chassis[AJN_Motor_flag_2].speed_gear = chassis_move.motor_chassis[AJN_Motor_flag_1].speed_gear = 0xAA;
				}
			}
			else 
				greal_time_F = xTaskGetTickCount();
			
			if((chassis_move.chassis_RC->rc.ch[3] > 800)&&(chassis_move.chassis_RC->rc.ch[0] == 0))
			{
				if(xTaskGetTickCount() - greal_time_B> 2000)
				{
					chassis_move.motor_chassis[AJN_Motor_flag_2].speed_gear = chassis_move.motor_chassis[AJN_Motor_flag_1].speed_gear = 0x55;					
				}					
			}
			else 
				greal_time_B = xTaskGetTickCount();
	
			if(chassis_move.chassis_RC->rc.ch[2] >= 0)
			{
				chassis_move.motor_chassis[AJN_Motor_flag_1].speed_set = (int16_t)((float)chassis_move.chassis_RC->rc.ch[2] / 1000.0f * (chassis_move.chassis_RC->rc.var[0]-1000)*5);	
				chassis_move.motor_chassis[AJN_Motor_flag_2].speed_set = chassis_move.motor_chassis[AJN_Motor_flag_1].speed_set;	
				chassis_move.motor_chassis[GL_Brake_flag].brak_set = 0;				
			}
			
		if(chassis_move.chassis_RC->rc.ch[1] < 0)
		{
			chassis_move.motor_chassis[GL_Brake_flag].brak_set = (uint8_t)((float)(-chassis_move.chassis_RC->rc.ch[1]) / 1000.0f * (chassis_move.chassis_RC->rc.var[1]-1000)/10);
			if(chassis_move.motor_chassis[GL_Brake_flag].brak_set > 9)
				chassis_move.motor_chassis[AJN_Motor_flag_1].speed_set = chassis_move.motor_chassis[AJN_Motor_flag_2].speed_set = 0;
		}
			
		chassis_move.motor_chassis[GL_Steer_flag_1].angle_set = (int16_t)((float)chassis_move.chassis_RC->rc.ch[0] / 1000.0f * 540);
		
		}
		else if((switch_is_down(chassis_move.chassis_RC->rc.s[0])|| toe_is_error(IBUS_TOE)) && (chassis_move.chassis_RC->ipc.flag == 1))  // 上位机模式
		{
			chassis_move.chaiss_MODE = 0xA5;
			if(((xTaskGetTickCount() - chassis_move.chassis_RC->ipc.time) > 1000))
			{
				
				chassis_move.motor_chassis[AJN_Motor_flag_1].speed_set = 0;
				chassis_move.motor_chassis[AJN_Motor_flag_2].speed_set = 0;			
				chassis_move.motor_chassis[GL_Steer_flag_1].angle_set = 0;			
				chassis_move.motor_chassis[GL_Brake_flag].brak_set  = 0;
				
			}
			else 
			{
				chassis_move.motor_chassis[AJN_Motor_flag_1].speed_gear = chassis_move.chassis_RC->ipc.gear;
				chassis_move.motor_chassis[AJN_Motor_flag_2].speed_gear = chassis_move.chassis_RC->ipc.gear;

				chassis_move.motor_chassis[AJN_Motor_flag_1].speed_set = chassis_move.chassis_RC->ipc.rpm;
				chassis_move.motor_chassis[AJN_Motor_flag_2].speed_set = chassis_move.chassis_RC->ipc.rpm;
				
				chassis_move.motor_chassis[GL_Steer_flag_1].angle_set = chassis_move.chassis_RC->ipc.angle*18/10;
				
				chassis_move.motor_chassis[GL_Brake_flag].brak_set = chassis_move.chassis_RC->ipc.barke;
				
				chassis_move.SW_num = chassis_move.chassis_RC->ipc.Switch;
			}
		}
		else 
		{
				chassis_move.chaiss_MODE = 0x00;
				chassis_move.motor_chassis[AJN_Motor_flag_1].speed_set = 0;
				chassis_move.motor_chassis[AJN_Motor_flag_2].speed_set = 0;			
				chassis_move.motor_chassis[AJN_Motor_flag_1].speed_flag = 0x00;
				chassis_move.motor_chassis[AJN_Motor_flag_2].speed_flag = 0x00;
				chassis_move.motor_chassis[AJN_Motor_flag_1].speed_gear = 0x00;
				chassis_move.motor_chassis[AJN_Motor_flag_2].speed_gear = 0x00;

				chassis_move.motor_chassis[GL_Steer_flag_1].angel_flag = 0x00;
				chassis_move.motor_chassis[GL_Steer_flag_1].angle_set = 0;			
			
				chassis_move.motor_chassis[GL_Brake_flag].brak_set  = 0;
			
		}
		
		
		check_IN_RC();
		
		SW_Fun(chassis_move.SW_num);
		
		chassis_move.speed = chassis_move.motor_chassis[AJN_Motor_flag_1].chassis_motor_measure->speed_rpm  * 60 / 10 * 189  / 100 / 20 ;   // RPM * 轮胎周长(m) * 60 * 减速比
		
		CAN_Send_Tx_Fun();
		
		osDelay(10);
  }
  /* USER CODE END chassis_task */
}

void check_IN_RC()
{
	if(RPM_MAX < chassis_move.motor_chassis[AJN_Motor_flag_1].speed_set)
		chassis_move.motor_chassis[AJN_Motor_flag_1].speed_set = 0; 
	
	if(RPM_MAX < chassis_move.motor_chassis[AJN_Motor_flag_2].speed_set)
		chassis_move.motor_chassis[AJN_Motor_flag_2].speed_set = 0; 
	
	if(BRAKE_MAX  < chassis_move.motor_chassis[GL_Brake_flag].brak_set)
		chassis_move.motor_chassis[GL_Brake_flag].brak_set = 0;

	if((ANGLE_MAX  < chassis_move.motor_chassis[GL_Steer_flag_1].angle_set)||(ANGLE_MAX < -(chassis_move.motor_chassis[GL_Steer_flag_1].angle_set)))
		chassis_move.motor_chassis[GL_Steer_flag_1].angle_set = 0;

}


void CAN_Send_Tx_Fun()
{
			for(int i=0; i<Streer_GL_num;i++)
		{
			send_chassis_turnLoR_GL(GL_TxID_Streer_1+i*16,chassis_move.motor_chassis[GL_Steer_flag_1+i].angel_flag,chassis_move.motor_chassis[GL_Steer_flag_1+i].angle_set);
			osDelay(5);	
		}
			
		for(int i=0; i<Drive_AJN_num;i++)
		{
			send_chassis_QD_AJN(AJN_TxID_Motor_1+i,chassis_move.motor_chassis[AJN_Motor_flag_1+i].speed_flag,chassis_move.motor_chassis[AJN_Motor_flag_1+i].speed_gear,chassis_move.motor_chassis[AJN_Motor_flag_1+i].speed_set);
			osDelay(5);	
		}
			
		for(int i=0; i<Drive_HG_num;i++)
		{
			switch(i)
			{
				case 0: send_chassis_QD_HG(HG_TxID_Motor_1,chassis_move.motor_chassis[HG_Motor_flag_1].speed_flag,chassis_move.motor_chassis[HG_Motor_flag_1].speed_gear,chassis_move.motor_chassis[HG_Motor_flag_1].speed_set);
								break;
				case 1: send_chassis_QD_HG(HG_TxID_Motor_2,chassis_move.motor_chassis[HG_Motor_flag_2].speed_flag,chassis_move.motor_chassis[HG_Motor_flag_2].speed_gear,chassis_move.motor_chassis[HG_Motor_flag_2].speed_set);
								break;			
			}
			osDelay(5);	
		}
	

		send_chassis_ZD(chassis_move.motor_chassis[GL_Brake_flag].brak_set);
		osDelay(5);
			
		send_chassis_fb_0();	
    osDelay(5);
		
		send_chassis_fb_1();	
    osDelay(5);
		
		send_chassis_fb_2();	
    osDelay(5);
			
		static uint8_t uuid = 0;
		if(uuid++ > 50)
		{
			send_UUID();
		  osDelay(5);
			uuid = 0;
		}

}

void SW_Fun(const uint8_t SW_num)
{
	if((SW_num & 0x01) == 0x01)
	{
		HAL_GPIO_WritePin(GPIOC, MOS12V_1_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, MOS12V_1_Pin, GPIO_PIN_RESET);		
	}
	
	if((SW_num & 0x02) == 0x02)
	{
		HAL_GPIO_WritePin(GPIOC, MOS12V_2_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, MOS12V_2_Pin, GPIO_PIN_RESET);		
	}
	if((SW_num & 0x04) == 0x04)
	{
		HAL_GPIO_WritePin(GPIOC, MOS12V_3_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, MOS12V_3_Pin, GPIO_PIN_RESET);		
	}
	
	if((SW_num & 0x08) == 0x08)
	{
		HAL_GPIO_WritePin(GPIOC, MOS12V_4_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, MOS12V_4_Pin, GPIO_PIN_RESET);		
	}
	if((SW_num & 0x10) == 0x10)
	{
		HAL_GPIO_WritePin(GPIOC, MOS12V_5_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, MOS12V_5_Pin, GPIO_PIN_RESET);		
	}
	if((SW_num & 0x20) == 0x20)
	{
		HAL_GPIO_WritePin(GPIOC, MOS12V_6_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, MOS12V_6_Pin, GPIO_PIN_RESET);		
	}

}
