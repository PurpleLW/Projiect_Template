/**
  ****************************(C) COPYRIGHT 2023 康沃瑞****************************
  * @file       firmware.c/h
  * @brief      固件代码，存放VCU原始参数，并将参数进行封装
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Jun-7-2023       LW              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2023 康沃瑞****************************
  */
	
#include "firmware.h"
#include "string.h"

void ParaSetFun(Parameter* para)
{
	strcpy((char *)para->Basic_infor.version,"BY_VCU_F105_2.2");

}