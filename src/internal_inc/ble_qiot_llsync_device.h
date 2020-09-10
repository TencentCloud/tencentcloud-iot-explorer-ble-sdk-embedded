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
#ifndef QCLOUD_BLE_QIOT_LLSYNC_DEVICE_H
#define QCLOUD_BLE_QIOT_LLSYNC_DEVICE_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "ble_qiot_common.h"
#include "ble_qiot_export.h"
#include "ble_qiot_hmac.h"

#define BLE_LOCAL_PSK_LEN           4
#define BLE_BIND_IDENTIFY_STR_LEN   8
#define BLE_EXPIRATION_TIME         60  // timestamp expiration value
#define BLE_UNBIND_REQUEST_STR      "UnbindRequest"
#define BLE_UNBIND_REQUEST_STR_LEN  (sizeof("UnbindRequest") - 1)
#define BLE_UNBIND_RESPONSE         "UnbindResponse"
#define BLE_UNBIND_RESPONSE_STR_LEN (sizeof("UnbindResponse") - 1)

typedef enum {
    E_DEV_MSG_SYNC_TIME = 0,  // sync info before bind
    E_DEV_MSG_CONN_VALID,     // connect request
    E_DEV_MSG_BIND_SUCC,      // inform bind success
    E_DEV_MSG_BIND_FAIL,      // inform bind failed
    E_DEV_MSG_UNBIND,         // unbind request
    E_DEV_MSG_CONN_SUCC,      // inform connect result
    E_DEV_MSG_CONN_FAIL,
    E_DEV_MSG_UNBIND_SUCC,  // inform unbind result
    E_DEV_MSG_UNBIND_FAIL,
    E_DEV_MSG_MSG_BUTT,
} e_dev_info_msg_type;

typedef enum {
    E_BIND_IDLE = 0,   // no bind
    E_BIND_WAIT,       // wait bind, return idle state if no bind in the period
    E_BIND_SUCC,       // bound
    E_BIND_CONNECTED,  // connected
} e_ble_bind_state;

typedef struct ble_device_info_t_ {
    char product_id[BLE_QIOT_PRODUCT_ID_LEN];
    char device_name[BLE_QIOT_DEVICE_NAME_LEN + 1];
    char psk[BLE_QIOT_PSK_LEN];
    char mac[BLE_QIOT_MAC_LEN];
} ble_device_info;

typedef struct ble_core_data_ {
    uint8_t bind_state;
    char    local_psk[BLE_LOCAL_PSK_LEN];
    char    identify_str[BLE_BIND_IDENTIFY_STR_LEN];
} ble_core_data;

// write to uuid FEE1 before bind
typedef struct ble_bind_data_t_ {
    int nonce;
    int timestamp;
} ble_bind_data;

// connect data struct
typedef struct ble_conn_data_t_ {
    int  timestamp;
    char sign_info[SHA1_DIGEST_SIZE];
} ble_conn_data;

// unbind data struct
typedef struct ble_unbind_data_t_ {
    char sign_info[SHA1_DIGEST_SIZE];
} ble_unbind_data;

// read sdk data from flash
ble_qiot_ret_status_t ble_init_flash_data(void);

// set bind state
void ble_bind_state_set(e_ble_bind_state new_state);

// get bind state
e_ble_bind_state ble_bind_state_get(void);

// get connect state
bool ble_is_connected(void);

// get broadcast data
int ble_get_my_broadcast_data(char *out_buf, int buf_len);

// get bind authcode, return authcode length;
// out_buf length must greater than  SHA1_DIGEST_SIZE + BLE_QIOT_DEVICE_NAME_LEN
int ble_bind_get_authcode(const char *bind_data, uint16_t data_len, char *out_buf, uint16_t buf_len);

// write bind result to flash, return 0 is success
ble_qiot_ret_status_t ble_bind_write_result(const char *result, uint16_t len);

// write unbind result to flash, return 0 is success
ble_qiot_ret_status_t ble_unbind_write_result(void);

// get connect authcode, return authcode length;
// out_buf length must greater than  SHA1_DIGEST_SIZE + BLE_QIOT_DEVICE_NAME_LEN
int ble_conn_get_authcode(const char *conn_data, uint16_t data_len, char *out_buf, uint16_t buf_len);

// get connect authcode, return authcode length;
// out_buf length must greater than  SHA1_DIGEST_SIZE + BLE_UNBIND_RESPONSE_STR_LEN
int ble_unbind_get_authcode(const char *unbind_data, uint16_t data_len, char *out_buf, uint16_t buf_len);
#ifdef __cplusplus
}
#endif
#endif  // QCLOUD_BLE_QIOT_LLSYNC_DEVICE_H
