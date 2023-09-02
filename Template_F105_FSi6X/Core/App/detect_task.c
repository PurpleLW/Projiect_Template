/**
  ****************************(C) COPYRIGHT 2022 康沃瑞****************************
  * @file       detect_task.c/h
  * @brief      detect error task, judged by receiving data time. provide detect
                hook function, error exist function.
  *             检测错误任务， 通过接收数据时间来判断.提供 检测钩子函数,错误存在函数.
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Sep-30-2022     康沃瑞           1. done
  *  V1.0.1     Nov-04-2022      LW              1.增加获取设备对应的错误状态函数：unsigned char toe_is_error(uint8_t toe)
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2022 康沃瑞****************************
  */


#include "detect_task.h"
#include "cmsis_os.h"
#include "ANO_DT.h"
#include "remote_control.h"


/**
  * @brief          初始化error_list,赋值 offline_time, online_time, priority
  * @param[in]      time:系统时间
  * @retval         none
  */
static void detect_init(uint32_t time);


error_t error_list[ERROR_LIST_LENGHT + 1];

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t detect_task_stack;
#endif


void detect_task(void const * argument)
{
  /* USER CODE BEGIN detect_task */
	  static uint32_t system_time;
    system_time = xTaskGetTickCount();
    //init,初始化
    detect_init(system_time);
    //wait a time.空闲一段时间
    vTaskDelay(DETECT_TASK_INIT_TIME);

  /* Infinite loop */
      while (1)
    {
        static uint8_t error_num_display = 0;
        system_time = xTaskGetTickCount();

        error_num_display = ERROR_LIST_LENGHT;
        error_list[ERROR_LIST_LENGHT].is_lost = 0;
        error_list[ERROR_LIST_LENGHT].error_exist = 0;

        for (int i = 0; i < ERROR_LIST_LENGHT; i++)
        {
            //disable, continue
            //未使能，跳过
            if (error_list[i].enable == 0)
            {
                continue;
            }

            //judge offline.判断掉线
            if (system_time - error_list[i].new_time > error_list[i].set_offline_time)
            {
                if (error_list[i].error_exist == 0)
                {
                    //record error and time
                    //记录错误以及掉线时间
                    error_list[i].is_lost = 1;
                    error_list[i].error_exist = 1;
                    error_list[i].lost_time = system_time;
                }
                //judge the priority,save the highest priority ,
                //判断错误优先级， 保存优先级最高的错误码
                if (error_list[i].priority > error_list[error_num_display].priority)
                {
                    error_num_display = i;
                }
                

                error_list[ERROR_LIST_LENGHT].is_lost = 1;
                error_list[ERROR_LIST_LENGHT].error_exist = 1;
                //if solve_lost_fun != NULL, run it
                //如果提供解决函数，运行解决函数
                if (error_list[i].solve_lost_fun != NULL)
                {
                    error_list[i].solve_lost_fun();
                }
            }
            else if (system_time - error_list[i].work_time < error_list[i].set_online_time)
            {
                //just online, maybe unstable, only record
                //刚刚上线，可能存在数据不稳定，只记录不丢失，
                error_list[i].is_lost = 0;
                error_list[i].error_exist = 1;
            }
            else
            {
                error_list[i].is_lost = 0;
                //判断是否存在数据错误
                //judge if exist data error
                if (error_list[i].data_is_error != NULL)
                {
                    error_list[i].error_exist = 1;
                }
                else
                {
                    error_list[i].error_exist = 0;
                }
                //calc frequency
                //计算频率
                if (error_list[i].new_time > error_list[i].last_time)
                {
                    error_list[i].frequency = configTICK_RATE_HZ / (float)(error_list[i].new_time - error_list[i].last_time);
                }
            }
        }

		
		
   vTaskDelay(DETECT_CONTROL_TIME);
#if INCLUDE_uxTaskGetStackHighWaterMark
        detect_task_stack = uxTaskGetStackHighWaterMark(NULL);
#endif

  }
  /* USER CODE END detect_task */
}

/**
  * @brief          get toe error status
  * @param[in]      toe: table of equipment
  * @retval         true (eror) or false (no error)
  */
/**
  * @brief          获取设备对应的错误状态
  * @param[in]      toe:设备目录
  * @retval         true(错误) 或者false(没错误)
  */
unsigned char toe_is_error(uint8_t toe)
{
    return (error_list[toe].error_exist == 1);
}


void detect_hook(uint8_t toe)
{
    error_list[toe].last_time = error_list[toe].new_time;
    error_list[toe].new_time = xTaskGetTickCount();
    
    if (error_list[toe].is_lost)
    {
        error_list[toe].is_lost = 0;
        error_list[toe].work_time = error_list[toe].new_time;
    }
    
    if (error_list[toe].data_is_error_fun != NULL)
    {
        if (error_list[toe].data_is_error_fun())
        {
            error_list[toe].error_exist = 1;
            error_list[toe].data_is_error = 1;

            if (error_list[toe].solve_data_error_fun != NULL)
            {
                error_list[toe].solve_data_error_fun();
            }
        }
        else
        {
            error_list[toe].data_is_error = 0;
        }
    }
    else
    {
        error_list[toe].data_is_error = 0;
    }
}


/**
  * @brief          get error list
  * @param[in]      none
  * @retval         the point of error_list
  */
/**
  * @brief          得到错误列表
  * @param[in]      none
  * @retval         error_list的指针
  */
const error_t *get_error_list_point(void)
{
    return error_list;
}


static void detect_init(uint32_t time)
{
    //设置离线时间，上线稳定工作时间，优先级 offlineTime onlinetime priority
    uint16_t set_item[ERROR_LIST_LENGHT][3] =
        {
            {50, 60, 15},   //IBUS
<<<<<<< HEAD
            {100, 300, 11},   //motor1
            {100, 300, 11},   //motor2
=======
            {100, 400, 11},   //motor1
            {100, 400, 11},   //motor2
>>>>>>> e037807c5270ce72380d7a6187a961266f84d5ff
            {60, 200, 10},    //STEER1
            {60, 200, 10},    //STEER2
            {60, 200, 6},    //brake
//            {100, 100, 9},    //screen mode
//            {60, 100, 8},    //Ai mode

        };

    for (uint8_t i = 0; i < ERROR_LIST_LENGHT; i++)
    {
        error_list[i].set_offline_time = set_item[i][0];
        error_list[i].set_online_time = set_item[i][1];
        error_list[i].priority = set_item[i][2];
        error_list[i].data_is_error_fun = NULL;
        error_list[i].solve_lost_fun = NULL;
        error_list[i].solve_data_error_fun = NULL;

        error_list[i].enable = 1;
        error_list[i].error_exist = 1;
        error_list[i].is_lost = 1;
        error_list[i].data_is_error = 1;
        error_list[i].frequency = 0.0f;
        error_list[i].new_time = time;
        error_list[i].last_time = time;
        error_list[i].lost_time = time;
        error_list[i].work_time = time;
    }


//    error_list[DBUSTOE].dataIsErrorFun = RC_data_is_error;
//    error_list[DBUSTOE].solveLostFun = slove_RC_lost;
//    error_list[DBUSTOE].solveDataErrorFun = slove_data_error;

}
