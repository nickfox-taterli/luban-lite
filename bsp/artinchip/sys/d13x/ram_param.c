/*
 * Copyright (C) 2024 ArtInChip Technology Co.,Ltd
 * Author: Xiong Hao <hao.xiong@artinchip.com>
 */

#include <stdio.h>
#include <aic_core.h>
#include <aic_common.h>
#include <ram_param.h>

#define PSRAM_SINGLE   0
#define PSRAM_PARALLEL 1

enum PSRAM_FUSE_ID {
    APS3208K = 0x00,
    SCKW18_12816O = 0x01,
    APS12816O = 0x02,
};

struct _psram_id {
    u8 psram_fuse_id;
    u32 psram_chip_id;
};

struct _psram_info {
    u8 mark_id;
    u8 psram_num;
    u32 psram_size;
    struct _psram_id psram_id;
};

#define PSRAM_TABLE_INFO                                            \
{                                                                   \
    /* force use the cfg0 */                                        \
    {0x0, 0, 0, {0, 0}},                                            \
    /* D131BAS 4M */                                                \
    {0x01, PSRAM_SINGLE, 0x400000, {APS3208K, 0x80c980c9}},         \
    /* D131BBS 8M */                                                \
    {0x02, PSRAM_PARALLEL, 0x800000, {APS3208K, 0x80c980c9}},       \
    /* D131CBS 8M */                                                \
    {0x03, PSRAM_PARALLEL, 0x800000, {APS3208K, 0x80c980c9}},       \
    /* D131EBS 8M */                                                \
    {0x04, PSRAM_PARALLEL, 0x800000, {APS3208K, 0x80c980c9}},       \
    /* D132ENS 8M */                                                \
    {0x05, PSRAM_PARALLEL, 0x800000, {APS3208K, 0x80c980c9}},       \
    /* D131CCS1 16M SCKW18X128160AAE1 */                            \
    {0x06, PSRAM_PARALLEL, 0x1000000, {SCKW18_12816O, 0xc59ac59a}}, \
    /* D131CCS2 16M AP12816*/                                       \
    {0x06, PSRAM_PARALLEL, 0x1000000, {APS12816O, 0xdd8ddd8d}},     \
    /* D133ECS1 16M SCKW18X128160AAE1*/                             \
    {0x07, PSRAM_PARALLEL, 0x1000000, {SCKW18_12816O, 0xc59ac59a}}, \
    /* D133ECS2 16M AP12816*/                                       \
    {0x07, PSRAM_PARALLEL, 0x1000000, {APS12816O, 0xdd8ddd8d}},     \
    /* M6801SPDS NON PSRAM*/                                        \
    {0x31, 0, 0, {0, 0}},                                           \
    /* M6806SPES NON PSRAM*/                                        \
    {0x32, 0, 0, {0, 0}},                                           \
}

struct _psram_info psram_table_info[] = PSRAM_TABLE_INFO;

u8 psram_get_mark_id(void)
{
    u32 fuse_218 = readl(0x19010218);
    u8 mark_id = fuse_218 & 0xff;

    pr_info("fuse_218(0x19010218)=0x%x, mark_id=0x%x\n", fuse_218, mark_id);
    return mark_id;
}

u8 psram_get_psram_id(void)
{
    u32 fuse_224 = readl(0x19010224);
    u8 psram_id = (fuse_224 & 0xff0000) >> 20;

    pr_info("fuse_224(0x19010224)=0x%x, psram_id=0x%x\n", fuse_224, psram_id);
    return psram_id;
}

struct _psram_info *psram_get_info(u8 mark_id, u8 psram_fuse_id)
{
    u32 len = ARRAY_SIZE(psram_table_info);

    for (int i = 0; i < len; i++) {
        if (((mark_id == psram_table_info[i].mark_id) &&
             (psram_fuse_id == psram_table_info[i].psram_id.psram_fuse_id))) {
            return &psram_table_info[i];
        }
    }
    pr_info("can't get the psram table, return the default info.\n");
    return &psram_table_info[0]; //if not find anyone, return the default info.
}

u32 aic_get_ram_size(void)
{
    struct _psram_info *psram_info;
    u8 mark_id, psram_id;

    mark_id = psram_get_mark_id();
    psram_id = psram_get_psram_id();
    psram_info = psram_get_info(mark_id, psram_id);

    return psram_info->psram_size;
}
