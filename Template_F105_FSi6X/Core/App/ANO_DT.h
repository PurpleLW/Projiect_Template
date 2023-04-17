#ifndef _ANO_DT_H_
#define _ANO_DT_H_

#include "main.h"


#define BYTE0(dwTemp)   (*(char *)(&dwTemp))
#define BYTE1(dwTemp)   (*((char *)(&dwTemp)+1))
#define BYTE2(dwTemp)   (*((char *)(&dwTemp)+2))
#define BYTE3(dwTemp)   (*((char *)(&dwTemp)+3))	

typedef __packed struct
{
        __packed struct
        {
                int16_t throttle;
                int16_t steering;
								int16_t ROL;
								int16_t PIT;
								int16_t aux1;
								int16_t aux2;
								int16_t aux3;
								int16_t aux4;
								int16_t var[2];
        } rc;
        __packed struct
        {
                int16_t speed;
                uint8_t gear;
                int16_t steering;
                int8_t brake;
                uint16_t sw;
								
					
        } host_c_down;
				 __packed struct
        {
                int16_t rpm;
                int16_t steering;
                int16_t speed;
								int16_t angle;
                uint8_t gear;
                int8_t brake;
								uint8_t Rover;
								uint8_t mode;
					
        }vehicle_status;
        __packed struct
        {
                int16_t Dmotor_current[2];
								int16_t Dmotor_tem[2];
								int16_t Smotor_current[2];
								int16_t Smotor_tem[2];
        }Sensor;
				 __packed struct
        {
								int16_t rpm[2];
								int16_t steering[2];
								int8_t brake;
								uint8_t gear[2];
								uint8_t sw[4];
        }VCOut;

} host_computer_debugg;

void ANODT_Send40(host_computer_debugg* hc_debugg); 

void ANODT_SendF1(host_computer_debugg* hc_debugg);  //小端模式  
  //小端模式
void ANODT_SendF2(host_computer_debugg* hc_debugg);  //小端模式

void ANODT_SendF3(host_computer_debugg* hc_debugg);  //小端模式

void ANODT_SendF4(host_computer_debugg* hc_debugg);  //小端模式

void ANODT_SendF5(host_computer_debugg* hc_debugg);  //小端模式


#endif
