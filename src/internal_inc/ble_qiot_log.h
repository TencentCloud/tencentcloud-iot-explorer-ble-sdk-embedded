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
#ifndef QCLOUD_BLE_QIOT_LOG_H
#define QCLOUD_BLE_QIOT_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ble_qiot_config.h"

#ifndef BLE_QIOT_DEBUG_PRINT
#define BLE_QIOT_DEBUG_PRINT
#endif

#ifndef BLE_QIOT_INFO_PRINT
#define BLE_QIOT_INFO_PRINT
#endif

#ifndef BLE_QIOT_WARN_PRINT
#define BLE_QIOT_WARN_PRINT
#endif

#ifndef BLE_QIOT_ERROR_PRINT
#define BLE_QIOT_ERROR_PRINT
#endif

#ifndef BLE_QIOT_HEX_PRINT
#define BLE_QIOT_HEX_PRINT
#endif

#ifndef ble_qiot_log_d
#define ble_qiot_log_d(...) BLE_QIOT_DEBUG_PRINT(__VA_ARGS__)
#endif
#ifndef ble_qiot_log_i
#define ble_qiot_log_i(...) BLE_QIOT_INFO_PRINT(__VA_ARGS__)
#endif
#ifndef ble_qiot_log_w
#define ble_qiot_log_w(...) BLE_QIOT_WARN_PRINT(__VA_ARGS__)
#endif
#ifndef ble_qiot_log_e
#define ble_qiot_log_e(...) BLE_QIOT_ERROR_PRINT(__VA_ARGS__)
#endif

#ifndef ble_qiot_log_hex
void HexDump(const char *hex_name, const char *data, uint32_t data_len);
#define ble_qiot_log_hex(hex_name, data, data_len) BLE_QIOT_HEX_PRINT(hex_name, data, data_len)
#endif

#ifdef __cplusplus
}
#endif
#endif  // QCLOUD_BLE_QIOT_LOG_H
