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
#define BLE_QIOT_SDK_DEBUG   0        // sdk debug switch

// the device broadcast is controlled by the user, but we provide a mechanism to help the device save more power.
// if you want broadcast is triggered by something like press a button instead of all the time, and the broadcast
// stopped automatically in a few minutes if the device is not bind, define BLE_QIOT_BUTTON_BROADCAST is 1 and
// BLE_QIOT_BIND_TIMEOUT is the period that broadcast stopped.
// if the device in the bound state, broadcast dose not stop automatically.
#define BLE_QIOT_BUTTON_BROADCAST 1
#if (1 == BLE_QIOT_BUTTON_BROADCAST)
#define BLE_QIOT_BIND_TIMEOUT (2 * 60 * 1000)  // unit: ms
#endif

// some data like integer need to be transmitted in a certain byte order, defined it according to your device
#define __ORDER_LITTLE_ENDIAN__ 1234
#define __ORDER_BIG_ENDIAN__    4321
#define __BYTE_ORDER__          __ORDER_LITTLE_ENDIAN__

// some sdk info needs to be output to the standard output of the device, provide functions if you need the sdk output
#define BLE_QIOT_DEBUG_PRINT                         printf                             // debug
#define BLE_QIOT_INFO_PRINT                          printf                             // info
#define BLE_QIOT_WARN_PRINT                          printf                             // warning
#define BLE_QIOT_ERROR_PRINT                         printf                             // error
#define BLE_QIOT_HEX_PRINT(hex_name, data, data_len) HexDump(hex_name, data, data_len)  // output hex

// some sdk info needs to stored on the device and the address is up to you
#define BLE_QIOT_RECORD_FLASH_ADDR 0x3f000

#ifdef __cplusplus
}
#endif

#endif  // QCLOUD_BLE_QIOT_CONFIG_H
