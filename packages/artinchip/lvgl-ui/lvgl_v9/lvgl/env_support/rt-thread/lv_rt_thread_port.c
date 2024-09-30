/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: MIT
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-18     Meco Man     the first version
 * 2022-05-10     Meco Man     improve rt-thread initialization process
 */

#ifdef __RTTHREAD__

#include <lvgl.h>
#include <rtthread.h>

#define DBG_TAG    "LVGL"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

#ifndef LPKG_LVGL_THREAD_STACK_SIZE
    #define LPKG_LVGL_THREAD_STACK_SIZE 4096
#endif /* LPKG_LVGL_THREAD_STACK_SIZE */

#ifndef LPKG_LVGL_THREAD_PRIO
    #define LPKG_LVGL_THREAD_PRIO (RT_THREAD_PRIORITY_MAX*2/3)
#endif /* LPKG_LVGL_THREAD_PRIO */

#ifndef LPKG_LVGL_DISP_REFR_PERIOD
    #define LPKG_LVGL_DISP_REFR_PERIOD 33
#endif /* LPKG_LVGL_DISP_REFR_PERIOD */

#define LPKG_LVGL_DELAY_TIME 1

extern void lv_port_disp_init(void);
extern void lv_port_indev_init(void);
extern void lv_user_gui_init(void);

static struct rt_thread lvgl_thread;

#ifdef rt_align
    rt_align(RT_ALIGN_SIZE)
#else
    ALIGN(RT_ALIGN_SIZE)
#endif

#ifndef LV_MAIN_THREAD_STACK_SIZE
    #define LV_MAIN_THREAD_STACK_SIZE (4096 * 2)
#endif /* LV_MAIN_THREAD_STACK_SIZE */

static rt_uint8_t lvgl_thread_stack[LV_MAIN_THREAD_STACK_SIZE];

#if LV_USE_LOG
static void lv_rt_log(lv_log_level_t level, const char *buf)
{
    (void)level;
    LOG_I(buf);
}
#endif /* LV_USE_LOG */

static void lvgl_thread_entry(void *parameter)
{
#if LV_USE_LOG
    lv_log_register_print_cb(lv_rt_log);
#endif /* LV_USE_LOG */
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    lv_user_gui_init();

    lv_tick_set_cb(&rt_tick_get_millisecond);

    /* handle the tasks of LVGL */
    while(1)
    {
        lv_task_handler();
        rt_thread_mdelay(LPKG_LVGL_DELAY_TIME);
    }
}

int lvgl_thread_init(void)
{
    rt_err_t err;

    err = rt_thread_init(&lvgl_thread, "LVGL", lvgl_thread_entry, RT_NULL,
           &lvgl_thread_stack[0], sizeof(lvgl_thread_stack), LPKG_LVGL_THREAD_PRIO, 10);
    if(err != RT_EOK)
    {
        LOG_E("Failed to create LVGL thread");
        return -1;
    }
    rt_thread_startup(&lvgl_thread);

    return 0;
}
//INIT_ENV_EXPORT(lvgl_thread_init);

#endif /*__RTTHREAD__*/
