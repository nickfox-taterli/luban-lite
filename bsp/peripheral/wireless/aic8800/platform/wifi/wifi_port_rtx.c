#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <rtthread.h>
#include "wlan_dev.h"
#include "aic_plat_log.h"

#include "lmac_mac.h"
#include "wlan_if.h"
#include "fhost.h"

struct rt_wlan_device * s_wlan_dev = NULL;
struct rt_wlan_device * s_ap_dev = NULL;
rt_err_t aic8800_init(struct rt_wlan_device *wlan)
{
    AIC_LOG_PRINTF("%s, mode=%d\n", __func__, wlan->mode);
    #if 0
    AIC_LOG_PRINTF("%s ctrl pwrkey\n", __func__);
    platform_pwr_wifi_pin_init();
    platform_pwr_wifi_pin_disable();
    rtos_task_suspend(10);
    platform_pwr_wifi_pin_enable();
    #endif

    wifi_if_sdio_init();
    return 0;
}

rt_err_t aic8800_set_mode(struct rt_wlan_device *wlan, rt_wlan_mode_t mode)
{
    AIC_LOG_PRINTF("%s: %d\n", __func__, mode);
    if(mode == RT_WLAN_AP) {

    } else if ( mode == RT_WLAN_STATION){

    }

    return 0;
}

static int wlan_security2aic(rt_wlan_security_t security)
{
    int type = 0;

    switch(security)
    {
    case SECURITY_OPEN:

        break;
    case SECURITY_WEP_PSK:

        break;
    case SECURITY_WEP_SHARED:

        break;
    case SECURITY_WPA_AES_PSK:

        break;
    case SECURITY_WPA2_AES_PSK:

        break;
    case SECURITY_WPA2_MIXED_PSK:

        break;
    default:

        break;
    }
    return type;
}

static rt_wlan_security_t aic8800_akm2security(uint32_t akm, uint16_t group_cipher, uint16_t pairwise_cipher)
{
    rt_wlan_security_t wlan_sec = SECURITY_UNKNOWN;

    if (akm & CO_BIT(MAC_AKM_NONE)) {
        wlan_sec = SECURITY_OPEN;
    } else
    if (akm & CO_BIT(MAC_AKM_PRE_RSN)) {
        if (pairwise_cipher &
            (CO_BIT(MAC_CIPHER_WEP40)
            | CO_BIT(MAC_CIPHER_WEP40))) {
            wlan_sec = SECURITY_WEP_PSK;
        } else
        if (pairwise_cipher & CO_BIT(MAC_CIPHER_TKIP)) {
            wlan_sec = SECURITY_WPA_TKIP_PSK;
        } else
        if (pairwise_cipher &
            (CO_BIT(MAC_CIPHER_CCMP)
            | CO_BIT(MAC_CIPHER_CCMP_256))) {
            wlan_sec = SECURITY_WPA_AES_PSK;
        }
    } else
    if (akm & CO_BIT(MAC_AKM_PSK)) {
        if ((pairwise_cipher &
            (CO_BIT(MAC_CIPHER_TKIP) | CO_BIT(MAC_CIPHER_CCMP))) ==
            (CO_BIT(MAC_CIPHER_TKIP) | CO_BIT(MAC_CIPHER_CCMP))) {
            wlan_sec = SECURITY_WPA2_MIXED_PSK;
        } else
        if (pairwise_cipher & CO_BIT(MAC_CIPHER_TKIP)) {
            wlan_sec = SECURITY_WPA2_TKIP_PSK;
        } else
        if (pairwise_cipher &
            (CO_BIT(MAC_CIPHER_CCMP)
            | CO_BIT(MAC_CIPHER_CCMP_256))) {
            wlan_sec = SECURITY_WPA2_AES_PSK;
        }
    }

    //AIC_LOG_PRINTF("akm=%x,g=%d,p=%d,wlan_sec=0x%X\n", akm, group_cipher, pairwise_cipher, wlan_sec);

    return wlan_sec;
}

