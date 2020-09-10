## 概述

本文档介绍如何将 LLSync SDK 移植到目标硬件平台。

## 接入指引

一般情况下，各平台接入 LLSync SDK 可以分为两个步骤：

#### 硬件抽象层移植

`ble_qiot_import.h`中列出了所有需要开发者适配的接口，开发者可以参考函数描述在硬件平台上做具体实现。

###### 设备接口适配

| 序号 |        函数         |                 说明                 |
| :--: | :-----------------: | :----------------------------------: |
|  1   | ble_get_product_id  |       获取设备三元信息之产品ID       |
|  2   | ble_get_device_name |      获取设备三元信息之设备名称      |
|  3   |     ble_get_psk     |      获取设备三元信息之设备密钥      |
|  4   |     ble_get_mac     |         获取设备蓝牙MAC地址          |
|  5   |   ble_write_flash   |  SDK 需要存储内部信息到设备存储介质  |
|  6   |   ble_read_flash    | SDK 需要从设备存储介质读取存储的信息 |
|  7   |     定时器接口      |             定时器类接口             |

1. 设备三元信息在物联网开发平台创建设备后获取，开发者需要负责信息存储。
2. SDK 需要存储约 20 字节数据到 flash 中，存储地址由开发者划分，通过宏`BLE_QIOT_RECORD_FLASH_ADDR`指定。
3. 当使用定时广播功能时，需要实现定时器类接口。若不使用定时广播功能，定时器类接口可以实现为桩函数。

###### BLE 协议栈适配

| 序号 |            函数            |              说明               |
| :--: | :------------------------: | :-----------------------------: |
|  1   |   ble_advertising_start    |          开始广播接口           |
|  2   |    ble_advertising_stop    |          停止广播接口           |
|  3   |    ble_send_indication     |       向UUID FFE3写入通知       |
|  4   |      ble_services_add      |   添加LLSync 服务到BLE 协议栈   |
|  5   | ble_get_user_data_mtu_size | 向 UUID FFE3 写入数据的最大长度 |

1. LLSync 规定了广播数据的格式，开发者需要通过 BLE 协议栈将传入的数据广播出去。
2. SDK 定义了 LLSync Service，开发者需要将其加入 BLE 协议栈。
3. LLSync 规定了数据的最大长度为2048字节，SDK 实现了数据的分片发送，连续多次调用`ble_send_indication`将数据发送出去。SDK 内没有做数据重传处理，因此开发者需要保证数据的发送成功。

#### 数据模版代码生成

1. 开发者在物联网开发平台创建设备，定义[数据模版](https://cloud.tencent.com/document/product/1081/34916)

2. 通过脚本将数据模版转换为C代码

   ```c
   iot@iot-MB0 scripts % python3 interpret_json_dt/src/interpret_dt_ble.py interpret_json_dt/example.json
   reading json file start
   reading json file end
   generate header file start
   generate header file end
   generate source file start
   generate source file end
   
   ```

3. 拷贝生成的`ble_qiot_template.c`和`ble_qiot_template.h`到用户代码文件夹。

## 应用开发

###### SDK配置

拷贝`ble_qiot_config.h`到用户代码文件夹，并做简单配置。

1. `BLE_QIOT_RECORD_FLASH_ADDR`是 SDK 数据的存储地址。
2. `__BYTE_ORDER__`是设备的大小端定义。
3. SDK 支持定时广播。定义`BLE_QIOT_BUTTON_BROADCAST`为1，当设备在未绑定状态下开始广播时，经过`BLE_QIOT_BUTTON_BROADCAST`定义的时间后广播自动停止。设备处于绑定状态时不支持定时广播功能。
4. 需要适配 SDK 的log输出接口。

###### 例程代码抽取

SDK 已经适配了多个硬件平台，例程代码存储在`samples/nrf52832`目录下。开发者可以通过脚本将指定硬件平台的例程抽取出来，将抽取后的代码加入硬件平台进行编译。以`nordic 52832`示例：

```c
iot@iot-MB0 code_extract % python3 code_extract.py nrf52832
extract code for nrf52832 start, dest dir /iot/Desktop/work_code/qcloud_iot_explorer_ble/scripts/code_extract/qcloud-iot-ble-nrf52832
extract code success
```

一般的，例程代码中会提供相应的`readme.md`文档说明如何编译。

###### 数据模版开发

例程中`data_template`目录下包含了数据模版文件和转换后的 C 代码模版，开发者可以进行参考。

1. 对于数据模版中的`propertyies`，C 代码模版中给每个`id`生成了`ble_property_xxx_set`和`ble_property_xxx_get`两个接口。SDK 收到服务器下发的数据后通过`ble_property_xxx_set`传递给用户，开发者接收数据后进行处理。`ble_property_xxx_get`用于获取设备上的数据，SDK 负责将其上报到服务器。在`sg_ble_property_array`中定义了每个`id`的属性。
2. 对于数据模版中的`events`，每个`event`有若干个`param`，C 代码模版中给每个`param`生成了一个`ble_event_get_xxx`接口，用于获取设备上的数据，SDK 负责将其上报到服务器。在`sg_ble_event_array`中定义了每个`event`的属性，每个`event`也有一个数组用来描述其包含的所有`param`的属性。
3. 对于数据模版中的`actions`，每个`action`有若干个`inputid`和`outputid`，`inputid`中定义了服务器下发的数据，`outputid`中定义了设备上报的数据，C 代码模版中给每个`action`生成了`ble_action_handle_xxx_input_cb`和`ble_action_handle_xxx_output_cb`两个接口。SDK 收到服务器下发的数据后通过`ble_action_handle_xxx_input_cb`传递给用户，开发者接收数据后进行处理，处理结束后 SDK 通过`ble_action_handle_xxx_output_cb`获取用户反馈的数据上报到服务器。在`sg_ble_action_array`中定义了每个`action`的属性。

一般约定，用户数据以网络序进行传递，所以开发者需要做字节序转换。

###### API说明

在`ble_qiot_export.h`中提供了 API 函数，一般情况下用户只需要调用 API 函数即可完成应用开发。

| 序号 |            函数            |                             说明                             |
| :--: | :------------------------: | :----------------------------------------------------------: |
|  1   |   ble_qiot_explorer_init   |                          SDK 初始化                          |
|  2   |    ble_event_get_status    |      获取数据模版最新信息，对应数据模版`get_status`方法      |
|  3   | ble_event_report_property  |          上报设备最新信息，对应数据模版`report`方法          |
|  4   |       ble_event_post       |            上报事件，对应数据模版`event_post`方法            |
|  5   | ble_qiot_advertising_start | 开始广播，广播包内容请参考 |
|  6   | ble_qiot_advertising_stop  |                           停止广播                           |
|  7   |  ble_device_info_write_cb  |            UUID FFE1数据到达后调用此接口传入数据             |
|  8   |    ble_lldata_write_cb     |            UUID FFE2数据到达后调用此接口传入数据             |

## LLSync 协议

请参见[LLSync蓝牙设备接入协议](https://github.com/tencentyun/qcloud-iot-explorer-BLE-sdk-embedded/blob/master/docs/LLSync蓝牙设备接入协议.pdf)
