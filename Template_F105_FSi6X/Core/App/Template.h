/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       Template.c/h
  * @brief      RM referee system data solve. RM裁判系统数据处理
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
#define UART2_Debugg_flag  0  //默认输出调试信息
=======
#define UART2_Debugg_flag  1  //默认输出调试信息
>>>>>>> e037807c5270ce72380d7a6187a961266f84d5ff
#if UART2_Debugg_flag == 0
	#define UART2_flag     1 //普通串口使用
#endif

// 是否打开RS485 功能
#define UART3_RS485_flag  0

#define SWITCH_1_flag   1	  // 12V输出开关1  0为不输出 ，1为输出12v
#define SWITCH_2_flag		1	  // 12V输出开关2
#define SWITCH_3_flag		1	 	// 12V输出开关3
#define SWITCH_4_flag   1	  // 12V输出开关4
#define SWITCH_5_flag   0	  // 12V输出开关5
#define SWITCH_6_flag 	0	  // 12V输出开关6

#define CAN1_BAUD   8  // CAN波特率   16:250   8:500  4:1000 
#define CAN2_BAUD   8  // CAN波特率   16:250   8:500  4:1000 

#define UART2_Baud   9600   // 串口波特率
#define UART3_Baud   115200   // 串口波特率

//***********************遥控输出最大值*****************************
#define RPM_MAX   6000     // 输出最大转速
#define ANGLE_MAX   720     // 输出最大角度
#define BRAKE_MAX   120     // 输出最大制动压力


#define _IBUS 1 //遥控使用的通讯协议：定义为IBUS ，注释为 SBUS协议


//***********************DEBUGG************************************
#define RC_DEBUGG 0  // 遥控信息
#define PC_DEBUGG 0  //  上位机信息

//*************************chaiss*************************************
#define Streer_GL_num 1
#define Drive_AJN_num 1
#define Drive_HG_num 0

#endif

