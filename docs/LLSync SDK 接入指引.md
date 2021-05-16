# 1 概述

本文档介绍如何将 LLSync SDK 移植到目标硬件平台。

# 2 详细接入指引

对于单BLE设备，设备端需要使用 LLSync SDK 的标准蓝牙功能，您可以参考[LLSync SDK标准蓝牙功能接入指引](./LLSync SDK标准蓝牙功能接入指引.md)文档。
对于BLE + WIFI设备，设备端可能仅需要使用 LLSync SDK 的辅助配网功能，您可以参考[LLSync SDK辅助配网功能接入指引](./LLSync SDK辅助配网功能接入指引.md)文档。

## 2.1 数据模版代码生成

1. 开发者在物联网开发平台创建设备，定义 [数据模版](https://cloud.tencent.com/document/product/1081/34916)。

2. 通过脚本将数据模版转换为C代码。

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

# 3 应用开发

例程中`data_template`目录下包含了数据模版文件和转换后的 C 代码模版，开发者可以进行参考。

1. 对于数据模版中的`propertyies`，C 代码模版中给每个`id`生成了`ble_property_xxx_set`和`ble_property_xxx_get`两个接口。SDK 收到服务器下发的数据后通过`ble_property_xxx_set`传递给用户，开发者接收数据后进行处理。`ble_property_xxx_get`用于获取设备上的数据，SDK 负责将其上报到服务器。在`sg_ble_property_array`中定义了每个`id`的属性。
2. 对于数据模版中的`events`，每个`event`有若干个`param`，C 代码模版中给每个`param`生成了一个`ble_event_get_xxx`接口，用于获取设备上的数据，SDK 负责将其上报到服务器。在`sg_ble_event_array`中定义了每个`event`的属性，每个`event`也有一个数组用来描述其包含的所有`param`的属性。
3. 对于数据模版中的`actions`，每个`action`有若干个`inputid`和`outputid`，`inputid`中定义了服务器下发的数据，`outputid`中定义了设备上报的数据，C 代码模版中给每个`action`生成了`ble_action_handle_xxx_input_cb`和`ble_action_handle_xxx_output_cb`两个接口。SDK 收到服务器下发的数据后通过`ble_action_handle_xxx_input_cb`传递给用户，开发者接收数据后进行处理，处理结束后 SDK 通过`ble_action_handle_xxx_output_cb`获取用户反馈的数据上报到服务器。在`sg_ble_action_array`中定义了每个`action`的属性。

一般约定，用户数据以网络序进行传递，所以开发者需要做字节序转换。

# 4 LLSync 协议

请参见[LLSync蓝牙设备接入协议](./LLSync蓝牙设备接入协议.pdf) 