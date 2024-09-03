/*
 * Copyright (c) 2023-2024, ArtInChip Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Wu Dehuang <dehuang.wu@artinchip.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <console.h>
#include <aic_core.h>
#include <aic_common.h>
#include <aic_errno.h>
#include <aic_stdio.h>
#include <boot_param.h>
#include <boot_time.h>
#include <usb_drv.h>
#include <usbhost.h>
#include <uart.h>
#include <config_parse.h>
#include <aicupg.h>
#include <fatfs.h>
#include <mmc.h>
#include <hal_syscfg.h>
#include <upg_uart.h>
#include <userid.h>

#define UPG_TYPE_NONE 0
#define UPG_TYPE_USB 1
#define UPG_TYPE_UART 2
#define UPG_TYPE_SDCARD 3
#define UPG_TYPE_UDISK 4

#define UPG_MODE_NORMAL 0
#define UPG_MODE_USERID 1
#define UPG_MODE_FORCE 2

static int upg_type;
static int upg_mode;
static int usb_host_id;

#define ACK 0x06
#define CHECK_TIME_INTERVAL_US 10000
#if defined(LPKG_USING_USERID)
static void uart_userid_sync(void)
{
    int i = 0;
    char c, *p = "AIBURNID\n";

    /* Send SYN + AIBURNID to Host, if AiBurnID Tool resp in 10ms, then stop
     * booting and go into upgmode
     */
    c = 0x16; /* SYN */
    uart_putchar(0, c);
    for (i = 0; i < 9; i++)
        uart_putchar(0, p[i]);
}

static bool uart_burn_userid_detect(void)
{
    u64 start, cur;
    bool ret = false;
    char c;

    if (AIC_BOOTLOADER_CONSOLE_UART != 0)
        uart_init(0);

    uart_userid_sync();

    start = aic_get_time_us();
    while (1) {
        c = uart_getchar(0);
        cur = aic_get_time_us();
        if (c == ACK) {
            ret = true;
            break;
        }
        if ((cur - start) > CHECK_TIME_INTERVAL_US) {
            ret = false;
            break;
        }
    }

    return ret;
}
#endif

#define RUNCMD console_run_cmd

bool usbd_connect_pc_check(void)
{
#if defined(AICUPG_USB_ENABLE)
    /*
     * If USB is connecting to HOST PC
     */
    usbd_connection_check_start();
    if (usbd_connection_check_status()) {
        usbd_connection_check_end();
        return true;
    }
#endif

    return false;
}

static bool userid_check(void)
{
#if defined(LPKG_USING_USERID)
    /*
     * UserID case:
     * 1. Image already is burned to Flash
     * 2. Boot from normal flash, but need to check in BootLoader
     * 3. If UserID is not locked
     * 4. If USB is connecting to HOST
     * 5. If UART is connecting to HOST, and HOST TOOL is ready
     *
     */
    int ret;
    u32 flag = 0;

    ret = userid_init();
    if (ret)
        return false;

    ret = userid_read("lock", 0, (void *)&flag, 4);
    userid_deinit();
    if (ret > 0 && flag != 0) {
        /* userid is locked, no need to enter userid mode anymore */
        return false;
    }
    /*
     * userid is not locked, need to enter burn userid mode
     * 1. if USB is connecting to PC, use USB to burn userid
     * 2. Otherwise try to check uart mode
     */
    if (usbd_connect_pc_check()) {
        upg_type = UPG_TYPE_USB;
        upg_mode = UPG_MODE_USERID;
        return true;
    }
    if (uart_burn_userid_detect()) {
        upg_type = UPG_TYPE_UART;
        upg_mode = UPG_MODE_USERID;
        return true;
    }
#endif

    return false;
}

