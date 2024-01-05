#include <rtthread.h>
#include <rthw.h>

static rt_tick_t rt_tick = 0; /* ϵͳʱ�������� */
extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];
extern rt_uint32_t rt_thread_ready_priority_group;
#if 0	

void rt_tick_increase(void)
{
	rt_ubase_t i;
	struct rt_thread *thread;
	rt_tick++;

	/* ɨ������б��������̵߳� remaining_tic�������Ϊ0�����1 */
	for(i=0; i<RT_THREAD_PRIORITY_MAX; i++)
	{
		thread = rt_list_entry( rt_thread_priority_table[i].next,
														struct rt_thread,
														tlist);
	  if(thread->remaining_tick > 0)
		{
			thread->remaining_tick--;
		}
	}
	/* 扫描就绪列表中所有线程的remaining_tick，如果不为0，则减1 */
	for(i=0; i<RT_THREAD_PRIORITY_MAX; i++)
	{
		thread = rt_list_entry( rt_thread_priority_table[i].next,
														struct rt_thread,
														tlist);
		
	  if(thread->remaining_tick > 0)
		{
			thread->remaining_tick--;
			if(thread->remaining_tick == 0)
			{
				rt_thread_ready_priority_group |= thread->number_mask;
			}
		}
	}
	 /* ϵͳ���� */
	rt_schedule();
}
#else
void rt_tick_increase(void)
{
	
	struct rt_thread *thread;
	/* 系统时基计数器加1操作，rt_tick 是一个全局变量 */
	++rt_tick;
	
	/* 获取当前线程线程控制块 */
	thread = rt_thread_self();
	
	/* 时间片递减 */
	--thread->remaining_tick;
	
	/* 如果时间片用完，则重置时间片，然后让出处理器 */
	if(thread->remaining_tick == 0)
	{
		/* 重置时间片 */
		thread->remaining_tick = thread->init_tick;
		
		/* 让出处理器 */
		rt_thread_yield();
	}
	
	/* 扫描系统定时器列表 */
	rt_timer_check();
} 
#endif		

rt_tick_t rt_tick_get(void)
{
    /* return the global tick */
    return rt_tick;
}

