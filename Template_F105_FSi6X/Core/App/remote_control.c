/**
  ****************************(C) COPYRIGHT 2022 康沃瑞****************************
  * @file       remote_control.c/h
  * @brief      遥控器处理，遥控器是通过类似SBUS的协议传输，利用DMA传输方式节约CPU
  *             资源，利用串口空闲中断来拉起处理函数，同时提供一些掉线重启DMA，串口
  *             的方式保证热插拔的稳定性。
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Aug-9-2022     CWR              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2022 康沃瑞****************************
  */

#include "remote_control.h"
#include "usart.h"
#include "cmsis_os.h"
#include "string.h"
#include "detect_task.h"

//遥控器出错数据上限
#define RC_CHANNAL_ERROR_VALUE 1800

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;

extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_rx;

//取正函数
static int16_t RC_abs(int16_t value);

/**
  * @brief          remote control protocol resolution
  * @param[in]      sbus_buf: raw data point
  * @param[out]     rc_ctrl: remote control data struct point
  * @retval         none
  */
/**
  * @brief          遥控器协议解析
  * @param[in]      sbus_buf: 原生数据指针
  * @param[out]     rc_ctrl: 遥控器数据指
  * @retval         none
  */
#ifdef _IBUS
static void ibus_to_rc(volatile const uint8_t *sbus_buf, RC_ctrl_t *rc_ctrl);
#else  
static void sbus_to_rc(volatile const uint8_t *sbus_buf, RC_ctrl_t *rc_ctrl);	
#endif	
static void rs485_r(volatile const uint8_t *sbus_buf, RS485_t* rs485_t);

//remote control data 
//遥控器控制变量
RC_ctrl_t rc_ctrl;

RS485_t rs485_t;
//接收原始数据，为18个字节，给了36个字节长度，防止DMA传输越界
static uint8_t sbus_rx_buf[SBUS_RX_BUF_NUM];

static uint8_t rs485_rx_buf[SBUS_RX_BUF_NUM];



void RC_Init(uint8_t *rx_buf, uint16_t dma_buf_num)
{
   
    SET_BIT(huart1.Instance->CR3, USART_CR3_DMAR);
	
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    __HAL_DMA_DISABLE(&hdma_usart1_rx);
	
   

    hdma_usart1_rx.Instance->CPAR = (uint32_t) & (USART1->DR);
    
    hdma_usart1_rx.Instance->CMAR = (uint32_t)(rx_buf);
    
    hdma_usart1_rx.Instance->CNDTR = dma_buf_num;

	
    __HAL_DMA_ENABLE(&hdma_usart1_rx);
}


void RS485_Init(uint8_t *rx_buf, uint16_t dma_buf_num)
{
   
    SET_BIT(huart3.Instance->CR3, USART_CR3_DMAR);
	
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
    __HAL_DMA_DISABLE(&hdma_usart3_rx);
	
   

    hdma_usart3_rx.Instance->CPAR = (uint32_t) & (USART3->DR);
    
    hdma_usart3_rx.Instance->CMAR = (uint32_t)(rx_buf);
    
    hdma_usart3_rx.Instance->CNDTR = dma_buf_num;

	
    __HAL_DMA_ENABLE(&hdma_usart3_rx);
}


void RC_unable(void)
{
    __HAL_UART_DISABLE(&huart1);
}
void RC_restart(uint16_t dma_buf_num)
{
    __HAL_UART_DISABLE(&huart1);
    __HAL_DMA_DISABLE(&hdma_usart1_rx);

    hdma_usart1_rx.Instance->CNDTR = dma_buf_num;

    __HAL_DMA_ENABLE(&hdma_usart1_rx);
    __HAL_UART_ENABLE(&huart1);

}



/**
  * @brief          remote control init
  * @param[in]      none
  * @retval         none
  */
/**
  * @brief          遥控器初始化
  * @param[in]      none
  * @retval         none
  */
void remote_control_init(void)
{
    RC_Init(sbus_rx_buf, SBUS_RX_BUF_NUM);
}

