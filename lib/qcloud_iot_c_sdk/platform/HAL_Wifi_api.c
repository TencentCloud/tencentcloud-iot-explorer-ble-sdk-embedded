/*
 * Tencent is pleased to support the open source community by making IoT Hub
 available.
 * Copyright (C) 2018-2020 THL A29 Limited, a Tencent company. All rights
 reserved.

 * Licensed under the MIT License (the "License"); you may not use this file
 except in
 * compliance with the License. You may obtain a copy of the License at
 * http://opensource.org/licenses/MIT

 * Unless required by applicable law or agreed to in writing, software
 distributed under the License is
 * distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 KIND,
 * either express or implied. See the License for the specific language
 governing permissions and
 * limitations under the License.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qcloud_iot_export.h"
#include "qcloud_iot_import.h"

#include "qcloud_wifi_config.h"
#include "ble_qiot_export.h"
#include "ble_qiot_import.h"

bool HAL_Wifi_IsConnected(void)
{
    // TODO, retrun true if you got ip
    return false;
}

int HAL_Wifi_read_err_log(uint32_t offset, void *log, size_t log_size)
{
    Log_i("HAL_Wifi_read_err_log");

    return QCLOUD_RET_SUCCESS;
}

int HAL_Wifi_write_err_log(uint32_t offset, void *log, size_t log_size)
{
    Log_i("HAL_Wifi_write_err_log");

    return QCLOUD_RET_SUCCESS;
}

int HAL_Wifi_clear_err_log(void)
{
    Log_i("HAL_Wifi_clear_err_log");

    return QCLOUD_RET_SUCCESS;
}

ble_qiot_ret_status_t ble_combo_wifi_token_set(const char *token, uint16_t len)
{
    qiot_device_bind_set_token(token);
    return 0;
}

ble_qiot_ret_status_t ble_combo_wifi_log_get(void)
{
    app_send_ble_dev_log();
    app_send_ble_error_log();
    return 0;
}
