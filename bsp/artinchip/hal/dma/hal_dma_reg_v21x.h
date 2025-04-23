/*
 * Copyright (c) 2025, ArtInChip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _ARTINCHIP_HAL_DMA_REG_V3X_H_
#define _ARTINCHIP_HAL_DMA_REG_V3X_H_

#include "aic_core.h"
#include "hal_dma.h"
#include "aic_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NEED_EXTRA_REGISTER(value, x) ((value) % (x) != 0U)
#define TASK_MAX_NUM            24

#define DELAY_DEF_VAL           0x40

#define DMA_IRQ_CHAN_NR         4
#define MAX_LEN                 0x2000000
#define DMA_MAX_BUSWIDTH        DMA_SLAVE_BUSWIDTH_8_BYTES

#define DMA_IRQ_EN_REG(x)       ((x) * 0x04 + 0x00)
#define DMA_IRQ_DIS_REG(x)      ((x) * 0x04 + 0x20)
#define DMA_CHAN_OFFSET         (0x80)
#define DMA_IRQ_STA_REG(x)      ((x) * 0x04 + 0x40)
#define DMA_CH_STA_REG          (0x00A0)
#define DMA_LINK_ID_DEF         0xa1c86688
#define DMA_LINK_END_FLAG       0xFFFFFFFC

/*
 * define dma_v2.1 register list
 */
#define DMA_MEM_CFG             (0x0020)
#define DMA_GATE_REG            (0x0028)
#define DMA_CH_EN_REG           (0x0000)
#define DMA_CH_PAUSE_REG        (0x0004)
#define DMA_CH_TASK_REG         (0x0008)
#define DMA_CH_CFG_REG          (0x000C)
#define DMA_CH_SRC_REG          (0x0010)
#define DMA_CH_SINK_REG         (0x0014)
#define DMA_CH_LEFT_REG         (0x0018)
#define DMA_CH_MODE_REG         (0x0028)
#define DMA_CH_PKG_NUM_REG      (0x0030)
#define DMA_CH_MEMSET_VAL_REG   (0x0034)

#define DMA_BUS_CFG_REG         (0x80)
#define DMA_SET_LINK_ID_REG     (0x88)
#define DMA_FIFO_SIZE_REG       (0x90)
/* channel config */
#define DMA_CH_CTL1_REG         (0x00)
#define DMA_CH_CTL2_REG         (0x04)
#define DMA_CH_TASK_ADD1_REG    (0x08)
#define DMA_CH_TASK_ADD2_REG    (0x0C)
#define DMA_CH_CTL3_REG         (0x10)
#define DMA_CH_CTL4_REG         (0x14)
#define DMA_CH_MEM_SET_REG      (0x18)
#define DMA_CH_TASK_BCNT_REG    (0x1c)
#define DMA_LINK_ID_REG         (0x20)
/* task config */
#define DMA_TASK_CFG1_REG       (0x24)
#define DMA_BLOCK_LEN_REG       (0x28)
#define DMA_SRC_ADDR_REG        (0x2C)
#define DMA_DST_ADDR_REG        (0x30)
#define DMA_TASK_LEN_REG        (0x34)
#define DMA_TASK_CFG2_REG       (0x38)
#define DMA_NEXT_TASK_ADDR_REG  (0x3c)
#define DMA_SRC_WB_ADDR_SET_REG (0x40)
#define DMA_DST_WB_ADDR_SET_REG (0x44)
#define DMA_SRC_WB_DATA_REG     (0x48)
#define DMA_DST_WB_DATA_REG     (0x4C)
#define DMA_DEBUG_REG           (0x60)

/*
 * define macro for access register for specific channel
 */
