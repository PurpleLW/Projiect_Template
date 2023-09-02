#ifndef _FLASH_H_
#define _FLASH_H_
#include "main.h"
#include "stm32f1xx_hal_flash_ex.h" 
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
 
 
<<<<<<< HEAD
#define STM32_FLASH_SIZE 	128 	 	//所选STM32的FLASH容量大小(单位为K)
=======
#define STM32_FLASH_SIZE 	64 	 	//所选STM32的FLASH容量大小(单位为K)
>>>>>>> e037807c5270ce72380d7a6187a961266f84d5ff
    #if     STM32_FLASH_SIZE < 256      //设置扇区大小
    #define STM_SECTOR_SIZE     1024    //1K字节
    #else 
    #define STM_SECTOR_SIZE	    2048    //2K字节
#endif
 
#define STM32_FLASH_BASE            0x08000000 		//STM32 FLASH的起始地址
<<<<<<< HEAD
#define FLASH_USER_START_ADDR   ( STM32_FLASH_BASE + STM_SECTOR_SIZE * 126 ) //写Flash的地址，这里从第62页开始
#define FLASH_USER_END_ADDR     ( STM32_FLASH_BASE + STM_SECTOR_SIZE * 127 ) //写Flash的地址，这里以第64页结束
=======
#define FLASH_USER_START_ADDR   ( STM32_FLASH_BASE + STM_SECTOR_SIZE * 62 ) //写Flash的地址，这里从第62页开始
#define FLASH_USER_END_ADDR     ( STM32_FLASH_BASE + STM_SECTOR_SIZE * 64 ) //写Flash的地址，这里以第64页结束
>>>>>>> e037807c5270ce72380d7a6187a961266f84d5ff
 
 
void Flash_Erase(void); 
void Flash_Write(u32 *pBuffer,u32 NumToWrite);
void Flash_Read(u32 *pBuffer,u32 NumToRead);
 
 
#endif



