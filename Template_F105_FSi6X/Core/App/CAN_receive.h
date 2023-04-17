/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       can_receive.c/h
  * @brief      there is CAN interrupt function  to receive motor data,
  *             and CAN send function to send motor current to control motor.
  *             ������CAN�жϽ��պ��������յ������,CAN���ͺ������͵���������Ƶ��.
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

#ifndef CAN_RECEIVE_H
#define CAN_RECEIVE_H

#include "main.h"

#define CHASSIS_CAN hcan1
#define GIMBAL_CAN hcan2

/* CAN send and receive ID */
typedef enum
{
    AJN_Motor_1 = 0x1B0,
	  AJN_Motor_2 = 0x1B1,
		AJN_Motor_3 = 0x1B2,

    GL_Steer_1 = 0x18F,
	  GL_Steer_2 = 0x19F,
    GL_Brake = 0x289,
	
		Call_Back = 0x201,
	
	  AJN_TxID_Motor_1 = 0x1A0,
		AJN_TxID_Motor_2 = 0x1A1,
	 	AJN_TxID_Motor_3 = 0x1A2,

	
		GL_TxID_Streer_1 = 0x314,
		GL_TxID_Streer_2 = 0x324,

		HG_TxID_Motor_1 = 0x0CF89A27,
		HG_TxID_Motor_2 = 0x0CF88627,
	
		HG_Motor_ID_1 = 0x10F8109A,

	
} can_msg_id_e;

//rm motor data
typedef struct
{
    int16_t speed_rpm;
    int16_t current;
    uint8_t temperate;
    int16_t last_current;
		uint16_t err;     // �����־λ
		uint8_t state;   	//״̬
	  int16_t angle;
		uint8_t braking_force;
		uint8_t gear; 

} motor_measure_t;

typedef enum
{
    AJN_Motor_flag_1 = 0,  // ������������ 1
	  AJN_Motor_flag_2,      // ������������ 2
    AJN_Motor_flag_3,			 // ������������ 3
	  GL_Steer_flag_1,       // ��·ת��1
    GL_Steer_flag_2,       // ��·ת��2
		GL_Brake_flag,         // ��·�ƶ�
		HG_Motor_flag_1,			 // ���������� 1
		HG_Motor_flag_2,       // ���������� 2
		HG_Motor_flag_3,       // ���������� 3
	
	  NAME_LIST_LENGHT,
	
} Name_chaiss;


  
/**
  * @brief          Return to the data pointer of Angenen drive motor 1
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          ���ذ������������1 ����ָ��
  * @param[in]      none
  * @retval         �������ָ��
  */
extern const motor_measure_t *get_drive_motor_1_measure_point(void);

/**
  * @brief          Return to the data pointer of Angenen drive motor 2
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          ���ذ������������2 ����ָ��
  * @param[in]      none
  * @retval         �������ָ��
  */
extern const motor_measure_t *get_drive_motor_2_measure_point(void);

/**
  * @brief          Return to China highway steering motor 1 data pointer
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          �����й���·ת����1����ָ��
  * @param[in]      none
  * @retval         �������ָ��
  */
extern const motor_measure_t *get_steering_motor_1_measure_point(void);

/**
  * @brief          Return to China highway steering motor 2 data pointer
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          �����й���·ת����2����ָ��
  * @param[in]      none
  * @retval         �������ָ��
  */
extern const motor_measure_t *get_steering_motor_2_measure_point(void);

/**
  * @brief          Returns the data pointer of the Chinese highway brake motor
  * @param[in]      none
  * @retval         motor data point
  */
/**
  * @brief          �����й���·�ƶ��������ָ��
  * @param[in]      none
  * @retval         �������ָ��
  */
extern const motor_measure_t *get_brake_motor_measure_point(void);



extern void send_chassis_turnLoR_GL(uint16_t CAN_ID,uint8_t flag,int16_t jd)
;
extern void send_chassis_ZD(uint8_t zd);

extern void send_chassis_QD_AJN(uint16_t CAN_ID,uint8_t flag,uint8_t swich_flag,uint16_t ZhuangSu);

extern void send_chassis_QD_HG(uint32_t CAN_ID,uint8_t flag,uint8_t swich_flag,uint16_t ZhuangSu);

extern void send_chassis_fb_0(void);

extern void send_chassis_fb_1(void);

extern void send_chassis_fb_2(void);

extern void send_UUID(void);

#endif
