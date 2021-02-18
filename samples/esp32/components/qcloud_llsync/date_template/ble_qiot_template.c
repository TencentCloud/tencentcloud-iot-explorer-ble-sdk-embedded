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

#include "ble_qiot_template.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "ble_qiot_export.h"
#include "ble_qiot_common.h"
#include "ble_qiot_param_check.h"

static bool     sg_test_bool     = 0;
static int      sg_test_int      = 0;
static char     sg_test_str[128] = {0};
static float    sg_test_float    = 0.0;
static uint16_t sg_test_enum     = 0;
static uint32_t sg_test_time     = 0;

static struct_property_t_struct  sg_test_struct;
static struct_property_t_struct2 sg_test_struct2;

static int ble_property_t_bool_set(const char *data, uint16_t len)
{
    sg_test_bool = data[0];
    printf("set: sg_test_bool: %d\r\n", sg_test_bool);
    return 0;
}

static int ble_property_t_bool_get(char *data, uint16_t buf_len)
{
    data[0] = sg_test_bool;
    printf("get: sg_test_bool: %d\r\n", sg_test_bool);
    return sizeof(uint8_t);
}

static int ble_property_t_int_set(const char *data, uint16_t len)
{
    int test_int = 0;

    memcpy(&test_int, data, sizeof(int));
    sg_test_int = NTOHL(test_int);
    printf("set: sg_test_int: %d\r\n", sg_test_int);

    return 0;
}

static int ble_property_t_int_get(char *data, uint16_t buf_len)
{
    int test_int = 0;

    printf("get: sg_test_int: %d\r\n", sg_test_int);
    test_int = HTONL(sg_test_int);
    memcpy(data, &test_int, sizeof(int));

    return sizeof(uint32_t);
}

static int ble_property_t_str_set(const char *data, uint16_t len)
{
    memcpy(sg_test_str, data, sizeof(sg_test_str) > len ? len : (sizeof(sg_test_str) - 1));
    printf("set: sg_test_str: %s\r\n", sg_test_str);

    return 0;
}

static int ble_property_t_str_get(char *data, uint16_t buf_len)
{
    memcpy(data, sg_test_str, strlen(sg_test_str));
    printf("get: sg_test_str: %s\r\n", sg_test_str);

    return strlen(sg_test_str);
}

static int ble_property_t_float_set(const char *data, uint16_t len)
{
    memcpy(&sg_test_float, data, sizeof(float));
    printf("set: sg_test_float: %f\r\n", sg_test_float);

    return 0;
}

static int ble_property_t_float_get(char *data, uint16_t buf_len)
{
    memcpy(data, &sg_test_float, sizeof(float));
    printf("get: sg_test_float: %f\r\n", sg_test_float);

    return sizeof(float);
}

static int ble_property_t_enum_set(const char *data, uint16_t len)
{
    uint16_t test_enum = 0;

    memcpy(&test_enum, data, sizeof(uint16_t));
    sg_test_enum = NTOHS(test_enum);
    printf("set: sg_test_enum: %d\r\n", sg_test_enum);

    return 0;
}

static int ble_property_t_enum_get(char *data, uint16_t buf_len)
{
    uint16_t test_enum = 0;

    printf("get: sg_test_enum: %d\r\n", sg_test_enum);
    test_enum = HTONS(sg_test_enum);
    memcpy(data, &test_enum, sizeof(uint16_t));

    return sizeof(uint16_t);
}

static int ble_property_t_time_set(const char *data, uint16_t len)
{
    int test_int = 0;

    memcpy(&test_int, data, sizeof(int));
    sg_test_time = NTOHL(test_int);
    printf("set: sg_test_time: %d\r\n", sg_test_time);

    return 0;
}

static int ble_property_t_time_get(char *data, uint16_t buf_len)
{
    int test_int = 0;

    printf("get: sg_test_time: %d\r\n", sg_test_time);
    test_int = HTONL(sg_test_time);
    memcpy(data, &test_int, sizeof(int));

    return sizeof(uint32_t);
}

static int ble_property_t_struct_s_bool_set(const char *data, uint16_t len)
{
    sg_test_struct.m_s_bool = data[0];
    printf("set: sg_test_struct.m_s_bool: %d\r\n", sg_test_struct.m_s_bool);
    return 0;
}