int upg_type_check(enum boot_device bd)
{
    enum aic_reboot_reason r;

    upg_type = UPG_TYPE_NONE;
    upg_mode = UPG_MODE_NORMAL;

    r = aic_get_reboot_reason();
    /*
     * if REBOOT_REASON_UPGRADE is not clear in BROM or PBP, should be clear
     * in bootloader, otherwise watchdog reset will reboot failure.
     */
    if (r == REBOOT_REASON_UPGRADE)
        aic_clr_reboot_reason();

    if (r == REBOOT_REASON_BL_UPGRADE) {
        /*
         * User run command "aicupg gotobl" in Application, to enter upgmode of
         * BootLoader
         */
        if (usbd_connect_pc_check()) {
            upg_type = UPG_TYPE_USB;
            upg_mode = UPG_MODE_NORMAL;
        } else {
            upg_type = UPG_TYPE_UART;
            upg_mode = UPG_MODE_NORMAL;
        }
        aic_clr_reboot_reason();
        return 0;
    }

    if (bd == BD_USB) {
        if (usbd_connect_pc_check()) {
            upg_type = UPG_TYPE_USB;
            upg_mode = UPG_MODE_NORMAL;
        } else {
            upg_type = UPG_TYPE_UART;
            upg_mode = UPG_MODE_NORMAL;
        }
        return 0;
    }

    if (bd == BD_SDFAT32) {
        upg_type = UPG_TYPE_SDCARD;
        upg_mode = UPG_MODE_NORMAL;
        return 0;
    }

    if (bd == BD_UDISK) {
        upg_type = UPG_TYPE_UDISK;
        upg_mode = UPG_MODE_NORMAL;
        return 0;
    }

#if defined(AICUPG_UDISK_ENABLE)
    /*
     * D21x udisk upgrade in bootloader
     * D13x maybe BROM not recognize the udisk but bootloader can recognize it
     */
    usb_host_id = usbh_get_connect_id();
    boot_time_trace("UDISK checked");
    if (usb_host_id > 0) {
        upg_type = UPG_TYPE_UDISK;
        upg_mode = UPG_MODE_NORMAL;
        return 0;
    }
#endif

    if (userid_check()) {
        if (usbd_connect_pc_check()) {
            upg_type = UPG_TYPE_USB;
            upg_mode = UPG_MODE_USERID;
        } else {
            upg_type = UPG_TYPE_UART;
            upg_mode = UPG_MODE_USERID;
        }
        return 0;
    }

    return -1;
}

int bl_upgmode_get_type(void)
{
    return upg_type;
}

int bl_upgmode_get_mode(void)
{
    return upg_mode;
}

int bl_upgmode_detect(enum boot_device bd)
{
    int ret = 0, cont_boot = 1;

    ret = upg_type_check(bd);

    if (ret) {
        return cont_boot;
    }

    if (upg_type == UPG_TYPE_NONE) {
        /*
         * No need to run upgmode, just try to boot from storage device
         */
        cont_boot = 1;
        return cont_boot;
    } else {
        if (upg_type == UPG_TYPE_SDCARD) {
            ret = RUNCMD("aicupg fat mmc 1");
            /* SDCARD FAT32 information is from BROM, if upgrading failure, just
             * stop in command line mode
             */
            cont_boot = 0;
        }
        if (upg_type == UPG_TYPE_UDISK) {
            if (usb_host_id == 0)
                ret = RUNCMD("aicupg fat udisk 0");
            else if (usb_host_id == 1)
                ret = RUNCMD("aicupg fat udisk 1");
            if (!ret) {
                /* Upgrading success, no need to run boot from storage anymore */
                cont_boot = 0;
            } else {
                /* Upgrading failed, need to run boot from storage */
                cont_boot = 1;
            }
        }
        if (upg_type == UPG_TYPE_USB) {
            if (upg_mode == UPG_MODE_NORMAL) {
                RUNCMD("aicupg usb 0");
                cont_boot = 0;
            } else if (upg_mode == UPG_MODE_USERID) {
                /* If no host connect to device, skip it and continue to boot
                 * app
                 */
                ret = RUNCMD("aicupg usb 0 userid");
                if (ret)
                    cont_boot = 1;
                else
                    cont_boot = 0;
            }
        }
        if (upg_type == UPG_TYPE_UART) {
            if (upg_mode == UPG_MODE_NORMAL) {
                ret = RUNCMD("aicupg uart 0");
                cont_boot = 0;
            } else if (upg_mode == UPG_MODE_USERID) {
                /* If no host connect to device, skip it and continue to boot
                 * app
                 */
                ret = RUNCMD("aicupg uart 0 userid");
                if (ret)
                    cont_boot = 1;
                else
                    cont_boot = 0;
            }
        }
    }

    return cont_boot;
}