void aic8800_scan_result_callback(struct mac_scan_result *result)
{
    struct rt_wlan_buff buff;
    struct rt_wlan_info scan_node = {0,};
    struct rt_wlan_info *scan_list = &scan_node;
    int fhost_band = result->chan->band;
    scan_list->band = (fhost_band == PHY_BAND_2G4) ? RT_802_11_BAND_2_4GHZ : RT_802_11_BAND_5GHZ;
    scan_list->channel  = phy_freq_to_channel(fhost_band, result->chan->freq);
    scan_list->rssi     = result->rssi;
    scan_list->security = aic8800_akm2security(result->akm, result->group_cipher, result->pairwise_cipher);
    memcpy((char *)scan_list->bssid, &result->bssid.array[0], RT_WLAN_BSSID_MAX_LENGTH);
    scan_list->ssid.len = result->ssid.length;
    memcpy(scan_list->ssid.val, result->ssid.array, RT_WLAN_SSID_MAX_LENGTH);
    buff.len = sizeof(struct rt_wlan_info);
    buff.data = (void *)scan_list;
    rt_wlan_dev_indicate_event_handle(s_wlan_dev, RT_WLAN_DEV_EVT_SCAN_REPORT, &buff);
}

rt_err_t aic8800_scan(struct rt_wlan_device *wlan, struct rt_scan_info *scan_info)
{
    AIC_LOG_PRINTF("%s\n", __func__);
    if (scan_info){
        AIC_LOG_PRINTF("scan ssid:%s, len:%d\n", scan_info->ssid.val, scan_info->ssid.len);
        AIC_LOG_PRINTF("bssid=%02X:%02X:%02X:%02X:%02X:%02X\n",
            scan_info->bssid[0], scan_info->bssid[1], scan_info->bssid[2],
            scan_info->bssid[3], scan_info->bssid[4], scan_info->bssid[5]);
        AIC_LOG_PRINTF("channel:%d->%d, passive=%d\n", scan_info->channel_min, scan_info->channel_max, scan_info->passive);
    }
    wlan_if_scan(aic8800_scan_result_callback);
    rt_wlan_dev_indicate_event_handle(s_wlan_dev, RT_WLAN_DEV_EVT_SCAN_DONE, NULL);
    return 0;
}

void aic8800_mac_status_get_callback(wifi_mac_status_e st)
{
    AIC_LOG_PRINTF("%s st=%d\n", __func__, st);
    if (st == WIFI_MAC_STATUS_CONNECTED) {
        rt_wlan_dev_indicate_event_handle(s_wlan_dev, RT_WLAN_DEV_EVT_CONNECT, NULL);
    } else if (st == WIFI_MAC_STATUS_DISCONNECTED) {
        rt_wlan_dev_indicate_event_handle(s_wlan_dev, RT_WLAN_DEV_EVT_DISCONNECT, NULL);
    }
}

rt_err_t aic8800_join(struct rt_wlan_device *wlan, struct rt_sta_info *sta_info)
{
    int ret;
    rt_uint8_t ssid_str[RT_WLAN_SSID_MAX_LENGTH + 1];
    rt_uint8_t pass_str[RT_WLAN_PASSWORD_MAX_LENGTH + 1];
    memcpy(ssid_str, sta_info->ssid.val, sta_info->ssid.len);
    ssid_str[sta_info->ssid.len] = '\0';
    memcpy(pass_str, sta_info->key.val, sta_info->key.len);
    pass_str[sta_info->key.len] = '\0';
    AIC_LOG_PRINTF("%s ssid: %s\n", __func__, ssid_str);
    fhost_get_mac_status_register(aic8800_mac_status_get_callback);
    ret = wlan_start_sta(ssid_str, pass_str, 0);
    AIC_LOG_PRINTF("wlan_start_sta ret=%d\n", ret);
    return 0;
}


