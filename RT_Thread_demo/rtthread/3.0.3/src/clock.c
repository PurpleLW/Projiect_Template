#include <rtthread.h>
#include <rthw.h>

static rt_tick_t rt_tick = 0; /* 系统时基计数器 */
extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];
extern rt_uint32_t rt_thread_ready_priority_group;
#if 0	

void rt_tick_increase(void)
{
	rt_ubase_t i;
	struct rt_thread *thread;
	rt_tick++;

	/* 扫描就绪列表中所有线程的 remaining_tic，如果不为0，则减1 */
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
	/* 鎵弿灏辩华鍒楄〃涓墍鏈夌嚎绋嬬殑remaining_tick锛屽鏋滀笉涓�0锛屽垯鍑�1 */
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
	 /* 系统调度 */
	rt_schedule();
}
#else
void rt_tick_increase(void)
{
	
	struct rt_thread *thread;
	/* 绯荤粺鏃跺熀璁℃暟鍣ㄥ姞1鎿嶄綔锛宺t_tick 鏄竴涓叏灞�鍙橀噺 */
	++rt_tick;
	
	/* 鑾峰彇褰撳墠绾跨▼绾跨▼鎺у埗鍧� */
	thread = rt_thread_self();
	
	/* 鏃堕棿鐗囬�掑噺 */
	--thread->remaining_tick;
	
	/* 濡傛灉鏃堕棿鐗囩敤瀹岋紝鍒欓噸缃椂闂寸墖锛岀劧鍚庤鍑哄鐞嗗櫒 */
	if(thread->remaining_tick == 0)
	{
		/* 閲嶇疆鏃堕棿鐗� */
		thread->remaining_tick = thread->init_tick;
		
		/* 璁╁嚭澶勭悊鍣� */
		rt_thread_yield();
	}
	
	/* 鎵弿绯荤粺瀹氭椂鍣ㄥ垪琛� */
	rt_timer_check();
} 
#endif		

rt_tick_t rt_tick_get(void)
{
    /* return the global tick */
    return rt_tick;
}