static int ble_property_t_struct_s_bool_get(char *data, uint16_t buf_len)
{
    data[0] = sg_test_struct.m_s_bool;
    printf("get: sg_test_struct.m_s_bool: %d\r\n", sg_test_struct.m_s_bool);
    return sizeof(uint8_t);
}

static int ble_property_t_struct_s_int_set(const char *data, uint16_t len)
{
    int test_int = 0;

    memcpy(&test_int, data, sizeof(int));
    sg_test_struct.m_s_int = NTOHL(test_int);
    printf("set: sg_test_struct.m_s_int: %d\r\n", sg_test_struct.m_s_int);
    return 0;
}

static int ble_property_t_struct_s_int_get(char *data, uint16_t buf_len)
{
    int test_int = 0;

    printf("get: sg_test_int: %d\r\n", sg_test_struct.m_s_int);
    test_int = HTONL(sg_test_struct.m_s_int);
    memcpy(data, &test_int, sizeof(int));

    return sizeof(uint32_t);
}

static int ble_property_t_struct_s_str_set(const char *data, uint16_t len)
{
    memcpy(sg_test_struct.m_s_str, data,
           sizeof(sg_test_struct.m_s_str) > len ? len : (sizeof(sg_test_struct.m_s_str) - 1));
    printf("set: sg_test_struct.m_s_str: %s\r\n", sg_test_struct.m_s_str);
    return 0;
}

static int ble_property_t_struct_s_str_get(char *data, uint16_t buf_len)
{
    memcpy(data, sg_test_struct.m_s_str, strlen(sg_test_struct.m_s_str));
    printf("get: sg_test_struct.m_s_str: %s\r\n", sg_test_struct.m_s_str);
    return strlen(sg_test_struct.m_s_str);
}

static int ble_property_t_struct_s_float_set(const char *data, uint16_t len)
{
    memcpy(&sg_test_struct.m_s_float, data, sizeof(float));
    printf("set: sg_test_struct.m_s_float: %f\r\n", sg_test_struct.m_s_float);
    return 0;
}

static int ble_property_t_struct_s_float_get(char *data, uint16_t buf_len)
{
    memcpy(data, &sg_test_struct.m_s_float, sizeof(float));
    printf("get: sg_test_struct.m_s_float: %f\r\n", sg_test_struct.m_s_float);
    return sizeof(float);
}

static int ble_property_t_struct_s_enum_set(const char *data, uint16_t len)
{
    uint16_t test_enum = 0;

    memcpy(&test_enum, data, sizeof(uint16_t));
    sg_test_struct.m_s_enum = NTOHS(test_enum);
    printf("set: sg_test_struct.m_s_enum: %d\r\n", sg_test_struct.m_s_enum);
    return 0;
}

static int ble_property_t_struct_s_enum_get(char *data, uint16_t buf_len)
{
    uint16_t test_enum = 0;

    printf("get: sg_test_struct.m_s_enum: %d\r\n", sg_test_struct.m_s_enum);
    test_enum = HTONS(sg_test_struct.m_s_enum);
    memcpy(data, &test_enum, sizeof(uint16_t));
    return sizeof(uint16_t);
}

static int ble_property_t_struct_s_time_set(const char *data, uint16_t len)
{
    int test_int = 0;

    memcpy(&test_int, data, sizeof(int));
    sg_test_struct.m_s_time = NTOHL(test_int);
    printf("set: sg_test_struct.m_s_time: %d\r\n", sg_test_struct.m_s_time);
    return 0;
}

static int ble_property_t_struct_s_time_get(char *data, uint16_t buf_len)
{
    int test_int = 0;

    printf("get: sg_test_struct.m_s_time: %d\r\n", sg_test_struct.m_s_time);
    test_int = HTONL(sg_test_struct.m_s_time);
    memcpy(data, &test_int, sizeof(int));
    return sizeof(uint32_t);
}

