/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       remote_control.c/h
  * @brief      ң��������ң������ͨ������SBUS��Э�鴫�䣬����DMA���䷽ʽ��ԼCPU
  *             ��Դ�����ô��ڿ����ж�������������ͬʱ�ṩһЩ��������DMA������
  *             �ķ�ʽ��֤�Ȳ�ε��ȶ��ԡ�
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. done
  *  V1.0.0     Nov-11-2019     RM              1. support development board tpye c
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2022 ������****************************
  */

#include "HMI_task.h"
#include "cmsis_os.h"
#include "Flash.h"
#include "firmware.h"
#include "string.h"
static uint8_t TEXT_Buffer[] = {"BB_VCU_F105_1.2"};
static uint8_t datatemp[1024];
uint8_t *p = datatemp;
 
uint8_t ijkjjj = 0;

Parameter paraDemo;
uint16_t size_para;
 void HMI_task(void const * argument)
 {
	 
  for(;;)
  {
		
//		uint16_t size = sizeof(TEXT_Buffer);
		paraDemo.RpmMax = 5000;
		paraDemo.AngleMax = 500;
		paraDemo.PressureMax = 100;
		
		paraDemo.can.can1Baud = 8;
		paraDemo.can.can2Baud = 16;
		paraDemo.can.ReceiveID = 0x201;
		paraDemo.Chassis.ChassisCom.MDrive2 = 1;
		paraDemo.Chassis.ChassisEnable.MDrive2 = 1;
		paraDemo.UART.RCProtocol = 1;
		paraDemo.Chassis.ModelOther = 2;
		size_para = sizeof(paraDemo);
		Flash_Write((uint32_t *)&paraDemo,(size_para + 3)/4);
		
			Flash_Read((uint32_t *)datatemp,(size_para + 3)/4);
			vTaskDelete(NULL);
  }
	 
 }

