/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       Template.c/h
  * @brief      RM referee system data solve. RM����ϵͳ���ݴ���
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Nov-11-2019     RM              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "main.h"

#define PWM1_flag  0  // PA0
#define PWM2_flag  0  // PA1
#define PWM3_flag  0  // PA6
#define PWM4_flag  0  // PA7

#define CAN1_flag  1
#define CAN2_flag  1
	
<<<<<<< HEAD
#define UART2_Debugg_flag  0  //Ĭ�����������Ϣ
=======
#define UART2_Debugg_flag  1  //Ĭ�����������Ϣ
>>>>>>> e037807c5270ce72380d7a6187a961266f84d5ff
#if UART2_Debugg_flag == 0
	#define UART2_flag     1 //��ͨ����ʹ��
#endif

// �Ƿ��RS485 ����
#define UART3_RS485_flag  0

#define SWITCH_1_flag   1	  // 12V�������1  0Ϊ����� ��1Ϊ���12v
#define SWITCH_2_flag		1	  // 12V�������2
#define SWITCH_3_flag		1	 	// 12V�������3
#define SWITCH_4_flag   1	  // 12V�������4
#define SWITCH_5_flag   0	  // 12V�������5
#define SWITCH_6_flag 	0	  // 12V�������6

#define CAN1_BAUD   8  // CAN������   16:250   8:500  4:1000 
#define CAN2_BAUD   8  // CAN������   16:250   8:500  4:1000 

#define UART2_Baud   9600   // ���ڲ�����
#define UART3_Baud   115200   // ���ڲ�����

//***********************ң��������ֵ*****************************
#define RPM_MAX   6000     // ������ת��
#define ANGLE_MAX   720     // ������Ƕ�
#define BRAKE_MAX   120     // �������ƶ�ѹ��


#define _IBUS 1 //ң��ʹ�õ�ͨѶЭ�飺����ΪIBUS ��ע��Ϊ SBUSЭ��


//***********************DEBUGG************************************
#define RC_DEBUGG 0  // ң����Ϣ
#define PC_DEBUGG 0  //  ��λ����Ϣ

//*************************chaiss*************************************
#define Streer_GL_num 1
#define Drive_AJN_num 1
#define Drive_HG_num 0

#endif

