/*
*****************************************************************
*   main 
*****************************************************************
*/

#include "rtthread.h"
#include "stm32f10x.h"
#include <rthw.h>

rt_uint8_t flag1;
rt_uint8_t flag2;
rt_uint8_t flag3;

extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];

/* 定义线程控制块 */
struct rt_thread rt_flag1_thread;
struct rt_thread rt_flag2_thread;
struct rt_thread rt_flag3_thread;


ALIGN(RT_ALIGN_SIZE)

/* 定义线程栈 */
rt_uint8_t rt_flag1_thread_stack[512];
rt_uint8_t rt_flag2_thread_stack[512];
rt_uint8_t rt_flag3_thread_stack[512];

/* 线程声明 */
void flag1_thread_entry(void *p_arg);
void flag2_thread_entry(void *p_arg);
void flag3_thread_entry(void *p_arg);


/* 函数声明 */
void delay(rt_uint32_t count);



void SysTick_Handler(void)
{
	/* 进入中断 */
	rt_interrupt_enter();
	/* 时基更新 */
	rt_tick_increase();
	
	/* 离开中断 */
	rt_interrupt_leave();
}

int main(void)
{
	/* 硬件初始化 */
	/* 将硬件相关的初始化放在这里，如果是软件仿真则没有相关初始化代码 */

	/* 关中断 */
	rt_hw_interrupt_disable();

	/* SysTick 中断频率设置 */
	SysTick_Config( SystemCoreClock / RT_TICK_PER_SECOND );
	
	/* 系统定时器列表初始化 */
	rt_system_timer_init();
	
	/* 调度器初始化 */
	rt_system_scheduler_init();
	
	/* 初始化空闲线程 */
	rt_thread_idle_init();
	
	/* 初始化线程 */
	rt_thread_init( &rt_flag1_thread,					        /* 线程控制块 */
									"rt_flag1_thread",
									flag1_thread_entry,       				/* 线程入口地址 */
									RT_NULL,                   				/* 线程形参 */
									&rt_flag1_thread_stack[0], 				/* 线程栈起始地址 */
									sizeof(rt_flag1_thread_stack),
									2,
									4);	/* 线程栈大小，单位为字节 */
	/* 将线程插入到就绪列表 */
//	rt_list_insert_before( &(rt_thread_priority_table[0]),&(rt_flag1_thread.tlist));
	rt_thread_startup(&rt_flag1_thread);	
	/* 初始化线程 */
	rt_thread_init( &rt_flag2_thread,					        /* 线程控制块 */
									"rt_flag2_thread",
									flag2_thread_entry,       				/* 线程入口地址 */
									RT_NULL,                   				/* 线程形参 */
									&rt_flag2_thread_stack[0], 				/* 线程栈起始地址 */
									sizeof(rt_flag2_thread_stack),   /* 线程栈大小，单位为字节 */
									3, /* 优先级 */
									2);	 /* 时间片 */      
	/* 将线程插入到就绪列表 */
//	rt_list_insert_before( &(rt_thread_priority_table[1]),&(rt_flag2_thread.tlist));
	rt_thread_startup(&rt_flag2_thread);	

	/* 初始化线程 */
	rt_thread_init(&rt_flag3_thread,
									"rt_flag3_thread",
									flag3_thread_entry,
									RT_NULL,
									&rt_flag3_thread_stack[0],
									sizeof(rt_flag3_thread_stack),
									3,
									3);
	/* 将线程插入到就绪列表 */
	rt_thread_startup(&rt_flag3_thread);
	/* 启动系统调度器 */
	rt_system_scheduler_start();
	
}


/* 软件延时 */
void delay(rt_uint32_t count)
{
	for(;count!=0; count--) ;
}

/* 线程1  */
void flag1_thread_entry(void *p_arg)
{
	for( ;; )
	{
		flag1 = 1;
		rt_thread_delay(3);
		flag1 = 0;
		rt_thread_delay(3);
	}
}

/* 线程2  */
void flag2_thread_entry(void *p_arg)
{
	for( ;; )
	{
		flag2 = 1;
//		rt_thread_delay(2);
		delay(100);
		flag2 = 0;
//		rt_thread_delay(2);
		delay(100);

	}
}

/* 线程23 */
void flag3_thread_entry(void *p_arg)
{
	for( ;; )
	{
		flag3 = 1;
//		rt_thread_delay(3);
		delay(100);
		flag3 = 0;
//		rt_thread_delay(3);
		delay(100);
	}
}