static ble_property_t sg_ble_t_struct_property_array[BLE_QIOT_STRUCT_T_STRUCT_PROPERTY_ID_BUTT] = {
    {ble_property_t_struct_s_bool_set, ble_property_t_struct_s_bool_get, BLE_QIOT_PROPERTY_AUTH_RW,
     BLE_QIOT_DATA_TYPE_BOOL},
    {ble_property_t_struct_s_int_set, ble_property_t_struct_s_int_get, BLE_QIOT_PROPERTY_AUTH_RW,
     BLE_QIOT_DATA_TYPE_INT},
    {ble_property_t_struct_s_str_set, ble_property_t_struct_s_str_get, BLE_QIOT_PROPERTY_AUTH_RW,
     BLE_QIOT_DATA_TYPE_STRING},
    {ble_property_t_struct_s_float_set, ble_property_t_struct_s_float_get, BLE_QIOT_PROPERTY_AUTH_RW,
     BLE_QIOT_DATA_TYPE_FLOAT},
    {ble_property_t_struct_s_enum_set, ble_property_t_struct_s_enum_get, BLE_QIOT_PROPERTY_AUTH_RW,
     BLE_QIOT_DATA_TYPE_ENUM},
    {ble_property_t_struct_s_time_set, ble_property_t_struct_s_time_get, BLE_QIOT_PROPERTY_AUTH_RW,
     BLE_QIOT_DATA_TYPE_TIME},
};

static int ble_property_t_struct_set(const char *data, uint16_t len)
{
    return ble_user_property_struct_handle(data, len, sg_ble_t_struct_property_array,
                                           BLE_QIOT_STRUCT_T_STRUCT_PROPERTY_ID_BUTT);
}

static int ble_property_t_struct_get(char *data, uint16_t len)
{
    return ble_user_property_struct_get_data(data, len, sg_ble_t_struct_property_array,
                                             BLE_QIOT_STRUCT_T_STRUCT_PROPERTY_ID_BUTT);
}

static int ble_property_t_struct2_s_bool_set(const char *data, uint16_t len)
{
    sg_test_struct2.m_s_bool = data[0];
    printf("set: sg_test_struct2.m_s_bool: %d\r\n", sg_test_struct2.m_s_bool);
    return 0;
}

static int ble_property_t_struct2_s_bool_get(char *data, uint16_t buf_len)
{
    data[0] = sg_test_struct2.m_s_bool;
    printf("get: sg_test_struct2.m_s_bool: %d\r\n", sg_test_struct2.m_s_bool);
    return sizeof(uint8_t);
}

static ble_property_t sg_ble_t_struct2_property_array[BLE_QIOT_STRUCT_T_STRUCT2_PROPERTY_ID_BUTT] = {
    {ble_property_t_struct2_s_bool_set, ble_property_t_struct2_s_bool_get, BLE_QIOT_PROPERTY_AUTH_RW,
     BLE_QIOT_DATA_TYPE_BOOL},
};

static int ble_property_t_struct2_set(const char *data, uint16_t len)
{
    return ble_user_property_struct_handle(data, len, sg_ble_t_struct2_property_array,
                                           BLE_QIOT_STRUCT_T_STRUCT2_PROPERTY_ID_BUTT);
}

static int ble_property_t_struct2_get(char *data, uint16_t len)
{
    return ble_user_property_struct_get_data(data, len, sg_ble_t_struct2_property_array,
                                             BLE_QIOT_STRUCT_T_STRUCT2_PROPERTY_ID_BUTT);
}

static ble_property_t sg_ble_property_array[BLE_QIOT_PROPERTY_ID_BUTT] = {
    {ble_property_t_bool_set, ble_property_t_bool_get, BLE_QIOT_PROPERTY_AUTH_RW, BLE_QIOT_DATA_TYPE_BOOL},
    {ble_property_t_int_set, ble_property_t_int_get, BLE_QIOT_PROPERTY_AUTH_RW, BLE_QIOT_DATA_TYPE_INT},
    {ble_property_t_str_set, ble_property_t_str_get, BLE_QIOT_PROPERTY_AUTH_RW, BLE_QIOT_DATA_TYPE_STRING},
    {ble_property_t_float_set, ble_property_t_float_get, BLE_QIOT_PROPERTY_AUTH_RW, BLE_QIOT_DATA_TYPE_FLOAT},
    {ble_property_t_enum_set, ble_property_t_enum_get, BLE_QIOT_PROPERTY_AUTH_RW, BLE_QIOT_DATA_TYPE_ENUM},
    {ble_property_t_time_set, ble_property_t_time_get, BLE_QIOT_PROPERTY_AUTH_RW, BLE_QIOT_DATA_TYPE_TIME},
    {ble_property_t_struct_set, ble_property_t_struct_get, BLE_QIOT_PROPERTY_AUTH_RW, BLE_QIOT_DATA_TYPE_STRUCT},
    {ble_property_t_struct2_set, ble_property_t_struct2_get, BLE_QIOT_PROPERTY_AUTH_RW, BLE_QIOT_DATA_TYPE_STRUCT},
};

