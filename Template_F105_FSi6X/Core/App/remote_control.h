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

#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H
#include "main.h"
#include "Template.h"

#define SBUS_RX_BUF_NUM 36u

#define RS485_LENGTH 5u

#ifdef _IBUS
	#define RC_FRAME_LENGTH 32u
#else  
	#define RC_FRAME_LENGTH 25u
#endif

/* ----------------------- RC Switch Definition_SBus----------------------------- */
#define RC_CH_VALUE_MIN         ((uint16_t)364)
#define RC_CH_VALUE_OFFSET      ((uint16_t)1024)
#define RC_CH_VALUE_MAX         ((uint16_t)1684)



/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP                ((uint16_t)1)
#define RC_SW_MID               ((uint16_t)3)
#define RC_SW_DOWN              ((uint16_t)2)
#define switch_is_down(s)       (s == RC_SW_DOWN)
#define switch_is_mid(s)        (s == RC_SW_MID)
#define switch_is_up(s)         (s == RC_SW_UP)



/* ----------------------- Data Struct ------------------------------------- */

typedef __packed struct
{
        __packed struct
        {
                int16_t ch[4];
                int16_t s[4];
								int16_t var[2];
        } rc;
        __packed struct
        {
                int16_t x;
                int16_t y;
                int16_t z;
                uint8_t press_l;
                uint8_t press_r;
        } mouse;
        __packed struct
        {
                uint16_t v;
        } key;
				__packed struct
        {
                int16_t rpm;
					      int16_t angle;
								uint8_t barke;
								uint8_t gear;
								uint8_t flag;
								uint8_t Switch;
								uint32_t	time;
        } ipc;

} RC_ctrl_t;

typedef __packed struct
{
        __packed struct
        {
                int16_t a[4];
                int16_t b[4];
								int16_t v[2];
        } rc;
        __packed struct
        {
                int16_t x;
                int16_t y;
                int16_t z;
                uint8_t press_l;
                uint8_t press_r;
        } mouse;

} RS485_t;


/* ----------------------- Internal Data ----------------------------------- */

void remote_control_init(void);
void RS485_Fun_init(void);
extern const RC_ctrl_t *get_remote_control_point(void);
void RS485_SendByte(char* SendData);

#endif

