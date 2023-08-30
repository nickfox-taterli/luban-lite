/*
 * Copyright (c) 2022-2023, ArtInChip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef LV_TPC_RUN
#define LV_TPC_RUN

#ifdef __cplusplus
extern "C" {
#endif

#include <rtthread.h>
#include <rtdevice.h>

int tpc_run(const char *name, rt_uint16_t x, rt_uint16_t y);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LV_TPC_RUN */