static bool ble_check_space_enough_by_type(uint8_t type, uint16_t left_size)
{
    switch (type) {
        case BLE_QIOT_DATA_TYPE_BOOL:
            return left_size >= sizeof(uint8_t);
        case BLE_QIOT_DATA_TYPE_INT:
        case BLE_QIOT_DATA_TYPE_FLOAT:
        case BLE_QIOT_DATA_TYPE_TIME:
            return left_size >= sizeof(uint32_t);
        case BLE_QIOT_DATA_TYPE_ENUM:
            return left_size >= sizeof(uint16_t);
        default:
            // string length is unknow, default true
            return true;
    }
}

static uint16_t ble_check_ret_value_by_type(uint8_t type, uint16_t buf_len, uint16_t ret_val)
{
    switch (type) {
        case BLE_QIOT_DATA_TYPE_BOOL:
            return ret_val <= sizeof(uint8_t);
        case BLE_QIOT_DATA_TYPE_INT:
        case BLE_QIOT_DATA_TYPE_FLOAT:
        case BLE_QIOT_DATA_TYPE_TIME:
            return ret_val <= sizeof(uint32_t);
        case BLE_QIOT_DATA_TYPE_ENUM:
            return ret_val <= sizeof(uint16_t);
        default:
            // string length is unknow, default true
            return ret_val <= buf_len;
    }
}

uint8_t ble_get_property_type_by_id(uint8_t id)
{
    if (id >= BLE_QIOT_PROPERTY_ID_BUTT) {
        ble_qiot_log_e("invalid property id %d", id);
        return BLE_QIOT_DATA_TYPE_BUTT;
    }
    return sg_ble_property_array[id].type;
}

int ble_user_property_set_data(const e_ble_tlv *tlv)
{
    POINTER_SANITY_CHECK(tlv, BLE_QIOT_RS_ERR_PARA);
    if (tlv->id >= BLE_QIOT_PROPERTY_ID_BUTT) {
        ble_qiot_log_e("invalid property id %d", tlv->id);
        return BLE_QIOT_RS_ERR;
    }

    if (NULL != sg_ble_property_array[tlv->id].set_cb) {
        if (0 != sg_ble_property_array[tlv->id].set_cb(tlv->val, tlv->len)) {
            ble_qiot_log_e("set property id %d failed", tlv->id);
            return BLE_QIOT_RS_ERR;
        } else {
            return BLE_QIOT_RS_OK;
        }
    }
    ble_qiot_log_e("invalid set callback, id %d", tlv->id);

    return BLE_QIOT_RS_ERR;
}

int ble_user_property_get_data_by_id(uint8_t id, char *buf, uint16_t buf_len)
{
    int ret_len = 0;

    POINTER_SANITY_CHECK(buf, BLE_QIOT_RS_ERR_PARA);
    if (id >= BLE_QIOT_PROPERTY_ID_BUTT) {
        ble_qiot_log_e("invalid property id %d", id);
        return -1;
    }

    if (NULL != sg_ble_property_array[id].get_cb) {
        if (!ble_check_space_enough_by_type(sg_ble_property_array[id].type, buf_len)) {
            ble_qiot_log_e("not enough space get property id %d data", id);
            return -1;
        }
        ret_len = sg_ble_property_array[id].get_cb(buf, buf_len);
        if (ret_len < 0) {
            ble_qiot_log_e("get property id %d data failed", id);
            return -1;
        } else {
            if (ble_check_ret_value_by_type(sg_ble_property_array[id].type, buf_len, ret_len)) {
                return ret_len;
            } else {
                ble_qiot_log_e("property id %d length invalid", id);
                return -1;
            }
        }
    }
    ble_qiot_log_e("invalid callback, property id %d", id);

    return 0;
}

