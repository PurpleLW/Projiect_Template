/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       Debug_task.c/h
  * @brief      调试任务，主要通过uart2发送运行数据给上位机
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
#include "Debug_task.h"
#include "cmsis_os.h"
#include "ANO_DT.h"
#include "remote_control.h"
#include "Template.h"
#include "CAN_receive.h"
#include "chassis_task.h"


extern host_computer_debugg hc_debugg;
const RC_ctrl_t *chassis_RC;

const chassis_move_t *Chassis_move;

/**
  * @brief          Remote control data collection
  * @param[out]     hc_debugg:"host_computer_debugg"变量指针.
  * @retval         none
  */
/**
  * @brief          遥控器数据收集
  * @param[out]     hc_debugg:"host_computer_debugg"变量指针.
  * @retval         none
  */
static void RC_Debugg_fun(host_computer_debugg* hc_debugg);

/**
  * @brief          PC data collection
  * @param[out]     hc_debugg:"host_computer_debugg"变量指针.
  * @retval         none
  */
/**
  * @brief          上位机数据收集
  * @param[out]     hc_debugg:"host_computer_debugg"变量指针.
  * @retval         none
  */
static void PC_Debugg_fun(host_computer_debugg* hc_debugg);

/**
  * @brief          Vehicle status data collection
  * @param[out]     hc_debugg:"host_computer_debugg"变量指针.
  * @retval         none
  */
/**
  * @brief          整车状态数据收集
  * @param[out]     hc_debugg:"host_computer_debugg"变量指针.
  * @retval         none
  */
static void VehSta_Debugg_fun(host_computer_debugg* hc_debugg,const chassis_move_t* chassis_state_init);

/**
  * @brief          Control data in real time
  * @param[out]     hc_debugg:"host_computer_debugg"变量指针.
  * @retval         none
  */
/**
  * @brief          实时控制数据
  * @param[out]     hc_debugg:"host_computer_debugg"变量指针.
  * @retval         none
  */
static void RT_Debugg_fun(host_computer_debugg* hc_debugg);


uint32_t time;
uint32_t time1,time2;

/**
  * @brief          make the buzzer sound
  * @param[in]      num: the number of beeps 
  * @retval         none
  */
/**
  * @brief          使得蜂鸣器响
  * @param[in]      num:响声次数
  * @retval         none
  */
static void buzzer_warn_error(uint8_t num)
{
    static uint8_t show_num = 0;
    static uint8_t stop_num = 200;
    if(show_num == 0 && stop_num == 0)
    {
        show_num = num;
        stop_num = 200;
    }
    else if(show_num == 0)
    {
        stop_num--;
        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
    }
    else
    {
        static uint8_t tick = 0;
        tick++;
        if(tick < 50)
        {
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
        }
        else if(tick < 100)
        {
            HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
        }
        else
        {
            tick = 0;
            show_num--;
        }
    }
}

void Debug_task(void const * argument)
{
  /* USER CODE BEGIN Debug_task */
  /* Infinite loop */
	chassis_RC = get_remote_control_point();
  Chassis_move = get_chassis_move_point();
  for(;;)
  {
	#if UART2_Debugg_flag == 1
		RC_Debugg_fun(&hc_debugg);
		osDelay(20);
		PC_Debugg_fun(&hc_debugg);
		osDelay(20);
		VehSta_Debugg_fun(&hc_debugg,Chassis_move);
		osDelay(20);
		RT_Debugg_fun(&hc_debugg);
		osDelay(20);
	#endif
//		 osDelay(500);
//		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
//		osDelay(500);
//		
//		{  //软复位
//     __disable_irq();  // 关闭总中断		
//			for(uint32_t t=0;t<1000000;t++);
//			HAL_NVIC_SystemReset();//软复位
//		}
		static uint16_t ii = 1;
		buzzer_warn_error(ii);
//		ii = 0;
	
//		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_15);
    osDelay(10);
  }
  /* USER CODE END Debug_task */
}



/**
  * @brief          Remote control data collection
  * @param[out]     hc_debugg:"host_computer_debugg"变量指针.
  * @retval         none
  */
/**
  * @brief          遥控器数据收集
  * @param[out]     hc_debugg:"host_computer_debugg"变量指针.
  * @retval         none
  */
