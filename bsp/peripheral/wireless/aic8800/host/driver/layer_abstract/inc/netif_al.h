/*
 * Copyright (C) 2018-2024 AICSemi Ltd.
 *
 * All Rights Reserved
 */

#ifndef _NETIF_AL_H_
#define _NETIF_AL_H_

#include "aic_plat_types.h"
#include "netif_def.h"

typedef void (*aic_tx_cb)(uint32_t frame_id, bool acknowledged, void *arg);

uint32_t aic_offsetof(void);

uint8_t* aic_get_mac_address(void);
void aic_set_mac_address(uint8_t *addr);
void *aic_get_vif_mac_addr(void);
net_if_t *aic_get_vif_net_if(void);
void aic_tx_start(net_if_t *net_if, net_buf_tx_t *net_buf, aic_tx_cb cfm_cb, void *cfm_cb_arg);


#endif /* _NETIF_AL_H_ */

