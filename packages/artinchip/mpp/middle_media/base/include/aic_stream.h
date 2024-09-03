/*
 * Copyright (C) 2020-2024 ArtInChip Technology Co. Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Author: <jun.ma@artinchip.com>
 * Desc: aic stream
 */


#ifndef __AIC_STREAM_H__
#define __AIC_STREAM_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "mpp_dec_type.h"



struct aic_stream {
	/* read data */
	s64 (*read)(struct aic_stream *stream, void *buf, s64 len);
	/* write data */
	s64 (*write)(struct aic_stream *stream, void *buf, s64 len);
	/* return current file offset */
	s64 (*tell)(struct aic_stream *stream);
	/* close stream */
	s32 (*close)(struct aic_stream *stream);
	/* seek */
	s64 (*seek)(struct aic_stream *stream, s64 offset, s32 whence);
	/* get stream total size */
	s64 (*size)(struct aic_stream *stream);
};

#ifndef AIC_RL16
#   define AIC_RL16(x)                           \
    ((((const uint8_t*)(x))[1] << 8) |          \
      ((const uint8_t*)(x))[0])
#endif

#ifndef AIC_RL32
#   define AIC_RL32(x)                                \
    (((uint32_t)((const uint8_t*)(x))[3] << 24) |    \
               (((const uint8_t*)(x))[2] << 16) |    \
               (((const uint8_t*)(x))[1] <<  8) |    \
                ((const uint8_t*)(x))[0])
#endif

#define aic_stream_read(      \
		   stream,            \
		   buf,				  \
		   len)               \
	    ((struct aic_stream*)stream)->read(stream, buf, len)

#define aic_stream_write(      \
		   stream,            \
		   buf,				  \
		   len)               \
	    ((struct aic_stream*)stream)->write(stream, buf, len)

#define aic_stream_seek(      \
		   stream,            \
		   offset,			  \
		   whence)            \
	    ((struct aic_stream*)stream)->seek(stream, offset, whence)

#define aic_stream_tell(stream)\
	    ((struct aic_stream*)stream)->tell(stream)

#define aic_stream_size(stream)\
	    ((struct aic_stream*)stream)->size(stream)

#define aic_stream_close(stream)\
	    ((struct aic_stream*)stream)->close(stream)

s32 aic_stream_open(char *uri, struct aic_stream **stream, int flags);



int aic_stream_skip(struct aic_stream* s, int len);
void aic_stream_w8(struct aic_stream* s, int b);
void aic_stream_wl64(struct aic_stream* s, uint64_t val);
void aic_stream_wb64(struct aic_stream* s, uint64_t val);
void aic_stream_wl32(struct aic_stream* s, unsigned int val);
void aic_stream_wb32(struct aic_stream* s, unsigned int val);
void aic_stream_wl24(struct aic_stream* s, unsigned int val);
void aic_stream_wb24(struct aic_stream* s, unsigned int val);
void aic_stream_wl16(struct aic_stream* s, unsigned int val);
void aic_stream_wb16(struct aic_stream* s, unsigned int val);

int          aic_stream_r8  (struct aic_stream* s);
unsigned int aic_stream_rl16(struct aic_stream* s);
unsigned int aic_stream_rl24(struct aic_stream* s);
unsigned int aic_stream_rl32(struct aic_stream* s);
uint64_t     aic_stream_rl64(struct aic_stream* s);
unsigned int aic_stream_rb16(struct aic_stream* s);
unsigned int aic_stream_rb24(struct aic_stream* s);
unsigned int aic_stream_rb32(struct aic_stream* s);
uint64_t     aic_stream_rb64(struct aic_stream* s);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif





