/*
 * Copyright (c) 2022-2024, ArtInChip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Authors: Siyao.Li <siyao.li@artinchip.com>
 */
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/time.h>
#include <rtthread.h>

#include "hal_adcim.h"
#include "rtdevice.h"
#include "aic_core.h"
#include "aic_log.h"
#include "hal_gpai.h"

/* Global macro and variables */
#define AIC_GPAI_NAME               "gpai"
/* The default voltages are set to D21x->3.0V, D31x、D12x->2.5V */
#define AIC_GPAI_DEFAULT_VOLTAGE    3
#define AIC_GPAI_VOLTAGE_ACCURACY   10000
#define AIC_GPAI_FIFO_MAX_DEPTH     32

static rt_adc_device_t gpai_dev;
static const char sopts[] = "c:t:n:h";
static const struct option lopts[] = {
    {"channel", required_argument, NULL, 'c'},
    {"voltage", required_argument, NULL, 't'},
    {"number",  required_argument, NULL, 'n'},
    {"help",          no_argument, NULL, 'h'},
    {0, 0, 0, 0}
    };

static int g_sample_num = -1;
static u32 g_cal_param;
static rt_sem_t g_gpai_sem = RT_NULL;
/* Functions */

static void cmd_gpai_usage(char *program)
{
    printf("Compile time: %s %s\n", __DATE__, __TIME__);
    printf("Usage: %s [options]\n", program);
    printf("\t -c, --channel\t\tSelect one channel in [0, %d], default is 0\n",
           AIC_GPAI_CH_NUM - 1);
    printf("\t -t, --voltage\t\tModify default voltage\n");
    printf("\t -n, --number\t\tSet the number of samples\n");
    printf("\t -h, --help \n");
    printf("\n");
    printf("Example: %s -c 4 -n 100 -t 3\n", program);
}

static void gpai_irq_callback(void *arg)
{
    rt_sem_release(g_gpai_sem);
}

static int gpai_get_adc_period(struct aic_gpai_ch_info ch_info,
                               float def_voltage)
{
    int cnt = 0;
    int voltage = 0;
    rt_base_t level;
    int scale = AIC_GPAI_VOLTAGE_ACCURACY;
    int sample_cnt = g_sample_num;
    struct aic_gpai_irq_info irq_info;
    int ch = ch_info.chan_id;

    g_cal_param = hal_adcim_auto_calibration();

    irq_info.chan_id = ch;
    irq_info.callback = gpai_irq_callback;
    irq_info.callback_param = NULL;

    if (!g_gpai_sem)
        g_gpai_sem = rt_sem_create("gpai_period_sem", 0, RT_IPC_FLAG_FIFO);

    rt_adc_control(gpai_dev, RT_ADC_CMD_IRQ_CALLBACK, &irq_info);
    rt_adc_enable(gpai_dev, ch);

    while (cnt < sample_cnt) {
        if (rt_sem_take(g_gpai_sem, RT_WAITING_FOREVER)!=RT_EOK)
            break;
        level = rt_hw_interrupt_disable();
        rt_adc_control(gpai_dev, RT_ADC_CMD_GET_CH_INFO, (void *)&ch_info);
        rt_hw_interrupt_enable(level);

        for (int i = 0; i < ch_info.fifo_valid_cnt; i++) {
            if (ch_info.adc_values[i]) {
                voltage = hal_adcim_adc2voltage(&ch_info.adc_values[i],
                                                g_cal_param,
                                                AIC_GPAI_VOLTAGE_ACCURACY,
                                                def_voltage);
                rt_kprintf("[%d] GPAI ch%d: %d\n", cnt, ch,
                           ch_info.adc_values[i]);
                rt_kprintf("GPAI voltage:%d.%04d v\n", voltage / scale,
                           voltage % scale);
            }
            cnt++;
        }

    }

    rt_adc_disable(gpai_dev, ch);

    return voltage;
}

