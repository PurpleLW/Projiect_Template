#ifndef _FIRMWARE_H_
#define _FIRMWARE_H_

#include "main.h"


typedef  struct
{
    struct
    {
        uint8_t version[24];
    } Basic_infor;
    struct
    {
        uint8_t can1Baud;
        uint8_t can2Baud;
        uint16_t UCCPort;
        uint32_t ReceiveID;
        uint32_t OutputID1;
        uint32_t OutputID2;
        uint32_t OutputID3;
			
    } can;

    struct 
    {
        struct 
        {
            uint8_t MDrive1 : 1; 
            uint8_t MDrive2 : 1; 
            uint8_t MDrive3 : 1; 
            uint8_t MDrive4 : 1; 
            uint8_t MSteering1 : 1; 
            uint8_t MSteering2 : 1; 
            uint8_t Brake : 1; 
            uint8_t Other : 1; 
        } ChassisCom;

        struct 
        {
            uint8_t MDrive1 : 1; 
            uint8_t MDrive2 : 1; 
            uint8_t MDrive3 : 1; 
            uint8_t MDrive4 : 1; 
            uint8_t MSteering1 : 1; 
            uint8_t MSteering2 : 1; 
            uint8_t Brake : 1; 
            uint8_t Other : 1; 
        } ChassisEnable;
       
        uint8_t  ModelMDrive1;
        uint8_t  ModelMDrive2;
        uint8_t  ModelMDrive3;
        uint8_t  ModelMDrive4;
        uint8_t  ModelMSteering1 ; 
        uint8_t  ModelMSteering2 ; 
        uint8_t  ModelBrake ; 
        uint8_t  ModelOther ; 
       /* data */
    }Chassis;
    
    struct 
    {
        /* data */
        uint16_t RCProtocol;
        uint16_t Usart2Baud;
        uint16_t Usart3Baud;
        uint16_t Usart4Baud;
        uint16_t DebugInfo;

    }UART;
    
    uint16_t RpmMax;
    uint16_t AngleMax;
    uint16_t PressureMax;
    
} Parameter;


#endif  // _FIRMWARE_H_
