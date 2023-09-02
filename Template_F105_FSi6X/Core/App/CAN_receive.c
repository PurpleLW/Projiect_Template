/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       can_receive.c/h
  * @brief      there is CAN interrupt function  to receive motor data,
  *             and CAN send function to send motor current to control motor.
  *             这里是CAN中断接收函数，接收电机数据,CAN发送函数发送电机电流控制电机.
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. done
  *  V1.1.0     Nov-11-2019     RM              1. support hal lib
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */

#include "CAN_receive.h"
#include "cmsis_os.h"
#include "main.h"
#include "detect_task.h"
#include "remote_control.h"
#include "chassis_task.h"
#include "ANO_DT.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

/*
motor data,  电机数据, 0:驱动电机,安捷能 1:驱动电机,安捷能  2:转向电机 公路 3:转向电机 公路 4:制动电机 公路*/

static motor_measure_t motor_chassis[NAME_LIST_LENGHT];
//motor data read
extern RC_ctrl_t rc_ctrl;;
extern chassis_move_t chassis_move;

static CAN_TxHeaderTypeDef  chassis_tx_message;
static uint8_t              chassis_can_send_data[8];

static uint8_t Streer_Rx[8] = {0};
/**
  * @brief          hal CAN fifo call back, receive motor data
  * @param[in]      hcan, the point to CAN handle
  * @retval         none
  */