static void RC_Debugg_fun(host_computer_debugg* hc_debugg)
{
		hc_debugg->rc.throttle = chassis_RC->rc.ch[2];
		hc_debugg->rc.steering = chassis_RC->rc.ch[0];
		hc_debugg->rc.PIT = chassis_RC->rc.ch[1];
		hc_debugg->rc.ROL = chassis_RC->rc.ch[3];

		hc_debugg->rc.aux1 = chassis_RC->rc.s[0];
		hc_debugg->rc.aux2 = chassis_RC->rc.s[1];
		hc_debugg->rc.aux3 = chassis_RC->rc.s[2];
		hc_debugg->rc.aux4 = chassis_RC->rc.s[3];
		hc_debugg->rc.var[0] = chassis_RC->rc.var[0];
		hc_debugg->rc.var[1] = chassis_RC->rc.var[1];
		ANODT_Send40(hc_debugg);
}


/**
  * @brief          PC data collection
  * @param[out]     hc_debugg:"host_computer_debugg"变量指针.
  * @retval         none
  */
/**
  * @brief          上位机数据收集
  * @param[out]     hc_debugg:"host_computer_debugg"变量指针.
  * @retval         none
  */
static void PC_Debugg_fun(host_computer_debugg* hc_debugg)
{
		hc_debugg->host_c_down.speed = 0;
		hc_debugg->host_c_down.gear = 0;
		hc_debugg->host_c_down.steering = 0;
		hc_debugg->host_c_down.brake = 0;
		hc_debugg->host_c_down.sw = 0;
	
		ANODT_SendF2(hc_debugg);
}


/**
  * @brief          Vehicle status data collection
  * @param[out]     hc_debugg:"host_computer_debugg"变量指针.
  * @retval         none
  */
/**
  * @brief          整车状态数据收集
  * @param[out]     hc_debugg:"host_computer_debugg"变量指针.
  * @retval         none
  */
static void VehSta_Debugg_fun(host_computer_debugg* hc_debugg,const chassis_move_t* chassis_state_init)
{
		hc_debugg->vehicle_status.rpm = chassis_state_init->motor_chassis[0].chassis_motor_measure->speed_rpm;
		hc_debugg->vehicle_status.steering = chassis_state_init->motor_chassis[2].chassis_motor_measure->angle;
		hc_debugg->vehicle_status.speed = (int16_t)((float)hc_debugg->vehicle_status.rpm * 1.0f);
		hc_debugg->vehicle_status.angle = (int16_t)((float)hc_debugg->vehicle_status.steering * 1.0f);
	
		hc_debugg->vehicle_status.gear = hc_debugg->vehicle_status.rpm > 0 ? 0xAA : 0x55;
		hc_debugg->vehicle_status.brake = chassis_state_init->motor_chassis[4].chassis_motor_measure->braking_force;
		hc_debugg->vehicle_status.Rover = 0;
	  hc_debugg->vehicle_status.mode = 0;
		ANODT_SendF3(hc_debugg);
}


/**
  * @brief          Control data in real time
  * @param[out]     hc_debugg:"host_computer_debugg"变量指针.
  * @retval         none
  */
/**
  * @brief          实时控制数据
  * @param[out]     hc_debugg:"host_computer_debugg"变量指针.
  * @retval         none
  */
static void RT_Debugg_fun(host_computer_debugg* hc_debugg)
{
		hc_debugg->VCOut.rpm[0] = Chassis_move->motor_chassis[0].speed_set;
		hc_debugg->VCOut.rpm[1] = Chassis_move->motor_chassis[1].speed_set;
		hc_debugg->VCOut.gear[0] = Chassis_move->motor_chassis[0].speed_gear;
		hc_debugg->VCOut.gear[1] = Chassis_move->motor_chassis[1].speed_gear;

		hc_debugg->VCOut.steering[0] = Chassis_move->motor_chassis[2].angle_set;
		hc_debugg->VCOut.steering[1] = Chassis_move->motor_chassis[3].angle_set;
	
		hc_debugg->VCOut.brake = Chassis_move->motor_chassis[4].brak_set;

		hc_debugg->VCOut.sw[0] = chassis_RC->rc.s[0];
		hc_debugg->VCOut.sw[1] = chassis_RC->rc.s[1];
		hc_debugg->VCOut.sw[2] = chassis_RC->rc.s[2];
	
		ANODT_SendF1(hc_debugg);
}

