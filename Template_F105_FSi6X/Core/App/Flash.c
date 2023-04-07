/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       remote_control.c/h
  * @brief      遥控器处理，遥控器是通过类似SBUS的协议传输，利用DMA传输方式节约CPU
  *             资源，利用串口空闲中断来拉起处理函数，同时提供一些掉线重启DMA，串口
  *             的方式保证热插拔的稳定性。
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
  ****************************(C) COPYRIGHT 2022 康沃瑞****************************
  */


#include "Flash.h"
#include <stdio.h>

#include "usart.h"
 
static FLASH_EraseInitTypeDef EraseInitStruct;
u32 PAGEError = 0;
 /**********************************************************************************
  * 函数功能: 页擦除
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无 
  */
void Flash_Erase(void)
{  	
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;
    EraseInitStruct.NbPages     = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / STM_SECTOR_SIZE;
    
     if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)  
	 {
		 HAL_FLASH_Lock();  
//		 printf(" Error...1\r\n");
				uint8_t err[8] = {0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x01};
		  	HAL_UART_Transmit_DMA(&huart2,(uint8_t *)err,8);

         Error_Handler( );
	 }
}
 /**********************************************************************************
  * 函数功能: 数据写入
  * 输入参数: 写入数据缓存数组指针、写入数据数
  * 返 回 值: 无
  * 说    明：无 
  */    
void Flash_Write(u32 *pBuffer,u32  NumToWrite)
{
 
    u16  i=0;
    u32 Address = FLASH_USER_START_ADDR;
    HAL_FLASH_Unlock();	    //解锁
    Flash_Erase( );         //先擦除
                            //再写入
       uint8_t ok[8] = {0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0xAA};
		  	HAL_UART_Transmit_DMA(&huart2,(uint8_t *)ok,8);
     while ( (Address < FLASH_USER_END_ADDR) && (i<NumToWrite)  )    
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, pBuffer[i]) == HAL_OK)
        {
            Address = Address + 4;  //地址后移4个字节
            i++;
        }
        else
		{  
//			printf(" Error...2\r\n"); 
			uint8_t err[8] = {0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x02};
		  	HAL_UART_Transmit_DMA(&huart2,(uint8_t *)err,8);
            Error_Handler( );            
		}
    }
  
    HAL_FLASH_Lock();   //上锁
 
 
}
 
 /**********************************************************************************
  * 函数功能: 数据读取
  * 输入参数: 读取数据缓存数组指针、读出数据数
  * 返 回 值: 无
  * 说    明：无
  */
void Flash_Read(u32  *pBuffer,u32  NumToRead)
{
    u16  i=0;
    u32 Address = FLASH_USER_START_ADDR;
    
    while ( (Address < FLASH_USER_END_ADDR) && (i<NumToRead)  )
  {
    pBuffer[i++]= *(__IO u32 *)Address;  
    Address = Address + 4;   //地址后移4个字节
  }
  
}