/**
  * @brief          hal库CAN回调函数,接收电机数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8]={0};

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

	if(hcan == &hcan1)
	{
		switch (rx_header.StdId)
    {
			case AJN_Motor_1:
			{	
				motor_chassis[AJN_Motor_flag_1].speed_rpm = (rx_data[0]|((int16_t)rx_data[1]<<8));
				
				if(motor_chassis[AJN_Motor_flag_1].speed_rpm >= 10000)
					motor_chassis[AJN_Motor_flag_1].speed_rpm -= 10000;
				else 
					motor_chassis[AJN_Motor_flag_1].speed_rpm = 10000 - motor_chassis[AJN_Motor_flag_1].speed_rpm;
				
			  motor_chassis[AJN_Motor_flag_1].last_current = motor_chassis[0].current;
				motor_chassis[AJN_Motor_flag_1].current = (rx_data[2]|((int16_t)rx_data[3]<<8));
				motor_chassis[AJN_Motor_flag_1].err = rx_data[4];
				motor_chassis[AJN_Motor_flag_1].temperate = rx_data[6]-41;
				motor_chassis[AJN_Motor_flag_1].state = rx_data[7];
				       detect_hook(CHASSIS_MOTOR1_Dri_TOE);

				break;
			}
			case AJN_Motor_2:
			{	
				motor_chassis[AJN_Motor_flag_2].speed_rpm = (rx_data[0]|((int16_t)rx_data[1]<<8));
				
				if(motor_chassis[AJN_Motor_flag_2].speed_rpm >= 10000)
					motor_chassis[AJN_Motor_flag_2].speed_rpm -= 10000;
				else 
					motor_chassis[AJN_Motor_flag_2].speed_rpm = 10000 - motor_chassis[AJN_Motor_flag_1].speed_rpm;

			  motor_chassis[AJN_Motor_flag_2].last_current = motor_chassis[1].current;
				motor_chassis[AJN_Motor_flag_2].current = (rx_data[2]|((int16_t)rx_data[3]<<8));
				motor_chassis[AJN_Motor_flag_2].err = rx_data[4];
				motor_chassis[AJN_Motor_flag_2].temperate = rx_data[6]-41;
				motor_chassis[AJN_Motor_flag_2].state = rx_data[7];
				detect_hook(CHASSIS_MOTOR2_Dri_TOE);

				break;
			}
			case AJN_Motor_3:
			{	
				motor_chassis[AJN_Motor_flag_3].speed_rpm = (rx_data[0]|((int16_t)rx_data[1]<<8));
			  motor_chassis[AJN_Motor_flag_3].last_current = motor_chassis[1].current;
				motor_chassis[AJN_Motor_flag_3].current = (rx_data[2]|((int16_t)rx_data[3]<<8));
				motor_chassis[AJN_Motor_flag_3].err = rx_data[4];
				motor_chassis[AJN_Motor_flag_3].temperate = rx_data[6]-41;
				motor_chassis[AJN_Motor_flag_3].state = rx_data[7];
				break;
			}

			case GL_Steer_1:
			{	
				motor_chassis[GL_Steer_flag_1].state = rx_data[0];				
				motor_chassis[GL_Steer_flag_1].angle = (rx_data[2]|((int16_t)rx_data[1]<<8));
			  motor_chassis[GL_Steer_flag_1].last_current = motor_chassis[2].current;
				motor_chassis[GL_Steer_flag_1].current = (rx_data[4]|((int16_t)rx_data[3]<<8));
				motor_chassis[GL_Steer_flag_1].temperate = rx_data[6];
				detect_hook(CHASSIS_MOTOR1_Str_TOE);

				break;
			}
			case GL_Steer_2:
			{	
				motor_chassis[GL_Steer_flag_2].state = rx_data[0];				
				motor_chassis[GL_Steer_flag_2].angle = (rx_data[2]|((int16_t)rx_data[1]<<8));
			  motor_chassis[GL_Steer_flag_2].last_current = motor_chassis[3].current;
				motor_chassis[GL_Steer_flag_2].current = (rx_data[4]|((int16_t)rx_data[3]<<8));
				motor_chassis[GL_Steer_flag_2].temperate = rx_data[6];
				detect_hook(CHASSIS_MOTOR2_Str_TOE);

				break;
			}
			case GL_Brake:
			{	
				motor_chassis[GL_Brake_flag].braking_force = rx_data[0];
				motor_chassis[GL_Brake_flag].state = (rx_data[1]>>4)&0xFF;
				motor_chassis[GL_Brake_flag].err =  ((uint16_t)rx_data[4]<<8)|(rx_data[5]);
				detect_hook(CHASSIS_MOTOR1_Bra_TOE);

				break;
			}
			case 0x700:
			{
				Streer_Rx[0] = rx_data[0];
				Streer_Rx[1] = rx_data[1];
				Streer_Rx[2] = rx_data[2];
				Streer_Rx[3] = rx_data[3];
				Streer_Rx[4] = rx_data[4];
				Streer_Rx[5] = rx_data[5];
				Streer_Rx[6] = rx_data[6];
				Streer_Rx[7] = rx_data[7];

				break; 
			}

        default:
        {
					 
            break;
        }
	}
		
	switch (rx_header.ExtId)
  {
				case HG_Motor_ID_1:
				{	
					motor_chassis[HG_Motor_flag_1].gear  = rx_data[0]&0x03;
					motor_chassis[HG_Motor_flag_1].state = rx_data[1]&0x03;
					motor_chassis[HG_Motor_flag_1].err = rx_data[3];
					motor_chassis[HG_Motor_flag_1].speed_rpm = (rx_data[4]|((int16_t)rx_data[5]<<8));

					break;
				}

			  default:
        {
          break;
        }
	}

}
	else if(hcan == &hcan2)
	{
		switch (rx_header.StdId)
    {
				case Call_Back:
				{	
					rc_ctrl.ipc.flag =  rx_data[0];
					rc_ctrl.ipc.barke = rx_data[1];
					rc_ctrl.ipc.gear = rx_data[2];
					rc_ctrl.ipc.Switch = rx_data[3];
					rc_ctrl.ipc.rpm = (rx_data[5]|((int16_t)rx_data[4]<<8));
					rc_ctrl.ipc.angle = (rx_data[7]|((int16_t)rx_data[6]<<8));
					
					rc_ctrl.ipc.time = xTaskGetTickCount();
					break;
				}
        default:
        {
            break;
        }
		}
			switch (rx_header.ExtId)
    {
			        default:
        {
					 
            break;
        }
	}

	}
    
    
}
<<<<<<< HEAD
/*
void RC_restart(uint16_t dma_buf_num)
{
    __HAL_UART_DISABLE(&huart1);
    __HAL_DMA_DISABLE(&hdma_usart1_rx);

    hdma_usart1_rx.Instance->CNDTR = dma_buf_num;

    __HAL_DMA_ENABLE(&hdma_usart1_rx);
    __HAL_UART_ENABLE(&huart1);

}

*/
=======