rt_err_t aic8800_softap(struct rt_wlan_device *wlan, struct rt_ap_info *ap_info)
{
    int ret, str_len;
    struct aic_ap_cfg ap_cfg = {{"\0",},};
    AIC_LOG_PRINTF("%s\n", __func__);
    if (ap_info->ssid.len <= AIC_MAX_SSID_LEN) {
        str_len = ap_info->ssid.len;
    } else {
        str_len = AIC_MAX_SSID_LEN;
    }
    strncpy(ap_cfg.aic_ap_ssid.array, (char *)ap_info->ssid.val, str_len);
    ap_cfg.aic_ap_ssid.array[str_len] = '\0';
    ap_cfg.aic_ap_ssid.length = str_len;
    if (ap_info->key.len <= AIC_MAX_PASSWD_LEN) {
        str_len = ap_info->key.len;
    } else {
        str_len = AIC_MAX_PASSWD_LEN;
    }
    strncpy(ap_cfg.aic_ap_passwd.array, (char *)ap_info->key.val, str_len);
    ap_cfg.aic_ap_passwd.array[str_len] = '\0';
    ap_cfg.aic_ap_passwd.length = str_len;
    ap_cfg.bcn_interval = 100;
    ap_cfg.hidden_ssid = ap_info->hidden;
    ap_cfg.band = (ap_info->channel >= 36) ? PHY_BAND_5G : PHY_BAND_2G4;
    ap_cfg.type = PHY_CHNL_BW_20;
    ap_cfg.channel = ap_info->channel;
    ap_cfg.enable_he = 1;
    ap_cfg.enable_acs = 0;
    ap_cfg.sercurity_type = (ap_info->key.len == 0) ? KEY_NONE : KEY_WPA2;
    ap_cfg.dtim = 1;
    ap_cfg.sta_num = 32;
    AIC_LOG_PRINTF("Start ap ssid: %s\n", ap_cfg.aic_ap_ssid.array);
    ret = aic_wifi_start_ap(&ap_cfg);
    AIC_LOG_PRINTF("wlan_start_ap ret=%d\n", ret);
    if (ret >= 0) {
        rt_wlan_dev_indicate_event_handle(s_ap_dev, RT_WLAN_DEV_EVT_AP_START, NULL);
    }

    return 0;
}

rt_err_t aic8800_disconnect(struct rt_wlan_device *wlan)
{
    int fhost_vif_idx = 0;
    AIC_LOG_PRINTF("%s\n", __func__);
    wlan_disconnect_sta(fhost_vif_idx);
    rt_wlan_dev_indicate_event_handle(s_wlan_dev, RT_WLAN_DEV_EVT_DISCONNECT, NULL);
    return 0;
}

rt_err_t aic8800_ap_stop(struct rt_wlan_device *wlan)
{
    int ret;
    AIC_LOG_PRINTF("%s\n", __func__);
    ret = aic_wifi_stop_ap();
    if (ret == 0) {
        rt_wlan_dev_indicate_event_handle(s_ap_dev, RT_WLAN_DEV_EVT_AP_STOP, NULL);
    } else {
        AIC_LOG_PRINTF("stop_ap ret=%d\n", ret);
    }

	return 0;
}

rt_err_t aic8800_ap_deauth(struct rt_wlan_device *wlan, unsigned char mac[])
{
    AIC_LOG_PRINTF("%s\n", __func__);
    return -1;
}

rt_err_t aic8800_scan_stop(struct rt_wlan_device *wlan)
{
    AIC_LOG_PRINTF("%s\n", __func__);
    return -1;
}


int aic8800_get_rssi(struct rt_wlan_device *wlan)
{
    AIC_LOG_PRINTF("%s\n", __func__);
    return -1;
}

rt_err_t aic8800_set_channel(struct rt_wlan_device *wlan, int channel)
{
    AIC_LOG_PRINTF("%s\n", __func__);
    return -1;
}

int aic8800_get_channel(struct rt_wlan_device *wlan)
{
    AIC_LOG_PRINTF("%s\n", __func__);
    return -1;
}

rt_err_t aic8800_set_country(struct rt_wlan_device *wlan, rt_country_code_t country_code)
{
    AIC_LOG_PRINTF("%s\n", __func__);
    return -1;
}

rt_country_code_t aic8800_get_country(struct rt_wlan_device *wlan)
{
    AIC_LOG_PRINTF("%s\n", __func__);
    return -1;
}

