/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2017 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP32 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_ota_ops.h"

#include "ble_qiot_export.h"

extern void          ble_qiot_service_init(void);
static TimerHandle_t ota_reboot_timer;

static void test_task(void *pvParameters)
{
    while (1) {
        // ble_event_report_property();
        // ble_event_post(0);
        //  ble_event_get_status();
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}

void ble_ota_start_cb(void)
{
    printf("ble ota start callback\r\n");
    return;
}

ble_qiot_ret_status_t ble_ota_valid_file_cb(uint32_t file_size, char *file_version)
{
    printf("user valid file, size %d, file_version: %s\r\n", file_size, file_version);
    return BLE_QIOT_RS_OK;
}

static void ble_ota_reboot_timer(void *param)
{
    esp_restart();
}

void ble_ota_stop_cb(uint8_t result)
{
    printf("ble ota stop callback, result %d\r\n", result);
    if (result == BLE_QIOT_OTA_SUCCESS) {
        esp_partition_t *partition = esp_ota_get_next_update_partition(NULL);
        esp_ota_set_boot_partition(partition);
        printf("ota success, device restart after 5 seconds\r\n");
        ota_reboot_timer = xTimerCreate("reboot_timer", 5000 / portTICK_PERIOD_MS, pdFALSE, NULL, ble_ota_reboot_timer);
        xTimerStart(ota_reboot_timer, portMAX_DELAY);
    }
    return;
}

int ble_ota_write_flash(uint32_t flash_addr, const char *write_buf, uint16_t write_len)
{
    int ret = 0;

    if (flash_addr % 4096 == 0) {
        ret = spi_flash_erase_range(flash_addr, 4096);
    } else {
        if ((flash_addr + write_len - 1) / 4096 != flash_addr / 4096) {
            ret = spi_flash_erase_range(((flash_addr / 4096) + 1) * 4096, 4096);
        }
    }
    // printf("write ota addr 0x%x, write 0x%x, erase flash %d\r\n", flash_addr, write_len, ret);
    ret = spi_flash_write(flash_addr, write_buf, write_len);

    return ret == ESP_OK ? write_len : ret;
}

void app_main()
{
    nvs_flash_init();

    ble_ota_callback_reg(ble_ota_start_cb, ble_ota_stop_cb, ble_ota_valid_file_cb);
    ble_qiot_service_init();
    xTaskCreate(test_task, "tsk1", 4 * 1024, NULL, 5, NULL);
}