void RS485_Fun_init(void)
{
    RS485_Init(rs485_rx_buf, SBUS_RX_BUF_NUM);
}
/**
  * @brief          get remote control data point
  * @param[in]      none
  * @retval         remote control data point
  */
/**
  * @brief          获取遥控器数据指针
  * @param[in]      none
  * @retval         遥控器数据指针
  */
const RC_ctrl_t *get_remote_control_point(void)
{
    return &rc_ctrl;
}

//判断遥控器数据是否出错，
uint8_t RC_data_is_error(void)
{
    //使用了go to语句 方便出错统一处理遥控器变量数据归零
    if (RC_abs(rc_ctrl.rc.ch[0]) > RC_CHANNAL_ERROR_VALUE)
    {
        goto error;
    }
    if (RC_abs(rc_ctrl.rc.ch[1]) > RC_CHANNAL_ERROR_VALUE)
    {
        goto error;
    }
    if (RC_abs(rc_ctrl.rc.ch[2]) > RC_CHANNAL_ERROR_VALUE)
    {
        goto error;
    }
    if (RC_abs(rc_ctrl.rc.ch[3]) > RC_CHANNAL_ERROR_VALUE)
    {
        goto error;
    }
    if (rc_ctrl.rc.s[0] == 0)
    {
        goto error;
    }
    if (rc_ctrl.rc.s[1] == 0)
    {
        goto error;
    }
    return 0;

error:
    rc_ctrl.rc.ch[0] = 0;
    rc_ctrl.rc.ch[1] = 0;
    rc_ctrl.rc.ch[2] = 0;
    rc_ctrl.rc.ch[3] = 0;
    rc_ctrl.rc.s[0] = RC_SW_DOWN;
    rc_ctrl.rc.s[1] = RC_SW_DOWN;
    rc_ctrl.rc.s[2] = RC_SW_DOWN;
    rc_ctrl.rc.s[3] = RC_SW_DOWN;		
    rc_ctrl.mouse.x = 0;
    rc_ctrl.mouse.y = 0;
    rc_ctrl.mouse.z = 0;
    rc_ctrl.mouse.press_l = 0;
    rc_ctrl.mouse.press_r = 0;
    rc_ctrl.key.v = 0;
    return 1;
}

void slove_RC_lost(void)
{
    RC_restart(SBUS_RX_BUF_NUM);
}
void slove_data_error(void)
{
    RC_restart(SBUS_RX_BUF_NUM);
}


void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
		if(huart1.Instance->SR & UART_FLAG_RXNE)//接收到数据
    {
        __HAL_UART_CLEAR_PEFLAG(&huart1);
    }
    else if(USART1->SR & UART_FLAG_IDLE)
    {
        static uint16_t this_time_rx_len = 0;

        __HAL_UART_CLEAR_PEFLAG(&huart1);

        //disable DMA
        //失效DMA
		__HAL_DMA_DISABLE(&hdma_usart1_rx);
		//get receive data length, length = set_data_length - remain_length
        //获取接收数据长度,长度 = 设定长度 - 剩余长度
		this_time_rx_len = SBUS_RX_BUF_NUM - hdma_usart1_rx.Instance->CNDTR;
		if(this_time_rx_len == RC_FRAME_LENGTH)
		{
			#ifdef _IBUS
				ibus_to_rc(sbus_rx_buf, &rc_ctrl);
			#else  
				sbus_to_rc(sbus_rx_buf, &rc_ctrl);
			#endif		
			//记录数据接收时间
       detect_hook(IBUS_TOE);
			// 准备把值发给上位机
      // sbus_to_usart1(sbus_rx_buf[1]);
		}
			
		//reset set_data_lenght
        //重新设定数据长度
		hdma_usart1_rx.Instance->CNDTR = SBUS_RX_BUF_NUM;
		
		//enable DMA
        //使能DMA
		__HAL_DMA_ENABLE(&hdma_usart1_rx);

    }

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
if(huart3.Instance->SR & UART_FLAG_RXNE)//接收到数据
    {
        __HAL_UART_CLEAR_PEFLAG(&huart3);				
    }
    else if(USART3->SR & UART_FLAG_IDLE)
    {
        static uint16_t this_time_rx_len = 0;

        __HAL_UART_CLEAR_PEFLAG(&huart3);

        //disable DMA
        //失效DMA
		__HAL_DMA_DISABLE(&hdma_usart3_rx);
		//get receive data length, length = set_data_length - remain_length
        //获取接收数据长度,长度 = 设定长度 - 剩余长度
		this_time_rx_len = SBUS_RX_BUF_NUM - hdma_usart3_rx.Instance->CNDTR;
		if(this_time_rx_len == RS485_LENGTH)
		{
			rs485_r(rs485_rx_buf, &rs485_t);
			// 准备把值发给上位机
      // sbus_to_usart1(sbus_rx_buf[1]);
		}
		//reset set_data_lenght
        //重新设定数据长度
		hdma_usart3_rx.Instance->CNDTR = SBUS_RX_BUF_NUM;
		__HAL_DMA_ENABLE(&hdma_usart3_rx);
	}
  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}


