/*
* Copyright (C) 2020-2023 ArtInChip Technology Co. Ltd
*
*  author: <jun.ma@artinchip.com>
*  Desc: aic_parser
*/

#include <string.h>

#include "aic_mov_parser.h"
#include "aic_raw_parser.h"
#include "aic_mp3_parser.h"
#include "aic_wav_parser.h"
#ifdef AIC_MPP_AVI_DEMUX
#include "aic_avi_parser.h"
#endif


struct aic_parser_create_tbl {
	char  file_type[7];
	unsigned char len;
	s32 (*parser)(unsigned char *uri, struct aic_parser **parser);
};

struct aic_parser_create_tbl create_tbl[] = {
	{"mov", 3, aic_mov_parser_create},
	{"mp4", 3, aic_mov_parser_create},
	{"264", 3, aic_raw_parser_create},
	{"mp3", 3, aic_mp3_parser_create},
	{"wav", 3, aic_wav_parser_create},
#ifdef AIC_MPP_AVI_DEMUX
	{"avi", 3, aic_avi_parser_create},
#endif
};
s32 aic_parser_create(unsigned char *uri, struct aic_parser **parser)
{
	int i = 0;
	char* ptr = NULL;
	int size = 0;

	if (uri == NULL) {
		return -1;
	}

	ptr = strrchr((char *)uri, '.');
	if (ptr == NULL) {
		return -1;
	}

	printf("parser for (%s)\n", uri);

	size = sizeof(create_tbl)/sizeof(struct aic_parser_create_tbl);

	for (i = 0; i < size; i++) {
		if (!strncmp(ptr+1, create_tbl[i].file_type, create_tbl[i].len)) {
			return create_tbl[i].parser(uri, parser);
		}
	}

	logw("unkown parser for (%s)", uri);
	return -1;
}