rt_err_t aic8800_set_mac(struct rt_wlan_device *wlan, rt_uint8_t *mac)
{
    AIC_LOG_PRINTF("%s %02X:%02X:%02X:%02X:%02X:%02X\n", __func__, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return -1;
}

rt_err_t aic8800_get_mac(struct rt_wlan_device *wlan, rt_uint8_t *mac)
{
    AIC_LOG_PRINTF("%s\n", __func__);
    if (mac) {
        int fhost_vif_idx = 0, ret;
        ret = aic_wifi_fvif_mac_address_get(fhost_vif_idx, mac);
        AIC_LOG_PRINTF("ret=%d, mac=%02X:%02X:%02X:%02X:%02X:%02X\n", ret, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }
    return 0;
}

void aic8800_wifi_input(void *buf, int len, int if_id)
{
    struct rt_wlan_device *wlan_dev=NULL;
    wlan_dev=if_id ? s_ap_dev:s_wlan_dev;
    AIC_LOG_PRINTF("%s\n", __func__);

    if (rt_wlan_dev_report_data(wlan_dev, buf, len)){
        AIC_LOG_PRINTF("if[%d] input pkt fail\n",if_id);
    }
}

int aic8800_recv(struct rt_wlan_device *wlan, void *buff, int len)
{
    AIC_LOG_PRINTF("%s\n", __func__);
    return -1;
}

int aic8800_send(struct rt_wlan_device *wlan, void *buff, int len)
{
    //AIC_LOG_PRINTF("%s\n", __func__);
    if (buff) {
        int fhost_vif_idx = 0;
        if (aic_wifi_fvif_active_get(fhost_vif_idx) == 0) {
            AIC_LOG_PRINTF("sta disconnect or ap stop, drop pkt\n");
        } else {
            //AIC_LOG_PRINTF("buff=%p, len=%d\n", buff, len);
            //hexdump(buff, len + 8, 1);
            tx_eth_data_process(buff, len, NULL);
        }
    }
    return 0;
}

static const struct rt_wlan_dev_ops wlan_ops = {
    .wlan_init = aic8800_init,
    .wlan_mode = aic8800_set_mode,
    .wlan_scan = aic8800_scan,
    .wlan_join = aic8800_join,
    .wlan_softap = aic8800_softap,
    .wlan_disconnect = aic8800_disconnect,
    .wlan_ap_stop = aic8800_ap_stop,
    .wlan_ap_deauth = aic8800_ap_deauth,
    .wlan_scan_stop = aic8800_scan_stop,
    .wlan_get_rssi = aic8800_get_rssi,
    .wlan_set_channel = aic8800_set_channel,
    .wlan_get_channel = aic8800_get_channel,
    .wlan_set_country = aic8800_set_country,
    .wlan_get_country = aic8800_get_country,
    .wlan_set_mac = aic8800_set_mac,
    .wlan_get_mac = aic8800_get_mac,
    .wlan_recv = aic8800_recv,
    .wlan_send = aic8800_send,
};


int wifi_device_reg(void)
{
    AIC_LOG_PRINTF("%s ctrl pwrkey\n", __func__);
    platform_pwr_wifi_pin_init();
    platform_pwr_wifi_pin_disable();
    rtos_task_suspend(10);
    platform_pwr_wifi_pin_enable();

    s_wlan_dev = rt_malloc(sizeof(struct rt_wlan_device));
    if (!s_wlan_dev){
        rt_kprintf("wlan0 devcie malloc fail!\n");
        return -1;
    }
    rt_wlan_dev_register(s_wlan_dev, "wlan0", &wlan_ops, RT_WLAN_FLAG_STA_ONLY, NULL);

    s_ap_dev = rt_malloc(sizeof(struct rt_wlan_device));
    if (!s_ap_dev){
        rt_kprintf("ap0 devcie malloc fail!");
        return -1;
    }
    rt_wlan_dev_register(s_ap_dev, "ap0", &wlan_ops, RT_WLAN_FLAG_AP_ONLY, NULL);

    return 0;
}

INIT_COMPONENT_EXPORT(wifi_device_reg);