int ble_user_property_report_reply_handle(uint8_t result)
{
    ble_qiot_log_d("report reply result %d", result);

    return BLE_QIOT_RS_OK;
}

int ble_user_property_struct_handle(const char *in_buf, uint16_t buf_len, ble_property_t struct_arr[], uint8_t arr_size)
{
    uint16_t  parse_len = 0;
    uint16_t  ret_len   = 0;
    e_ble_tlv tlv;

    while (parse_len < buf_len) {
        memset(&tlv, 0, sizeof(e_ble_tlv));
        ret_len = ble_lldata_parse_tlv(in_buf + parse_len, buf_len - parse_len, &tlv);
        parse_len += ret_len;
        if (parse_len > buf_len) {
            ble_qiot_log_e("parse struct failed");
            return parse_len;
        }

        if (tlv.id >= arr_size) {
            ble_qiot_log_e("invalid array index %d", tlv.id);
            return parse_len;
        }
        if (NULL == struct_arr[tlv.id].set_cb) {
            ble_qiot_log_e("invalid member id %d", tlv.id);
            return parse_len;
        }
        if (BLE_QIOT_RS_OK != struct_arr[tlv.id].set_cb(tlv.val, tlv.len)) {
            ble_qiot_log_e("user handle property error, member id %d, type %d, len %d", tlv.id, tlv.type, tlv.len);
            return parse_len;
        }
    }

    return 0;
}

int ble_user_property_struct_get_data(char *in_buf, uint16_t buf_len, ble_property_t struct_arr[], uint8_t arr_size)
{
    uint8_t  property_id   = 0;
    uint8_t  property_type = 0;
    int      property_len  = 0;
    char *   data_buf      = in_buf;
    uint16_t data_len      = 0;
    uint16_t string_len    = 0;

    for (property_id = 0; property_id < arr_size; property_id++) {
        property_type = struct_arr[property_id].type;
        if (property_type >= BLE_QIOT_DATA_TYPE_BUTT) {
            ble_qiot_log_e("member id %d type %d invalid", property_id, property_type);
            return BLE_QIOT_RS_ERR;
        }
        data_buf[data_len++] = BLE_QIOT_PACKAGE_TLV_HEAD(property_type, property_id);
        if (BLE_QIOT_DATA_TYPE_STRING == property_type) {
            // reserved 2 bytes for string length
            property_len = struct_arr[property_id].get_cb((char *)data_buf + data_len + 2, buf_len - data_len - 2);
        } else {
            property_len = struct_arr[property_id].get_cb((char *)data_buf + data_len, buf_len - data_len);
        }
        if (property_len < 0) {
            ble_qiot_log_e("too long data, member id %d, data length %d", property_id, data_len);
            return BLE_QIOT_RS_ERR;
        } else if (property_len == 0) {
            // no data to post
            data_len--;
            data_buf[data_len] = '0';
            ble_qiot_log_d("member id %d no data to post", property_id);
        } else {
            if (BLE_QIOT_DATA_TYPE_STRING == property_type) {
                string_len = HTONS(property_len);
                memcpy(data_buf + data_len, &string_len, sizeof(uint16_t));
                data_len += sizeof(uint16_t);
            }
            data_len += property_len;
        }
    }

    return data_len;
}

static int ble_event_get_t_event_t_bool(char *data, uint16_t buf_len)
{
    data[0] = true;
    return sizeof(uint8_t);
}

static int ble_event_get_t_event_t_int(char *data, uint16_t buf_len)
{
    int test_int = HTONL(32);

    memcpy(data, &test_int, sizeof(int));
    return sizeof(uint32_t);
}

static int ble_event_get_t_event_t_str(char *data, uint16_t buf_len)
{
    memcpy(data, "event", sizeof("event") - 1);
    return sizeof("event") - 1;
}

