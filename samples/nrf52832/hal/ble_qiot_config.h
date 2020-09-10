/*
 * Copyright (C) 2019 THL A29 Limited, a Tencent company. All rights reserved.
 * Licensed under the MIT License (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://opensource.org/licenses/MIT
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef QCLOUD_BLE_QIOT_CONFIG_H
#define QCLOUD_BLE_QIOT_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

#define BLE_QIOT_SDK_VERSION "1.0.0"  // sdk version

#define BLE_QIOT_SDK_DEBUG 1        // sdk debug switch

#define BLE_QIOT_BUTTON_BROADCAST 1                     // unbind broadcast must be triggered by button if 1 is defined
#if (1 == BLE_QIOT_BUTTON_BROADCAST)
#define BLE_QIOT_BIND_TIMEOUT	  (2 * 60 * 1000)	    // bind timeout，unit: ms
#endif

#define __ORDER_LITTLE_ENDIAN__		1234
#define __ORDER_BIG_ENDIAN__		4321
#define __BYTE_ORDER__				__ORDER_LITTLE_ENDIAN__

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


#define BLE_QIOT_DEBUG_PRINT NRF_LOG_DEBUG
#define BLE_QIOT_INFO_PRINT  NRF_LOG_INFO
#define BLE_QIOT_WARN_PRINT  NRF_LOG_WARNING
#define BLE_QIOT_ERROR_PRINT NRF_LOG_ERROR
#define BLE_QIOT_HEX_PRINT(hex_name, data, data_len) \
    do { \
        NRF_LOG_INFO("dump: %s, length %d", hex_name, data_len); \
        NRF_LOG_RAW_HEXDUMP_INFO(data, data_len); \
    } while(0)


// nrf52832xxAA Flash size is 512KB, nrf52832xxAB Flash size is 512KB, be carefol of the address!
#define BLE_QIOT_RECORD_FLASH_ADDR     0x7e000 // qiot data storage address
#define BLE_QIOT_RECORD_FLASH_PAGESIZE 4096    // flash page size, see chip datasheet
#define BLE_QIOT_RECORD_FLASH_PAGENUM  2       // how many pages qiot use


#define APP_BLE_OBSERVER_PRIO           3                                       /**< Application's BLE observer priority. You shouldn't need to modify this value. */
#define APP_BLE_CONN_CFG_TAG            1                                       /**< A tag identifying the SoftDevice BLE configuration. */

#define APP_ADV_INTERVAL                64                                      /**< The advertising interval (in units of 0.625 ms; this value corresponds to 40 ms). */
#define APP_ADV_DURATION                BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED   /**< The advertising time-out (in units of seconds). When set to 0, we will never time out. */


#ifdef __cplusplus
}
#endif

#endif  // QCLOUD_BLE_QIOT_CONFIG_H