>>>>>>> e037807c5270ce72380d7a6187a961266f84d5ff
//********************LLLWWW***********************

void send_UUID()
{
<<<<<<< HEAD

=======
>>>>>>> e037807c5270ce72380d7a6187a961266f84d5ff
		uint32_t send_mail_box;
		chassis_tx_message.StdId = 0x099;
		chassis_tx_message.IDE = CAN_ID_STD;
		chassis_tx_message.RTR = CAN_RTR_DATA;
		chassis_tx_message.DLC = 0x08;
		 
		chassis_can_send_data[0] = 0x18;
		chassis_can_send_data[1] = 0x16;
		chassis_can_send_data[2] = 0x32;
		chassis_can_send_data[3] = 0x6A;
		chassis_can_send_data[4] = 0xDA; 
		chassis_can_send_data[5] = 0x20;
		chassis_can_send_data[6] = 0x00;
		chassis_can_send_data[7] = 0x00;
    HAL_CAN_AddTxMessage(&GIMBAL_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
	
}


void send_chassis_turnLoR_GL(uint16_t CAN_ID,uint8_t flag,int16_t jd)
{
	
    uint32_t send_mail_box;
    chassis_tx_message.StdId = CAN_ID;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = flag;
    chassis_can_send_data[1] = (uint8_t)((jd>>8) & 0xff);
    chassis_can_send_data[2] = (uint8_t)(jd & 0xff);
    chassis_can_send_data[3] = 0;
    chassis_can_send_data[4] = 0;
    chassis_can_send_data[5] = 0;
    chassis_can_send_data[6] = 0;
    chassis_can_send_data[7] = 0;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}



void send_chassis_ZD(uint8_t zd)
{
	
    uint32_t send_mail_box;
    chassis_tx_message.StdId = 0x364;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = zd;
    chassis_can_send_data[1] = 0;
    chassis_can_send_data[2] = 0;
    chassis_can_send_data[3] = 0;
    chassis_can_send_data[4] = 0;
    chassis_can_send_data[5] = 0;
    chassis_can_send_data[6] = 0;
    chassis_can_send_data[7] = 0;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
	
}

void send_chassis_QD_AJN(uint16_t CAN_ID,uint8_t flag,uint8_t swich_flag,uint16_t ZhuangSu)
{
	
    uint32_t send_mail_box;
    chassis_tx_message.StdId = CAN_ID;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = flag;
    chassis_can_send_data[1] = swich_flag;
    chassis_can_send_data[2] = (uint8_t)(ZhuangSu & 0xff);
    chassis_can_send_data[3] = (uint8_t)((ZhuangSu >> 8) &0xff);
    chassis_can_send_data[4] = 0;
    chassis_can_send_data[5] = 0;
    chassis_can_send_data[6] = 0x89;
    chassis_can_send_data[7] = 0x78;

    HAL_CAN_AddTxMessage(&CHASSIS_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);

}

void send_chassis_QD_HG(uint32_t CAN_ID,uint8_t flag,uint8_t swich_flag,uint16_t ZhuangSu)
{
	
    uint32_t send_mail_box;
    chassis_tx_message.ExtId = CAN_ID;
    chassis_tx_message.IDE = CAN_ID_EXT;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = flag;
    chassis_can_send_data[1] = swich_flag;
    chassis_can_send_data[2] = (uint8_t)(ZhuangSu & 0xff);
    chassis_can_send_data[3] = (uint8_t)((ZhuangSu >> 8) &0xff);
    chassis_can_send_data[4] = 0;
    chassis_can_send_data[5] = 0;
    chassis_can_send_data[6] = 0x89;
    chassis_can_send_data[7] = 0x78;

    HAL_CAN_AddTxMessage(&GIMBAL_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}

void send_chassis_fb_0()
{
	
    uint32_t send_mail_box;
    chassis_tx_message.StdId = 0x211;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
	
	  chassis_can_send_data[0] = BYTE0(chassis_move.speed);
	  chassis_can_send_data[1] = BYTE1(chassis_move.speed);
    chassis_can_send_data[2] = (uint8_t)(motor_chassis[AJN_Motor_flag_1].speed_rpm & 0xFF);
    chassis_can_send_data[3] = (uint8_t)((motor_chassis[AJN_Motor_flag_1].speed_rpm>>8) & 0xFF);
    chassis_can_send_data[4] = (uint8_t)(motor_chassis[GL_Steer_flag_1].angle & 0xFF);
    chassis_can_send_data[5] = (uint8_t)((motor_chassis[GL_Steer_flag_1].angle>>8) & 0xFF);
    chassis_can_send_data[6] = 	motor_chassis[4].braking_force;
		chassis_can_send_data[7] = chassis_move.motor_chassis[AJN_Motor_flag_1].speed_gear;

    HAL_CAN_AddTxMessage(&GIMBAL_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}

void send_chassis_fb_1()
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = 0x212;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
	
	  chassis_can_send_data[0] = chassis_move.SW_num;
	  chassis_can_send_data[1] = chassis_move.chaiss_MODE;
    chassis_can_send_data[2] = 0;
	  
	if(toe_is_error(CHASSIS_MOTOR1_Dri_TOE))
	 chassis_can_send_data[2] |= 0x01;
	else
	 chassis_can_send_data[2] &= 0xFE;
	
	if(toe_is_error(CHASSIS_MOTOR1_Bra_TOE))
	 chassis_can_send_data[2] |= 0x02;
	else
	 chassis_can_send_data[2] &= 0xFD;

	if(toe_is_error(CHASSIS_MOTOR1_Str_TOE))
	 chassis_can_send_data[2] |= 0x04;
	else
	 chassis_can_send_data[2] &= 0xFB;

    chassis_can_send_data[3] = motor_chassis[AJN_Motor_flag_1].err;
    chassis_can_send_data[4] = BYTE1(motor_chassis[GL_Brake_flag].err);
    chassis_can_send_data[5] = BYTE0(motor_chassis[GL_Brake_flag].err);
    chassis_can_send_data[6] = 0;
		chassis_can_send_data[7] = 0;

    HAL_CAN_AddTxMessage(&GIMBAL_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}
 
void send_chassis_fb_2()
{
    uint32_t send_mail_box;
    chassis_tx_message.StdId = 0x700;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
	
	  chassis_can_send_data[0] = Streer_Rx[0];
	  chassis_can_send_data[1] = Streer_Rx[1];
    chassis_can_send_data[2] = Streer_Rx[2];
    chassis_can_send_data[3] = Streer_Rx[3];
    chassis_can_send_data[4] = Streer_Rx[4];
    chassis_can_send_data[5] = Streer_Rx[5];
    chassis_can_send_data[6] = Streer_Rx[6];
		chassis_can_send_data[7] = Streer_Rx[7];

    HAL_CAN_AddTxMessage(&GIMBAL_CAN, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}

//***************************END************************


/**
  * @brief          Return to the data pointer of Angenen drive motor 1
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          返回安捷能驱动电机1 数据指针
  * @param[in]      none
  * @retval         电机数据指针
  */
const motor_measure_t *get_drive_motor_1_measure_point(void)
{
    return &motor_chassis[AJN_Motor_flag_1];
}


/**
  * @brief          Return to the data pointer of Angenen drive motor 2
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          返回安捷能驱动电机2 数据指针
  * @param[in]      none
  * @retval         电机数据指针
  */
const motor_measure_t *get_drive_motor_2_measure_point(void)
{
    return &motor_chassis[AJN_Motor_flag_2];
}


/**
  * @brief          Return to China highway steering motor 1 data pointer
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          返回中国公路转向电机1数据指针
  * @param[in]      none
  * @retval         电机数据指针
  */
const motor_measure_t *get_steering_motor_1_measure_point(void)
{
    return &motor_chassis[GL_Steer_flag_1];
}

/**
  * @brief          Return to China highway steering motor 2 data pointer
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          返回中国公路转向电机2数据指针
  * @param[in]      none
  * @retval         电机数据指针
  */
const motor_measure_t *get_steering_motor_2_measure_point(void)
{
    return &motor_chassis[GL_Steer_flag_2];
}

/**
  * @brief          Returns the data pointer of the Chinese highway brake motor
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          返回中国公路制动电机数据指针
  * @param[in]      none
  * @retval         电机数据指针
  */
const motor_measure_t *get_brake_motor_measure_point(void)
{
    return &motor_chassis[GL_Brake_flag];
}