static int ble_event_get_t_event_t_enum(char *data, uint16_t buf_len)
{
    uint16_t test_enum = HTONS(1);

    memcpy(data, &test_enum, sizeof(uint16_t));
    return sizeof(uint16_t);
}

static int ble_event_get_t_event_t_float(char *data, uint16_t buf_len)
{
    float test_float = 1.0;
    memcpy(data, &test_float, sizeof(float));
    return sizeof(float);
}

static int ble_event_get_t_event_t_time(char *data, uint16_t buf_len)
{
    int test_int = HTONL(32);

    memcpy(data, &test_int, sizeof(int));
    return sizeof(uint32_t);
}

static ble_event_param sg_ble_event_t_event_array[BLE_QIOT_EVENT_T_EVENT_PARAM_ID_BUTT] = {
    {ble_event_get_t_event_t_bool, BLE_QIOT_DATA_TYPE_BOOL},   {ble_event_get_t_event_t_int, BLE_QIOT_DATA_TYPE_INT},
    {ble_event_get_t_event_t_str, BLE_QIOT_DATA_TYPE_STRING},  {ble_event_get_t_event_t_enum, BLE_QIOT_DATA_TYPE_ENUM},
    {ble_event_get_t_event_t_float, BLE_QIOT_DATA_TYPE_FLOAT}, {ble_event_get_t_event_t_time, BLE_QIOT_DATA_TYPE_TIME},
};

static ble_event_t sg_ble_event_array[BLE_QIOT_EVENT_ID_BUTT] = {
    {sg_ble_event_t_event_array, sizeof(sg_ble_event_t_event_array) / sizeof(ble_event_param)},
};

int ble_event_get_id_array_size(uint8_t event_id)
{
    if (event_id >= BLE_QIOT_EVENT_ID_BUTT) {
        ble_qiot_log_e("invalid event id %d", event_id);
        return -1;
    }

    return sg_ble_event_array[event_id].array_size;
}

uint8_t ble_event_get_param_id_type(uint8_t event_id, uint8_t param_id)
{
    if (event_id >= BLE_QIOT_EVENT_ID_BUTT) {
        ble_qiot_log_e("invalid event id %d", event_id);
        return BLE_QIOT_DATA_TYPE_BUTT;
    }
    if (param_id >= sg_ble_event_array[event_id].array_size) {
        ble_qiot_log_e("invalid param id %d", param_id);
        return BLE_QIOT_DATA_TYPE_BUTT;
    }

    return sg_ble_event_array[event_id].event_array[param_id].type;
}

int ble_event_get_data_by_id(uint8_t event_id, uint8_t param_id, char *out_buf, uint16_t buf_len)
{
    int ret_len = 0;

    if (event_id >= BLE_QIOT_EVENT_ID_BUTT) {
        ble_qiot_log_e("invalid event id %d", event_id);
        return -1;
    }
    if (param_id >= sg_ble_event_array[event_id].array_size) {
        ble_qiot_log_e("invalid param id %d", param_id);
        return -1;
    }
    if (NULL == sg_ble_event_array[event_id].event_array[param_id].get_cb) {
        ble_qiot_log_e("invalid callback, event id %d, param id %d", event_id, param_id);
        return 0;
    }

    if (!ble_check_space_enough_by_type(sg_ble_event_array[event_id].event_array[param_id].type, buf_len)) {
        ble_qiot_log_e("not enough space get data, event id %d, param id %d", event_id, param_id);
        return -1;
    }
    ret_len = sg_ble_event_array[event_id].event_array[param_id].get_cb(out_buf, buf_len);
    if (ret_len < 0) {
        ble_qiot_log_e("get event data failed, event id %d, param id %d", event_id, param_id);
        return -1;
    } else {
        if (ble_check_ret_value_by_type(sg_ble_event_array[event_id].event_array[param_id].type, buf_len, ret_len)) {
            return ret_len;
        } else {
            ble_qiot_log_e("evnet data length invalid, event id %d, param id %d", event_id, param_id);
            return -1;
        }
    }
}

int ble_user_event_reply_handle(uint8_t event_id, uint8_t result)
{
    ble_qiot_log_d("event id %d, reply result %d", event_id, result);

    return BLE_QIOT_RS_OK;
}