#define DMA_IRQ_HALF_TASK   BIT(0)
#define DMA_IRQ_ONE_TASK    BIT(1)
#define DMA_IRQ_LINK_TASK   BIT(2)
#define DMA_IRQ_ID_ERR      BIT(3)
#define DMA_IRQ_ADDR_ERR    BIT(4)
#define DMA_IRQ_RD_AHB_ERR  BIT(5)
#define DMA_IRQ_WT_AHB_ERR  BIT(6)
#define DMA_IRQ_WT_AXI_ERR  BIT(7)
#define DMA_IRQ_CH_WIDTH    (8)
#define DMA_IRQ_MASK(ch)    (GENMASK(7, 0) << DMA_IRQ_SHIFT(ch))
#define DMA_IRQ_SHIFT(ch)   (DMA_IRQ_CH_WIDTH * (ch))
#define AIC_DMA_BUS_WIDTH                                                   \
    (BIT(DMA_SLAVE_BUSWIDTH_1_BYTE) | BIT(DMA_SLAVE_BUSWIDTH_2_BYTES)  |    \
     BIT(DMA_SLAVE_BUSWIDTH_4_BYTES) | BIT(DMA_SLAVE_BUSWIDTH_8_BYTES) |    \
     BIT(DMA_SLAVE_BUSWIDTH_16_BYTES))
/*
 * define bit index in channel configuration register
 */

/* task config1 */
#define SRC_PORT_BITSHIFT   0
#define SRC_BURST_BITSHIFT  7
#define SRC_TYPE_BITSHIFT   9
#define SRC_WIDTH_BITSHIFT  12
#define DST_PORT_BITSHIFT   16
#define DST_BURST_BITSHIFT  23
#define DST_TYPE_BITSHIFT   25
#define DST_WIDTH_BITSHIFT  28

/* task config2 */
#define DST_BUS_HPORT       28
#define SRC_BUS_HPROT       24
#define NEXT_TASK_H_ADDR    16
#define DST_HIGH_SADDR      8
#define SRC_HIGH_SADDR      0

#define DMA_DRQ_PORT_MASK   0x3F
#define DMA_WAIT_MODE       0
#define DMA_HANDSHAKE_MODE  1
#define DMA_DST_MODE_SHIFT  3
#define DMA_SRC_MODE_SHIFT  2
#define DMA_SRC_HANDSHAKE_ENABLE    4
#define DMA_DST_HANDSHAKE_ENABLE    4
#define DMA_S_WAIT_D_HANDSHAKE  (DMA_HANDSHAKE_MODE << DMA_DST_MODE_SHIFT)
#define DMA_S_HANDSHAKE_D_WAIT  (DMA_HANDSHAKE_MODE << DMA_SRC_MODE_SHIFT)
#define DMA_S_WAIT_D_WAIT   (DMA_WAIT_MODE)
#define DMA_FIFO_SIZE       0x200

/*
 * define bit index in channel pause register
 */
#define DMA_CH_RESUME       0x00
#define DMA_CH_LINK_PAUSE   0x01
#define DMA_CH_TASK_PAUSE   0x02
#define DMA_CH_ABANDON      0x04
#define DMA_CH_MEMSET       0x10

#ifdef __cplusplus
}
#endif

#define DMA_SLAVE_DEF(_id, _burst, _width) \
    static const struct dma_slave_table aic_dma_cfg_##_id = { \
        .id = _id, \
        .burst = _burst, \
        .burst_num = ARRAY_SIZE(_burst), \
        .width = _width, \
        .width_num = ARRAY_SIZE(_width), \
    }

#define AIC_DMA_CFG(_id)  [_id] = &(aic_dma_cfg_##_id)

static const u32 dma_width_1_byte[] = {DMA_SLAVE_BUSWIDTH_1_BYTE};
static const u32 dma_width_2_bytes[] = {DMA_SLAVE_BUSWIDTH_2_BYTES};
static const u32 dma_width_4_bytes[] = {DMA_SLAVE_BUSWIDTH_4_BYTES};
static const u32 dma_width_32_bytes[] = {DMA_SLAVE_BUSWIDTH_32_BYTES};
static const u32 dma_width_2_4_bytes[] = {DMA_SLAVE_BUSWIDTH_2_BYTES, DMA_SLAVE_BUSWIDTH_4_BYTES};
static const u32 dma_width_1_4_bytes[] = {DMA_SLAVE_BUSWIDTH_1_BYTE, DMA_SLAVE_BUSWIDTH_4_BYTES};

