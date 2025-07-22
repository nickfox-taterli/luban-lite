/*
 * Copyright (C) 2018-2024 AICSemi Ltd.
 *
 * All Rights Reserved
 */

#ifndef _SDIO_AL_H_
#define _SDIO_AL_H_

void aic_sdio_tx_init(void);
void aic_sdio_rx_task(void *argv);
void aic_sdio_buf_init(void);
#ifdef CONFIG_SDIO_BUS_PWRCTRL
int aic_sdio_bus_pwrctrl_preinit(void);
#endif

#endif /* _SDIO_AL_H_ */