static int ble_action_handle_t_action_input_cb(e_ble_tlv *input_param_array, uint8_t input_array_size,
                                               uint8_t *output_id_array)
{
    int      i          = 0;
    int      test_int   = 0;
    float    test_float = 0;
    uint16_t test_enum  = 0;
    uint32_t test_time  = 0;

    for (i = 0; i < input_array_size; i++) {
        if (BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_BOOL == input_param_array[i].id) {
            printf("input id: %d, val: %d\r\n", BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_BOOL, input_param_array[i].val[0]);
        } else if (BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_INT == input_param_array[i].id) {
            memcpy(&test_int, input_param_array[i].val, sizeof(int));
            test_int = NTOHL(test_int);
            printf("input id: %d, val: %d\r\n", BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_INT, test_int);
        } else if (BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_STR == input_param_array[i].id) {
            printf("input id: %d, val: %s\r\n", BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_STR, input_param_array[i].val);
        } else if (BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_FLOAT == input_param_array[i].id) {
            memcpy(&test_float, input_param_array[i].val, sizeof(float));
            printf("input id: %d, val: %f\r\n", BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_FLOAT, test_float);
        } else if (BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_ENUM == input_param_array[i].id) {
            memcpy(&test_enum, input_param_array[i].val, sizeof(uint16_t));
            test_enum = NTOHS(test_enum);
            printf("input id: %d, val: %d\r\n", BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_ENUM, test_enum);
        } else if (BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_TIME == input_param_array[i].id) {
            memcpy(&test_time, input_param_array[i].val, sizeof(int));
            test_time = NTOHL(test_time);
            printf("input id: %d, val: %d\r\n", BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_TIME, test_time);
        }
        output_id_array[input_param_array[i].id] = true;
    }

    return 0;
}

static int ble_action_handle_t_action_output_cb(uint8_t output_id, char *buf, uint16_t buf_len)
{
    uint16_t ret_len    = 0;
    int      test_int   = HTONL(32);
    int      test_float = 3;
    uint16_t test_enum  = HTONS(1);
    uint32_t test_time  = HTONL(1613059200);

    if (BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_BOOL == output_id) {
        buf[0]  = true;
        ret_len = 1;
    } else if (BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_INT == output_id) {
        memcpy(buf, &test_int, sizeof(int));
        ret_len = sizeof(int);
    } else if (BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_STR == output_id) {
        memcpy(buf, "output", sizeof("output") - 1);
        ret_len = sizeof("output") - 1;
    } else if (BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_FLOAT == output_id) {
        memcpy(buf, &test_float, sizeof(float));
        ret_len = sizeof(float);
    } else if (BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_ENUM == output_id) {
        memcpy(buf, &test_enum, sizeof(uint16_t));
        ret_len = sizeof(uint16_t);
    } else if (BLE_QIOT_ACTION_T_ACTION_INPUT_ID_IN_TIME == output_id) {
        memcpy(buf, &test_time, sizeof(int));
        ret_len = sizeof(int);
    }

    return ret_len;
}

static uint8_t sg_ble_action_t_action_input_type_array[BLE_QIOT_ACTION_T_ACTION_INPUT_ID_BUTT] = {
    BLE_QIOT_DATA_TYPE_BOOL,  BLE_QIOT_DATA_TYPE_INT,  BLE_QIOT_DATA_TYPE_STRING,
    BLE_QIOT_DATA_TYPE_FLOAT, BLE_QIOT_DATA_TYPE_ENUM, BLE_QIOT_DATA_TYPE_TIME,
};

static uint8_t sg_ble_action_t_action_output_type_array[BLE_QIOT_ACTION_T_ACTION_OUTPUT_ID_BUTT] = {
    BLE_QIOT_DATA_TYPE_BOOL,  BLE_QIOT_DATA_TYPE_INT,  BLE_QIOT_DATA_TYPE_STRING,
    BLE_QIOT_DATA_TYPE_FLOAT, BLE_QIOT_DATA_TYPE_ENUM, BLE_QIOT_DATA_TYPE_TIME,
};