static int gpai_get_adc_single(struct aic_gpai_ch_info ch_info,
                               float def_voltage)
{
    int cnt = 0;
    int voltage = 0;
    int scale = AIC_GPAI_VOLTAGE_ACCURACY;
    int sample_cnt = g_sample_num;
    u16 adc_val;
    int ch = ch_info.chan_id;

    g_cal_param = hal_adcim_auto_calibration();
    gpai_dev = (rt_adc_device_t)rt_device_find(AIC_GPAI_NAME);
    if (!gpai_dev) {
        rt_kprintf("Failed to open %s device\n", AIC_GPAI_NAME);
        return -RT_ERROR;
    }

    rt_adc_enable(gpai_dev, ch);

    while (cnt < sample_cnt) {
        rt_adc_control(gpai_dev, RT_ADC_CMD_GET_CH_INFO, (void *)&ch_info);

        aicos_msleep(10);
        adc_val = ch_info.adc_values[0];
        if (adc_val) {
            voltage = hal_adcim_adc2voltage(&adc_val, g_cal_param,
                                            AIC_GPAI_VOLTAGE_ACCURACY,
                                            def_voltage);
            rt_kprintf("[%d] GPAI ch%d: %d\n", cnt, ch, adc_val);
            rt_kprintf("GPAI voltage:%d.%04d v\n", voltage / scale,
                       voltage % scale);
        }
        cnt++;
    }

    rt_adc_disable(gpai_dev, ch);

    return voltage;
}

static int cmd_test_gpai(int argc, char **argv)
{
    int c;
    int ret;
    u32 ch = 0;
    float def_voltage = AIC_GPAI_DEFAULT_VOLTAGE;
    int voltage = -1;
    struct aic_gpai_ch_info ch_info = {0};

    if (argc < 2) {
        cmd_gpai_usage(argv[0]);
        return voltage;
    }

    optind = 0;
    while ((c = getopt_long(argc, argv, sopts, lopts, NULL)) != -1) {
        switch (c) {
        case 'c':
            ch = atoi(optarg);
            if ((ch < 0) || (ch >= AIC_GPAI_CH_NUM))
                pr_err("Invalid channel No.%s\n", optarg);
            break;
        case 't':
            def_voltage = atof(optarg);
            break;
        case 'n':
            g_sample_num = atoi(optarg);
            break;
        case 'h':
        default:
            cmd_gpai_usage(argv[0]);
            return voltage;
        }
    }

    if (ch < 0) {
        rt_kprintf("Please select a channel first\n");
        return voltage;
    }
    if (def_voltage < 0) {
        rt_kprintf("Please input valid standard voltage\n");
        return voltage;
    }

    if (g_sample_num <= 0) {
        rt_kprintf("Please set the number of samples\n");
        return voltage;
    }

    gpai_dev = (rt_adc_device_t)rt_device_find(AIC_GPAI_NAME);
    if (!gpai_dev) {
        rt_kprintf("Failed to open %s device\n", AIC_GPAI_NAME);
        return -RT_ERROR;
    }

    ch_info.chan_id = ch;

    ret = rt_adc_control(gpai_dev, RT_ADC_CMD_GET_MODE, (void *)&ch_info);
    if (ret) {
        rt_kprintf("Failed to get GPAI mode\n");
        return -RT_ERROR;
    }

    switch (ch_info.mode) {
    case AIC_GPAI_MODE_SINGLE:
        rt_kprintf("Starting gpai single mode\n");
        voltage = gpai_get_adc_single(ch_info, def_voltage);
        break;
    case AIC_GPAI_MODE_PERIOD:
        rt_kprintf("Starting gpai period mode\n");
        voltage = gpai_get_adc_period(ch_info, def_voltage);
        break;
    default:
        rt_kprintf("Unknown mode: %#x\n");
        break;
    }


    return voltage;
}

MSH_CMD_EXPORT_ALIAS(cmd_test_gpai, test_gpai, gpai device sample);
