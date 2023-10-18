#ifndef __RT_DEF_H
#define __RT_DEF_H


/********************************************************************
* 											数据类型
*********************************************************************
*/
/* RT-Thread 基础数据类型 */
typedef signed char                      	rt_int8_t;
typedef signed short                      rt_int16_t;
typedef signed long                      	rt_int32_t;
typedef unsigned char                     rt_uint8_t;
typedef unsigned short                    rt_uint16_t;
typedef unsigned long                     rt_uint32_t;
typedef int                               rt_bool_t;

/* 32bit CPU */
typedef long                              rt_base_t;
typedef unsigned long                     rt_ubase_t;          

typedef rt_base_t                         rt_err_t;
typedef rt_uint32_t                       rt_time_t;
typedef rt_uint32_t												rt_tick_t;
typedef rt_base_t                         rt_flag_t;
typedef rt_ubase_t                        rt_size_t;
typedef rt_ubase_t                        rt_dev_t;
typedef rt_base_t                         rt_off_t;

/*  */




#endif  /* __RT_DEF_H */