/**
  ****************************(C) COPYRIGHT 2022 ������****************************
  * @file       detect_task.c/h
  * @brief      detect error task, judged by receiving data time. provide detect
                hook function, error exist function.
  *             ���������� ͨ����������ʱ�����ж�.�ṩ ��⹳�Ӻ���,������ں���.
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Sep-30-2022     ������           1. done
  *  V1.0.1     Nov-04-2022      LW              1.���ӻ�ȡ�豸��Ӧ�Ĵ���״̬������unsigned char toe_is_error(uint8_t toe)
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2022 ������****************************
  */


#include "detect_task.h"
#include "cmsis_os.h"
#include "ANO_DT.h"
#include "remote_control.h"


/**
  * @brief          ��ʼ��error_list,��ֵ offline_time, online_time, priority
  * @param[in]      time:ϵͳʱ��
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
    //init,��ʼ��
    detect_init(system_time);
    //wait a time.����һ��ʱ��
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
            //δʹ�ܣ�����
            if (error_list[i].enable == 0)
            {
                continue;
            }

            //judge offline.�жϵ���
            if (system_time - error_list[i].new_time > error_list[i].set_offline_time)
            {
                if (error_list[i].error_exist == 0)
                {
                    //record error and time
                    //��¼�����Լ�����ʱ��
                    error_list[i].is_lost = 1;
                    error_list[i].error_exist = 1;
                    error_list[i].lost_time = system_time;
                }
                //judge the priority,save the highest priority ,
                //�жϴ������ȼ��� �������ȼ���ߵĴ�����
                if (error_list[i].priority > error_list[error_num_display].priority)
                {
                    error_num_display = i;
                }
                

                error_list[ERROR_LIST_LENGHT].is_lost = 1;
                error_list[ERROR_LIST_LENGHT].error_exist = 1;
                //if solve_lost_fun != NULL, run it
                //����ṩ������������н������
                if (error_list[i].solve_lost_fun != NULL)
                {
                    error_list[i].solve_lost_fun();
                }
            }
            else if (system_time - error_list[i].work_time < error_list[i].set_online_time)
            {
                //just online, maybe unstable, only record
                //�ո����ߣ����ܴ������ݲ��ȶ���ֻ��¼����ʧ��
                error_list[i].is_lost = 0;
                error_list[i].error_exist = 1;
            }
            else
            {
                error_list[i].is_lost = 0;
                //�ж��Ƿ�������ݴ���
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
                //����Ƶ��
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
  * @brief          ��ȡ�豸��Ӧ�Ĵ���״̬
  * @param[in]      toe:�豸Ŀ¼
  * @retval         true(����) ����false(û����)
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
  * @brief          �õ������б�
  * @param[in]      none
  * @retval         error_list��ָ��
  */
const error_t *get_error_list_point(void)
{
    return error_list;
}


static void detect_init(uint32_t time)
{
    //��������ʱ�䣬�����ȶ�����ʱ�䣬���ȼ� offlineTime onlinetime priority
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