static const u32 dma_burst_1[] = {1};
static const u32 dma_burst_4[] = {4};
static const u32 dma_burst_8[] = {8};
static const u32 dma_burst_16[] = {16};
static const u32 dma_burst_1_8[] = {1, 8};
static const u32 dma_burst_1_4_8_16[] = {1, 4, 8, 16};

/*                 ID                 burst                 witdh(byte) */
DMA_SLAVE_DEF(DMA_ID_XPWM0,         dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_XPWM1,         dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_XPWM2,         dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_XPWM3,         dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_I2C0,          dma_burst_1_4_8_16,	    dma_width_2_bytes);
DMA_SLAVE_DEF(DMA_ID_I2C1,          dma_burst_1_4_8_16,	    dma_width_2_bytes);
DMA_SLAVE_DEF(DMA_ID_UART0,         dma_burst_1_4_8_16,	    dma_width_1_byte);
DMA_SLAVE_DEF(DMA_ID_UART1,         dma_burst_1_4_8_16,	    dma_width_1_byte);
DMA_SLAVE_DEF(DMA_ID_CANFD0,        dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_CANFD1,        dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_SPI0,          dma_burst_1_4_8_16,	    dma_width_1_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_PBUS,       dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_CANFD,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_I2C0,       dma_burst_1_4_8_16,	    dma_width_2_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_I2C1,       dma_burst_1_4_8_16,	    dma_width_2_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_I2C2,       dma_burst_1_4_8_16,	    dma_width_2_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_UART0,      dma_burst_1_4_8_16,	    dma_width_1_byte);
DMA_SLAVE_DEF(DMA_ID_AP_UART1,      dma_burst_1_4_8_16,	    dma_width_1_byte);
DMA_SLAVE_DEF(DMA_ID_AP_UART2,      dma_burst_1_4_8_16,	    dma_width_1_byte);
DMA_SLAVE_DEF(DMA_ID_AP_UART3,      dma_burst_1_4_8_16,	    dma_width_1_byte);
DMA_SLAVE_DEF(DMA_ID_AP_UART4,      dma_burst_1_4_8_16,	    dma_width_1_byte);
DMA_SLAVE_DEF(DMA_ID_AP_UART5,      dma_burst_1_4_8_16,	    dma_width_1_byte);
DMA_SLAVE_DEF(DMA_ID_AP_UART6,      dma_burst_1_4_8_16,	    dma_width_1_byte);
DMA_SLAVE_DEF(DMA_ID_AP_UART7,      dma_burst_1_4_8_16,	    dma_width_1_byte);
DMA_SLAVE_DEF(DMA_ID_AP_XSPI,       dma_burst_1_4_8_16,	    dma_width_1_byte);
DMA_SLAVE_DEF(DMA_ID_AP_SPI0,       dma_burst_1_4_8_16,	    dma_width_1_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_SPI1,       dma_burst_1_4_8_16,	    dma_width_1_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_SPI2,       dma_burst_1_4_8_16,	    dma_width_1_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_SPI3,       dma_burst_1_4_8_16,	    dma_width_1_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_XPWM0,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_XPWM1,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_XPWM2,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_XPWM3,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_EPWM0,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_EPWM1,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_EPWM2,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_EPWM3,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_EPWM4,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_EPWM5,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_EPWM6,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_EPWM7,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_EPWM8,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_EPWM9,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_EPWM10,     dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_EPWM11,     dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI0,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI1,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI2,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI3,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI4,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI5,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI6,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI7,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI8,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI9,      dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI10,     dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI11,     dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI12,     dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI13,     dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI14,     dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI15,     dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI16,     dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI17,     dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI18,     dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI19,     dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI20,     dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI21,     dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI22,     dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_GPAI23,     dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_CODEC,      dma_burst_1_4_8_16,	    dma_width_2_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_ADC,        dma_burst_1_4_8_16,	    dma_width_2_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_I2S0,       dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_I2S1,       dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_I2S2,       dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_SAI0,       dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_SAI1,       dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_SAI2,       dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_SAI3,       dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_CAP0,       dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_CAP1,       dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_CAP2,       dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_CAP3,       dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_CAP4,       dma_burst_1_4_8_16,	    dma_width_4_bytes);
DMA_SLAVE_DEF(DMA_ID_AP_CAP5,       dma_burst_1_4_8_16,	    dma_width_4_bytes);