//取正函数
static int16_t RC_abs(int16_t value)
{
    if (value > 0)
    {
        return value;
    }
    else
    {
        return -value;
    }
}

/**
  * @brief          remote control protocol resolution
  * @param[in]      sbus_buf: raw data point
  * @param[out]     rc_ctrl: remote control data struct point
  * @retval         none
  */
/**
  * @brief          遥控器协议解析
  * @param[in]      sbus_buf: 原生数据指针
  * @param[out]     rc_ctrl: 遥控器数据指
  * @retval         none
  */
#ifdef _IBUS
static void ibus_to_rc(volatile const uint8_t *sbus_buf, RC_ctrl_t *rc_ctrl)
{
    if (sbus_buf == NULL || rc_ctrl == NULL)
    {
        return;
    }  
		
	uint16_t sum = 0;
	for(int i=0; i<30; i++)
		sum += sbus_buf[i];
	sum ^= 0xFFFF;
	if((sbus_buf[0]==0x20&&sbus_buf[1]==0x40)&&(((uint8_t)sum == sbus_buf[30])&&((uint8_t)(sum>>8)==sbus_buf[31])))
	{
		rc_ctrl->rc.ch[0]  = ((int16_t)sbus_buf[2]  | (((int16_t)sbus_buf[3] & 0x0F) << 8 )) & 0x0FFF;
    rc_ctrl->rc.ch[1]  = ((int16_t)sbus_buf[4]  | (((int16_t)sbus_buf[5] & 0x0F) << 8 )) & 0x0FFF;
		rc_ctrl->rc.ch[2]  = ((int16_t)sbus_buf[6]  | (((int16_t)sbus_buf[7] & 0x0F) << 8 )) & 0x0FFF;
    rc_ctrl->rc.ch[3]  = ((int16_t)sbus_buf[8]  | (((int16_t)sbus_buf[9] & 0x0F) << 8 )) & 0x0FFF;
		rc_ctrl->rc.s[0]   = ((int16_t)sbus_buf[10] | (((int16_t)sbus_buf[11] & 0x0F) << 8 )) & 0x0FFF;
		rc_ctrl->rc.s[1]   = ((int16_t)sbus_buf[12] | (((int16_t)sbus_buf[13] & 0x0F) << 8 )) & 0x0FFF;
		rc_ctrl->rc.s[2]   = ((int16_t)sbus_buf[14] | (((int16_t)sbus_buf[15] & 0x0F) << 8 )) & 0x0FFF;
		rc_ctrl->rc.s[3]   = ((int16_t)sbus_buf[16] | (((int16_t)sbus_buf[17] & 0x0F) << 8 )) & 0x0FFF;
		rc_ctrl->rc.var[0] = ((int16_t)sbus_buf[18] | (((int16_t)sbus_buf[19] & 0x0F) << 8 )) & 0x0FFF;
		rc_ctrl->rc.var[1] = ((int16_t)sbus_buf[20] | (((int16_t)sbus_buf[21] & 0x0F) << 8 )) & 0x0FFF;
		
		
		if(rc_ctrl->rc.s[0] == 1000)
			rc_ctrl->rc.s[0] = RC_SW_UP;
		else if(rc_ctrl->rc.s[0] == 2000)
			rc_ctrl->rc.s[0] = RC_SW_DOWN;
		
		if(rc_ctrl->rc.s[1] == 1000)
			rc_ctrl->rc.s[1] = RC_SW_UP;
		else if(rc_ctrl->rc.s[1] == 2000)
			rc_ctrl->rc.s[1] = RC_SW_DOWN;
		else if(rc_ctrl->rc.s[1] == 1500)
			rc_ctrl->rc.s[1] = RC_SW_MID;
		
		if(rc_ctrl->rc.s[2] == 1000)
			rc_ctrl->rc.s[2] = RC_SW_UP;
		else if(rc_ctrl->rc.s[2] == 2000)
			rc_ctrl->rc.s[2] = RC_SW_DOWN;
		else if(rc_ctrl->rc.s[2] == 1500)
			rc_ctrl->rc.s[2] = RC_SW_MID;

		if(rc_ctrl->rc.s[3] == 1000)
			rc_ctrl->rc.s[3] = RC_SW_UP;
		else if(rc_ctrl->rc.s[3] == 2000)
			rc_ctrl->rc.s[3] = RC_SW_DOWN;
		
		 
		rc_ctrl->rc.ch[0] = (int16_t)(((float)rc_ctrl->rc.ch[0]-1500) / 500.0f * 1000); 
		rc_ctrl->rc.ch[1] = (int16_t)(((float)rc_ctrl->rc.ch[1]-1500) / 500.0f * 1000); 
		rc_ctrl->rc.ch[2] = (int16_t)(((float)rc_ctrl->rc.ch[2]-1000)) ;/// 500.0f * 1000); 
		rc_ctrl->rc.ch[3] = (int16_t)(((float)rc_ctrl->rc.ch[3]-1500) / 500.0f * 1000); 
	
		if(RC_abs(rc_ctrl->rc.ch[0])<10)
			rc_ctrl->rc.ch[0] = 0;
		if(RC_abs(rc_ctrl->rc.ch[1])<10)
			rc_ctrl->rc.ch[1] = 0;
		if(RC_abs(rc_ctrl->rc.ch[2])<50)
			rc_ctrl->rc.ch[2] = 0;
		if(RC_abs(rc_ctrl->rc.ch[3])<50)
			rc_ctrl->rc.ch[3] = 0;

	}

	RC_data_is_error();
		
}
#else  
static void sbus_to_rc(volatile const uint8_t *sbus_buf, RC_ctrl_t *rc_ctrl)
{
    if (sbus_buf == NULL || rc_ctrl == NULL)
    {
        return;
    }
   
//    rc_ctrl->rc.ch[0] = (sbus_buf[0] | (sbus_buf[1] << 8)) & 0x07ff;        //!< Channel 0
		
		rc_ctrl->rc.ch[0] = ((int16_t)sbus_buf[1] >> 0 | ((int16_t)sbus_buf[2] << 8 )) & 0x07FF;  															 //!< Channel 1
		rc_ctrl->rc.ch[1] = ((int16_t)sbus_buf[2] >> 3 | ((int16_t)sbus_buf[3] << 5 )) & 0x07FF; 																 //!< Channel 2
		rc_ctrl->rc.ch[2] =	((int16_t)sbus_buf[3] >> 6 | ((int16_t)sbus_buf[4] << 2 ) | (int16_t)sbus_buf[5] << 10 ) & 0x07FF; 	 //!< Channel 3
		rc_ctrl->rc.ch[3] = ((int16_t)sbus_buf[5] >> 1 | ((int16_t)sbus_buf[6] << 7 )) & 0x07FF;																 //!< Channel 4
		rc_ctrl->rc.s[0] =	((int16_t)sbus_buf[6] >> 4 | ((int16_t)sbus_buf[7] << 4 )) & 0x07FF;      													//!< SWA	
		rc_ctrl->rc.s[1] =	((int16_t)sbus_buf[7] >> 7 | ((int16_t)sbus_buf[8] << 1 ) | (int16_t)sbus_buf[9] << 9 ) & 0x07FF;   //!< SWB
		rc_ctrl->rc.s[2] =	((int16_t)sbus_buf[9] >> 2 | ((int16_t)sbus_buf[10] << 6 )) & 0x07FF;																//!< SWC
		rc_ctrl->rc.s[3] =	((int16_t)sbus_buf[10] >> 5 | ((int16_t)sbus_buf[11] << 3 )) & 0x07FF;															//!< SWD
		rc_ctrl->rc.var[0] = ((int16_t)sbus_buf[12] << 0 | ((int16_t)sbus_buf[13] << 8 )) & 0x07FF;
    rc_ctrl->rc.var[1] = ((int16_t)sbus_buf[13] >> 3 | ((int16_t)sbus_buf[14] << 5 )) & 0x07FF;	
		
		
		
    rc_ctrl->rc.ch[0] = (float)(rc_ctrl->rc.ch[0]-RC_CH_VALUE_OFFSET)/784.0f*1000;
    rc_ctrl->rc.ch[1] = (float)(rc_ctrl->rc.ch[1]-RC_CH_VALUE_OFFSET)/784.0f*1000;
    rc_ctrl->rc.ch[2] = (float)(rc_ctrl->rc.ch[2]-RC_CH_VALUE_OFFSET)/784.0f*1000;
    rc_ctrl->rc.ch[3] = (float)(rc_ctrl->rc.ch[3]-RC_CH_VALUE_OFFSET)/784.0f*1000;

		/*
		
		1000 - 2000
		1000 1500 2000
		
		*/
		
//			rc_ctrl->rc.ch[0] = 
		
		
		if(rc_ctrl->rc.s[0] == RC_CH_VALUE_MIN)
				rc_ctrl->rc.s[0] = RC_SW_UP;
		else if(rc_ctrl->rc.s[0] == RC_CH_VALUE_MAX)
			 rc_ctrl->rc.s[0] = RC_SW_DOWN;
		
		if(rc_ctrl->rc.s[1] == RC_CH_VALUE_MIN)
				rc_ctrl->rc.s[1] = RC_SW_UP;
		else if(rc_ctrl->rc.s[1] == RC_CH_VALUE_MAX)
			  rc_ctrl->rc.s[1] = RC_SW_DOWN;
		else if(rc_ctrl->rc.s[1] == RC_CH_VALUE_OFFSET)
				rc_ctrl->rc.s[1] = RC_SW_MID;
		
		if(rc_ctrl->rc.s[2] == RC_CH_VALUE_MIN)
				rc_ctrl->rc.s[2] = RC_SW_UP;
		else if(rc_ctrl->rc.s[2] == RC_CH_VALUE_MAX)
			  rc_ctrl->rc.s[2] = RC_SW_DOWN;
		else if(rc_ctrl->rc.s[2] == RC_CH_VALUE_OFFSET)
				rc_ctrl->rc.s[2] = RC_SW_MID;

		if(rc_ctrl->rc.s[3] == RC_CH_VALUE_MIN)
				rc_ctrl->rc.s[3] = RC_SW_UP;
		else if(rc_ctrl->rc.s[3] == RC_CH_VALUE_MAX)
			 rc_ctrl->rc.s[3] = RC_SW_DOWN;
		
		RC_data_is_error();
		
}
#endif

void rs485_r(volatile const uint8_t *sbus_buf, RS485_t* rs485_t)
{
	rs485_t->rc.a[0] = sbus_buf[0];
	rs485_t->rc.a[1] = sbus_buf[1];
	rs485_t->rc.a[2] = sbus_buf[2];
	rs485_t->rc.a[3] = sbus_buf[3];

}
void RS485_SendByte(char* SendData)
{	   
	uint8_t size;
	
	HAL_GPIO_WritePin(GPIOB, RS485_S0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, RS485_S1_Pin, GPIO_PIN_SET);
	osDelay(5);
	
	size = (uint8_t)strlen(SendData);
	
	HAL_UART_Transmit(&huart3,(uint8_t*)SendData,size,0xffff);
	
	while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TXE) == RESET);
	
	osDelay(5);
	HAL_GPIO_WritePin(GPIOB, RS485_S0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, RS485_S1_Pin, GPIO_PIN_RESET);
} 
