/*
 * Copyright (C) 2024-2025, ArtInChip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Authors:  Ning Fang <ning.fang@artinchip.com>
 */

#ifndef LV_MPP_DEC_H
#define LV_MPP_DEC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rtconfig.h>
#include "lvgl.h"
#include "mpp_decoder.h"
#include "frame_allocator.h"

#ifndef LV_USE_MPP_DEC
#define LV_USE_MPP_DEC 1
#endif

#ifndef LV_CACHE_IMG_NUM_LIMIT
#define LV_CACHE_IMG_NUM_LIMIT 1
#endif

#ifndef LV_CACHE_IMG_NUM
#define LV_CACHE_IMG_NUM 8
#endif

#ifndef MPP_JPEG_DEC_OUT_SIZE_LIMIT_ENABLE
#define MPP_JPEG_DEC_OUT_SIZE_LIMIT_ENABLE
#endif

#ifndef MPP_JPEG_DEC_MAX_OUT_WIDTH
#define MPP_JPEG_DEC_MAX_OUT_WIDTH  2048
#endif

#ifndef MPP_JPEG_DEC_MAX_OUT_HEIGHT
#define MPP_JPEG_DEC_MAX_OUT_HEIGHT 2048
#endif

#define CHECK_RET(func, ret)  do {\
                            if (func != ret) {\
                                LV_LOG_ERROR("Invalid status");\
                                res = LV_RESULT_INVALID;\
                                goto out;\
                            }\
                         } while (0)

#define CHECK_PTR(ptr)  do {\
                            if (ptr == NULL) {\
                                LV_LOG_ERROR("ptr == NULL");\
                                res = LV_RESULT_INVALID;\
                                goto out;\
                            }\
                        } while (0)

typedef struct {
    lv_draw_buf_t decoded;
    struct mpp_buf dec_buf;
    uint8_t *data[3];
    bool cached;
} mpp_decoder_data_t;

typedef struct {
    lv_mutex_t lock;
    int32_t cache_num;
    int32_t max_num;
} mpp_cache_t;

static inline int jpeg_width_limit(int width)
{
    int r = 0;

    while(width > MPP_JPEG_DEC_MAX_OUT_WIDTH) {
        width = width >> 1;
        r++;

        if (r == 3) {
            break;
        }
    }
    return r;
}

static inline int jpeg_height_limit(int height)
{
    int r = 0;

    while(height > MPP_JPEG_DEC_MAX_OUT_HEIGHT) {
        height = height >> 1;
        r++;

        if (r == 3) {
            break;
        }
    }
    return r;
}

static inline bool image_suffix_is_jpg(char *ptr)
{
    return ((!strcmp(ptr, ".jpg")) || (!strcmp(ptr, ".jpeg"))
            || (!strcmp(ptr, ".JPG")) || (!strcmp(ptr, ".JPEG")));
}

static inline int jpeg_size_limit(int w, int h)
{
    return LV_MAX(jpeg_width_limit(w), jpeg_height_limit(h));
}

lv_color_format_t mpp_fmt_to_lv_fmt(enum mpp_pixel_format cf);

lv_result_t lv_png_decoder_info(const char *src, lv_image_header_t *header, uint32_t size, bool is_file);

lv_result_t lv_jpeg_decoder_info(const char *src, lv_image_header_t *header, uint32_t size, bool is_file);

lv_result_t lv_frame_buf_alloc(mpp_decoder_data_t *mpp_data, struct mpp_buf *alloc_buf,
                              int *size, uint32_t cf);

void lv_frame_buf_free(mpp_decoder_data_t *mpp_data);

void lv_set_frame_buf_size(struct mpp_frame *frame, int *buf_size, int size_shift);

struct frame_allocator* lv_open_allocator(struct mpp_frame* frame);

void lv_mpp_dec_init(void);

void lv_mpp_dec_deinit(void);

bool lv_drop_one_cached_image();

#if LV_CACHE_DEF_SIZE > 0
bool lv_image_cache_check(mpp_cache_t *mpp_cache);
#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif //LV_MPP_DEC_H
