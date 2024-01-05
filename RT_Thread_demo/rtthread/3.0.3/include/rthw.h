#ifndef __RT_HW_H
#define __RT_HW_H
#include "rtthread.h"

extern void rt_hw_context_switch_to(rt_uint32_t to);
extern void rt_hw_context_switch(rt_uint32_t from, rt_uint32_t to);
extern rt_base_t rt_hw_interrupt_disable();
extern void rt_hw_interrupt_enable(rt_base_t level);


#endif  /* __RT_DEF_H */