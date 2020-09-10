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
#ifdef __cplusplus
extern "C" {
#endif

#include "ble_qiot_service.h"

#include <stdio.h>

#include "ble_qiot_export.h"
#include "ble_qiot_import.h"
#include "ble_qiot_llsync_data.h"
#include "ble_qiot_llsync_device.h"
#include "ble_qiot_llsync_event.h"
#include "ble_qiot_log.h"
#include "ble_qiot_param_check.h"
#include "ble_qiot_service.h"
#include "ble_qiot_template.h"

#if (1 == BLE_QIOT_BUTTON_BROADCAST)
static ble_timer_t sg_bind_timer = NULL;
#endif

static qiot_service_init_s service_info = {
    .service_uuid16  = IOT_BLE_UUID_SERVICE,
    .service_uuid128 = IOT_BLE_UUID_BASE,
    .gatt_max_mtu    = BLE_GATT_MAX_MTU_SIZE,
    .device_info =
        {
            .uuid16          = IOT_BLE_UUID_DEVICE_INFO,
            .gatt_char_props = GATT_CHAR_READ | GATT_CHAR_WRITE,
            .on_write        = ble_device_info_write_cb,
        },
    .data =
        {
            .uuid16          = IOT_BLE_UUID_DATA,
            .gatt_char_props = GATT_CHAR_READ | GATT_CHAR_WRITE,
            .on_write        = ble_lldata_write_cb,
        },
    .event =
        {
            .uuid16          = IOT_BLE_UUID_EVENT,
            .gatt_char_props = GATT_CHAR_READ | GATT_CHAR_NOTIFY,
            .on_write        = NULL,
        },
};

const qiot_service_init_s *ble_get_qiot_services(void)
{
    return &service_info;
}

#if (1 == BLE_QIOT_BUTTON_BROADCAST)
static void ble_bind_timer_callback(void *param)
{
    ble_qiot_log_i("timer timeout");
    if (E_BIND_WAIT == ble_bind_state_get()) {
        ble_advertising_stop();
        ble_bind_state_set(E_BIND_IDLE);
        ble_qiot_log_i("stop advertising");
    }
}
#endif

ble_qiot_ret_status_t ble_qiot_advertising_start(void)
{
    adv_info my_adv_info;
    uint8_t  adv_data[32] = {0};
    uint8_t  adv_data_len = 0;

    if (E_BIND_IDLE == ble_bind_state_get()) {
#if (1 == BLE_QIOT_BUTTON_BROADCAST)
        if (NULL == sg_bind_timer) {
            sg_bind_timer = ble_timer_create(BLE_TIMER_ONE_SHOT_TYPE, ble_bind_timer_callback);
            if (NULL == sg_bind_timer) {
                ble_qiot_log_i("create bind timer failed");
                return BLE_QIOT_RS_ERR;
            }
        }
#endif

        ble_advertising_stop();

        ble_bind_state_set(E_BIND_WAIT);
        adv_data_len                   = ble_get_my_broadcast_data((char *)adv_data, sizeof(adv_data));
        my_adv_info.company_identifier = TENCENT_COMPANY_IDENTIFIER;
        my_adv_info.adv_data           = adv_data;
        my_adv_info.adv_data_len       = adv_data_len;
        ble_advertising_start(&my_adv_info);
        ble_qiot_log_i("start wait advertising");

#if (1 == BLE_QIOT_BUTTON_BROADCAST)
        ble_timer_start(sg_bind_timer, BLE_QIOT_BIND_TIMEOUT);
#endif
    } else if (E_BIND_WAIT == ble_bind_state_get()) {
        ble_advertising_stop();
        ble_bind_state_set(E_BIND_WAIT);
        adv_data_len                   = ble_get_my_broadcast_data((char *)adv_data, sizeof(adv_data));
        my_adv_info.company_identifier = TENCENT_COMPANY_IDENTIFIER;
        my_adv_info.adv_data           = adv_data;
        my_adv_info.adv_data_len       = adv_data_len;
        ble_advertising_start(&my_adv_info);
        ble_qiot_log_i("restart wait advertising");

#if (1 == BLE_QIOT_BUTTON_BROADCAST)
        ble_timer_stop(sg_bind_timer);
        ble_timer_start(sg_bind_timer, BLE_QIOT_BIND_TIMEOUT);
#endif
    } else if (E_BIND_SUCC == ble_bind_state_get()) {
        ble_advertising_stop();
        adv_data_len                   = ble_get_my_broadcast_data((char *)adv_data, sizeof(adv_data));
        my_adv_info.company_identifier = TENCENT_COMPANY_IDENTIFIER;
        my_adv_info.adv_data           = adv_data;
        my_adv_info.adv_data_len       = adv_data_len;
        ble_advertising_start(&my_adv_info);
        ble_qiot_log_i("start bind advertising");
    } else {
        // do nothing
    }

    return BLE_QIOT_RS_OK;
}

ble_qiot_ret_status_t ble_qiot_advertising_stop(void)
{
    return 0 == ble_advertising_stop() ? BLE_QIOT_RS_OK : BLE_QIOT_RS_ERR;
}

ble_qiot_ret_status_t ble_qiot_explorer_init(void)
{
    ble_qiot_ret_status_t      ret_code     = BLE_QIOT_RS_OK;
    const qiot_service_init_s *service_info = NULL;

    ble_qiot_log_i("llsync version %s", BLE_QIOT_SDK_VERSION);
    service_info = ble_get_qiot_services();
    ble_services_add(service_info);

    ret_code = ble_init_flash_data();
    if (ret_code != BLE_QIOT_RS_OK) {
        ble_qiot_log_e("flash init failed, ret code %d", ret_code);
    }

    return ret_code;
}

void ble_device_info_write_cb(const uint8_t *buf, uint16_t len)
{
    ble_device_info_msg_handle((const char *)buf, len);
}

void ble_lldata_write_cb(const uint8_t *buf, uint16_t len)
{
    ble_lldata_msg_handle((const char *)buf, len);
}

// when gap get ble disconnect event, use this function
void ble_gap_disconnect_cb(void)
{
    if (ble_is_connected()) {
        ble_bind_state_set(E_BIND_SUCC);
        ble_qiot_advertising_start();
    }
}

int ble_device_info_msg_handle(const char *in_buf, int in_len)
{
    POINTER_SANITY_CHECK(in_buf, BLE_QIOT_RS_ERR_PARA);
    uint8_t ch;
    char    out_buf[128] = {0};
    int     ret_len      = 0;

    ch = in_buf[0];
    switch (ch) {
        case E_DEV_MSG_SYNC_TIME:
            ret_len = ble_bind_get_authcode(in_buf + 1, in_len - 1, out_buf, sizeof(out_buf));
            if (ret_len <= 0) {
                ble_qiot_log_e("get bind authcode failed");
                return BLE_QIOT_RS_ERR;
            }
            ble_event_notify((uint8_t)BLE_QIOT_EVENT_UP_BIND_SIGN_RET, NULL, 0, out_buf, ret_len);
            break;
        case E_DEV_MSG_CONN_VALID:
            ret_len = ble_conn_get_authcode(in_buf + 1, in_len - 1, out_buf, sizeof(out_buf));
            if (ret_len <= 0) {
                ble_qiot_log_e("get connect authcode failed");
                return BLE_QIOT_RS_ERR;
            }
            ble_event_notify((uint8_t)BLE_QIOT_EVENT_UP_CONN_SIGN_RET, NULL, 0, out_buf, ret_len);
            break;
        case E_DEV_MSG_BIND_SUCC:
            if (BLE_QIOT_RS_OK != ble_bind_write_result(in_buf + 1, in_len - 1)) {
                ble_qiot_log_e("write bind result failed");
                return BLE_QIOT_RS_ERR;
            }
            break;
        case E_DEV_MSG_BIND_FAIL:
            ble_qiot_log_i("get msg bind fail");
            break;
        case E_DEV_MSG_UNBIND:
            ret_len = ble_unbind_get_authcode(in_buf + 1, in_len - 1, out_buf, sizeof(out_buf));
            if (ret_len <= 0) {
                ble_qiot_log_e("get unbind authcode failed");
                return BLE_QIOT_RS_ERR;
            }
            ble_event_notify((uint8_t)BLE_QIOT_EVENT_UP_UNBIND_SIGN_RET, NULL, 0, out_buf, ret_len);
            break;
        case E_DEV_MSG_CONN_SUCC:
            ble_qiot_log_i("get msg connect success");
            break;
        case E_DEV_MSG_CONN_FAIL:
            ble_qiot_log_i("get msg connect fail");
            break;
        case E_DEV_MSG_UNBIND_SUCC:
            ble_qiot_log_i("get msg unbind success");
            if (BLE_QIOT_RS_OK != ble_unbind_write_result()) {
                ble_qiot_log_e("write unbind result failed");
                return BLE_QIOT_RS_ERR;
            }
            break;
        case E_DEV_MSG_UNBIND_FAIL:
            ble_qiot_log_i("get msg unbind fail");
            break;
        default:
            break;
    }

    return BLE_QIOT_RS_OK;
}

// lldata message from remote
int ble_lldata_msg_handle(const char *in_buf, int in_len)
{
    POINTER_SANITY_CHECK(in_buf, BLE_QIOT_RS_ERR_PARA);

    uint8_t data_type   = 0;
    uint8_t data_effect = 0;
    uint8_t id          = 0;

    if (!ble_is_connected()) {
        ble_qiot_log_e("operation negate, device not connected");
        return BLE_QIOT_RS_ERR;
    }

    data_type = BLE_QIOT_PARSE_MSG_HEAD_TYPE(in_buf[0]);
    if (data_type >= BLE_QIOT_DATA_TYPE_BUTT) {
        ble_qiot_log_e("invalid data type %d", data_type);
        return BLE_QIOT_RS_ERR;
    }
    data_effect = BLE_QIOT_PARSE_MSG_HEAD_EFFECT(in_buf[0]);
    if (data_effect >= BLE_QIOT_EFFECT_BUTT) {
        ble_qiot_log_e("invalid data effect %d", data_effect);
        return BLE_QIOT_RS_ERR;
    }
    id = BLE_QIOT_PARSE_MSG_HEAD_ID(in_buf[0]);
    ble_qiot_log_d("data type %d, effect %d, id %d", data_type, data_effect, id);

    switch (data_type) {
        case BLE_QIOT_MSG_TYPE_PROPERTY:
            if (BLE_QIOT_EFFECT_REQUEST == data_effect) {
                // default E_BLE_DATA_DOWN_TYPE_CONTROL
                return ble_lldata_property_request_handle(in_buf + 1, in_len - 1);
            } else if (BLE_QIOT_EFFECT_REPLY == data_effect) {
                // id means BLE_QIOT_DATA_DOWN_GET_STATUS_REPLY or BLE_QIOT_DATA_DOWN_REPORT_REPLY
                return ble_lldata_property_reply_handle(id, in_buf + 1, in_len - 1);
            } else {
                return BLE_QIOT_RS_ERR;
            }
        case BLE_QIOT_MSG_TYPE_EVENT:
            if (BLE_QIOT_EFFECT_REPLY == data_effect) {
                return ble_lldata_event_handle(id, in_buf + 1, in_len - 1);
            } else {
                ble_qiot_log_e("invalid event data effect");
                return BLE_QIOT_RS_ERR;
            }
        case BLE_QIOT_MSG_TYPE_ACTION:
            if (BLE_QIOT_EFFECT_REQUEST == data_effect) {
                return ble_lldata_action_handle(id, in_buf + 1, in_len - 1);
            } else {
                ble_qiot_log_e("invalid action data effect");
                return BLE_QIOT_RS_ERR;
            }
        default:
            break;
    }

    return BLE_QIOT_RS_OK;
}

#ifdef __cplusplus
}
#endif
