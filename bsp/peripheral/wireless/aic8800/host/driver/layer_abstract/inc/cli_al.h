/*
 * Copyright (C) 2018-2024 AICSemi Ltd.
 *
 * All Rights Reserved
 */

#ifndef _CLI_AL_H_
#define _CLI_AL_H_


typedef void (*aic_clicb)(char *rsp);

void aic_cli_set_cb(aic_clicb cb);
int aic_cli_run_cmd(char *CmdBuffer);

#endif /* _CLI_AL_H_ */