static ble_action_t sg_ble_action_array[BLE_QIOT_ACTION_ID_BUTT] = {
    {ble_action_handle_t_action_input_cb, ble_action_handle_t_action_output_cb, sg_ble_action_t_action_input_type_array,
     sg_ble_action_t_action_output_type_array, sizeof(sg_ble_action_t_action_input_type_array) / sizeof(uint8_t),
     sizeof(sg_ble_action_t_action_output_type_array) / sizeof(uint8_t)},
};

uint8_t ble_action_get_intput_type_by_id(uint8_t action_id, uint8_t input_id)
{
    if (action_id >= BLE_QIOT_ACTION_ID_BUTT) {
        ble_qiot_log_e("invalid action id %d", action_id);
        return BLE_QIOT_DATA_TYPE_BUTT;
    }
    if (input_id >= sg_ble_event_array[action_id].array_size) {
        ble_qiot_log_e("invalid input id %d", input_id);
        return BLE_QIOT_DATA_TYPE_BUTT;
    }

    return sg_ble_action_array[action_id].input_type_array[input_id];
}

uint8_t ble_action_get_output_type_by_id(uint8_t action_id, uint8_t output_id)
{
    if (action_id >= BLE_QIOT_ACTION_ID_BUTT) {
        ble_qiot_log_e("invalid action id %d", action_id);
        return BLE_QIOT_DATA_TYPE_BUTT;
    }
    if (output_id >= sg_ble_event_array[action_id].array_size) {
        ble_qiot_log_e("invalid output id %d", output_id);
        return BLE_QIOT_DATA_TYPE_BUTT;
    }

    return sg_ble_action_array[action_id].output_type_array[output_id];
}

int ble_action_get_input_id_size(uint8_t action_id)
{
    if (action_id >= BLE_QIOT_ACTION_ID_BUTT) {
        ble_qiot_log_e("invalid action id %d", action_id);
        return -1;
    }

    return sg_ble_action_array[action_id].input_id_size;
}

int ble_action_get_output_id_size(uint8_t action_id)
{
    if (action_id >= BLE_QIOT_ACTION_ID_BUTT) {
        ble_qiot_log_e("invalid action id %d", action_id);
        return -1;
    }

    return sg_ble_action_array[action_id].output_id_size;
}

int ble_action_user_handle_input_param(uint8_t action_id, e_ble_tlv *input_param_array, uint8_t input_array_size,
                                       uint8_t *output_id_array)
{
    if (action_id >= BLE_QIOT_ACTION_ID_BUTT) {
        ble_qiot_log_e("invalid action id %d", action_id);
        return -1;
    }

    if (NULL != sg_ble_action_array[action_id].input_cb) {
        if (0 != sg_ble_action_array[action_id].input_cb(input_param_array, input_array_size, output_id_array)) {
            ble_qiot_log_e("input handle error");
            return -1;
        }
    }

    return 0;
}

int ble_action_user_handle_output_param(uint8_t action_id, uint8_t output_id, char *buf, uint16_t buf_len)
{
    int ret_len = 0;

    if (action_id >= BLE_QIOT_ACTION_ID_BUTT) {
        ble_qiot_log_e("invalid action id %d", action_id);
        return -1;
    }
    if (NULL == sg_ble_action_array[action_id].output_cb) {
        ble_qiot_log_e("invalid callback, action id %d", action_id);
        return 0;
    }

    if (!ble_check_space_enough_by_type(sg_ble_action_array[action_id].output_type_array[output_id], buf_len)) {
        ble_qiot_log_e("not enough space get data, action id %d, output id %d", action_id, output_id);
        return -1;
    }

    ret_len = sg_ble_action_array[action_id].output_cb(output_id, buf, buf_len);
    if (ret_len < 0) {
        ble_qiot_log_e("get action data failed, action id %d, output id %d", action_id, output_id);
        return -1;
    } else {
        if (ble_check_ret_value_by_type(sg_ble_action_array[action_id].output_type_array[output_id], buf_len,
                                        ret_len)) {
            return ret_len;
        } else {
            ble_qiot_log_e("action data length invalid, action id %d, output id %d", action_id, output_id);
            return -1;
        }
    }
}

#ifdef __cplusplus
}
#endif