static const struct dma_slave_table *aic_dma_slave_table[AIC_DMA_PORTS] = {
    AIC_DMA_CFG(DMA_ID_XPWM0),
    AIC_DMA_CFG(DMA_ID_XPWM1),
    AIC_DMA_CFG(DMA_ID_XPWM2),
    AIC_DMA_CFG(DMA_ID_XPWM3),
    AIC_DMA_CFG(DMA_ID_I2C0),
    AIC_DMA_CFG(DMA_ID_I2C1),
    AIC_DMA_CFG(DMA_ID_UART0),
    AIC_DMA_CFG(DMA_ID_UART1),
    AIC_DMA_CFG(DMA_ID_CANFD0),
    AIC_DMA_CFG(DMA_ID_CANFD1),
    AIC_DMA_CFG(DMA_ID_SPI0),
    AIC_DMA_CFG(DMA_ID_AP_PBUS),
    AIC_DMA_CFG(DMA_ID_AP_CANFD),
    AIC_DMA_CFG(DMA_ID_AP_I2C0),
    AIC_DMA_CFG(DMA_ID_AP_I2C1),
    AIC_DMA_CFG(DMA_ID_AP_I2C2),
    AIC_DMA_CFG(DMA_ID_AP_UART0),
    AIC_DMA_CFG(DMA_ID_AP_UART1),
    AIC_DMA_CFG(DMA_ID_AP_UART2),
    AIC_DMA_CFG(DMA_ID_AP_UART3),
    AIC_DMA_CFG(DMA_ID_AP_UART4),
    AIC_DMA_CFG(DMA_ID_AP_UART5),
    AIC_DMA_CFG(DMA_ID_AP_UART6),
    AIC_DMA_CFG(DMA_ID_AP_UART7),
    AIC_DMA_CFG(DMA_ID_AP_XPWM0),
    AIC_DMA_CFG(DMA_ID_AP_XPWM1),
    AIC_DMA_CFG(DMA_ID_AP_XPWM2),
    AIC_DMA_CFG(DMA_ID_AP_XPWM3),
    AIC_DMA_CFG(DMA_ID_AP_EPWM0),
    AIC_DMA_CFG(DMA_ID_AP_EPWM1),
    AIC_DMA_CFG(DMA_ID_AP_EPWM2),
    AIC_DMA_CFG(DMA_ID_AP_EPWM3),
    AIC_DMA_CFG(DMA_ID_AP_EPWM4),
    AIC_DMA_CFG(DMA_ID_AP_EPWM5),
    AIC_DMA_CFG(DMA_ID_AP_EPWM6),
    AIC_DMA_CFG(DMA_ID_AP_EPWM7),
    AIC_DMA_CFG(DMA_ID_AP_EPWM8),
    AIC_DMA_CFG(DMA_ID_AP_EPWM9),
    AIC_DMA_CFG(DMA_ID_AP_EPWM10),
    AIC_DMA_CFG(DMA_ID_AP_EPWM11),
    AIC_DMA_CFG(DMA_ID_AP_GPAI0),
    AIC_DMA_CFG(DMA_ID_AP_GPAI1),
    AIC_DMA_CFG(DMA_ID_AP_GPAI2),
    AIC_DMA_CFG(DMA_ID_AP_GPAI3),
    AIC_DMA_CFG(DMA_ID_AP_GPAI4),
    AIC_DMA_CFG(DMA_ID_AP_GPAI5),
    AIC_DMA_CFG(DMA_ID_AP_GPAI6),
    AIC_DMA_CFG(DMA_ID_AP_GPAI7),
    AIC_DMA_CFG(DMA_ID_AP_GPAI8),
    AIC_DMA_CFG(DMA_ID_AP_GPAI9),
    AIC_DMA_CFG(DMA_ID_AP_GPAI10),
    AIC_DMA_CFG(DMA_ID_AP_GPAI11),
    AIC_DMA_CFG(DMA_ID_AP_GPAI12),
    AIC_DMA_CFG(DMA_ID_AP_GPAI13),
    AIC_DMA_CFG(DMA_ID_AP_GPAI14),
    AIC_DMA_CFG(DMA_ID_AP_GPAI15),
    AIC_DMA_CFG(DMA_ID_AP_GPAI16),
    AIC_DMA_CFG(DMA_ID_AP_GPAI17),
    AIC_DMA_CFG(DMA_ID_AP_GPAI18),
    AIC_DMA_CFG(DMA_ID_AP_GPAI19),
    AIC_DMA_CFG(DMA_ID_AP_GPAI20),
    AIC_DMA_CFG(DMA_ID_AP_GPAI21),
    AIC_DMA_CFG(DMA_ID_AP_GPAI22),
    AIC_DMA_CFG(DMA_ID_AP_GPAI23),
    AIC_DMA_CFG(DMA_ID_AP_CODEC),
    AIC_DMA_CFG(DMA_ID_AP_I2S0),
    AIC_DMA_CFG(DMA_ID_AP_I2S1),
    AIC_DMA_CFG(DMA_ID_AP_I2S2),
    AIC_DMA_CFG(DMA_ID_AP_SAI0),
    AIC_DMA_CFG(DMA_ID_AP_SAI1),
    AIC_DMA_CFG(DMA_ID_AP_SAI2),
    AIC_DMA_CFG(DMA_ID_AP_SAI3),
    AIC_DMA_CFG(DMA_ID_AP_CAP0),
    AIC_DMA_CFG(DMA_ID_AP_CAP1),
    AIC_DMA_CFG(DMA_ID_AP_CAP2),
    AIC_DMA_CFG(DMA_ID_AP_CAP3),
    AIC_DMA_CFG(DMA_ID_AP_CAP4),
    AIC_DMA_CFG(DMA_ID_AP_CAP5),
};

