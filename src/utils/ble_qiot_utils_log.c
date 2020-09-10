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

#include "ble_qiot_log.h"

#include <stdint.h>
#include <stdio.h>

#include "ble_qiot_param_check.h"

#define HEX_DUMP_BYTE_PER_LINE 16

void HexDump(const char *hex_name, const char *data, uint32_t data_len)
{
    char buf[HEX_DUMP_BYTE_PER_LINE * 5] = {0};
    int  line_count = 0, line = 0, byte = 0, rest = 0, start_byte = 0;

    line_count = data_len / HEX_DUMP_BYTE_PER_LINE;
    if (data_len % HEX_DUMP_BYTE_PER_LINE) {
        line_count += 1;
    }

    printf("\r\ndump %s, length %d\r\n", hex_name, data_len);
    printf(" 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15\r\n");
    printf("===============================================\r\n");
    for (line = 0; line < line_count; line++) {
        start_byte = line * HEX_DUMP_BYTE_PER_LINE;
        if (data_len - start_byte < HEX_DUMP_BYTE_PER_LINE) {
            rest = data_len % HEX_DUMP_BYTE_PER_LINE;
        } else {
            rest = HEX_DUMP_BYTE_PER_LINE;
        }

        for (byte = 0; byte < HEX_DUMP_BYTE_PER_LINE; byte++) {
            if (byte < rest) {
                sprintf(&buf[byte * 3], "%02X ", data[start_byte + byte]);
            } else {
                sprintf(&buf[byte * 3], "   ");
            }
        }

        sprintf(&buf[HEX_DUMP_BYTE_PER_LINE * 3], "| ");
        for (byte = 0; byte < rest; byte++) {
            if (data[start_byte + byte] >= ' ' && data[start_byte + byte] <= '~') {
                buf[HEX_DUMP_BYTE_PER_LINE * 3 + 2 + byte] = data[start_byte + byte];
            } else {
                buf[HEX_DUMP_BYTE_PER_LINE * 3 + 2 + byte] = '.';
            }
        }
        sprintf(&buf[HEX_DUMP_BYTE_PER_LINE * 3 + 2 + rest], "\r\n");

        printf("%s", buf);  // do not use printf(buf), that cause '%' transfer next character
    }
    printf("\r\n");
}

#ifdef __cplusplus
}
#endif
