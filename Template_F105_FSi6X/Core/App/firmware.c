/**
  ****************************(C) COPYRIGHT 2023 ������****************************
  * @file       firmware.c/h
  * @brief      �̼����룬���VCUԭʼ�����������������з�װ
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Jun-7-2023       LW              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2023 ������****************************
  */
	
#include "firmware.h"
#include "string.h"

void ParaSetFun(Parameter* para)
{
	strcpy((char *)para->Basic_infor.version,"BY_VCU_F105_2.2");

}