struct aic_dma_dev {
    struct aic_dma_task task[TASK_MAX_NUM];
    s32 inited;
    unsigned long base;
    u32 burst_length; /* burst length capacity */
    u32 addr_widths; /* address width support capacity */
    struct aic_dma_chan dma_chan[AIC_DMA_CH_NUM];
    struct aic_dma_task *freetask;
    const struct dma_slave_table **slave_table;
} __ALIGNED(CACHE_LINE_SIZE);

struct aic_dma_dev *get_aic_dma_dev(void);
void *aic_dma_task_add(struct aic_dma_task *prev,
                        struct aic_dma_task *next,
                        struct aic_dma_chan *chan);
int aic_set_burst(struct dma_slave_config *sconfig,
                    enum dma_transfer_direction direction,
                    u32 *p_cfg);
struct aic_dma_task *aic_dma_task_alloc(void);
void aic_dma_free_desc(struct aic_dma_chan *chan);

static inline u32 dma_get_bus_width_by_addr(u32 mem_addr)
{
    if (mem_addr >= DRAM_BASE)
        return DMA_SLAVE_BUSWIDTH_8_BYTES;
    else if (mem_addr >= SRAM_BASE)
        return DMA_SLAVE_BUSWIDTH_8_BYTES;
    else
        return DMA_MAX_BUSWIDTH;
}

#endif /*_ARTINCHIP_HAL_DMA_REG_V2X_H_  */
