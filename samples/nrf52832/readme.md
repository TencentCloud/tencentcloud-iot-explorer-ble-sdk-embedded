# 1.工程简介

本工程基于 nRF52832 平台开发，使用的 SDK 版本为 `nRF5_SDK_15.3.0_59ac345`  

蓝牙协议栈为 s132 版本，路径为 `\components\softdevice\s132\hex\s132_nrf52_6.1.1_softdevice.hex`  

DeviceFamilyPack为 `NordicSemiconductor.nRF_DeviceFamilyPack.8.24.1.pack`  

如需其他 pack 请前往 `http://developer.nordicsemi.com/nRF51_SDK/pieces/nRF_DeviceFamilyPack/`  

例程路径 `\examples\ble_peripheral\ble_iot_demo`  

例程keil工程文件路径 `\examples\ble_peripheral\ble_iot_demo\pca10040\s132\arm5_no_packs`  


# 2.目录结构

例程工程目录结构为
```
\examples\ble_peripheral\ble_iot_demo
├─pca10040                              # 
│  └─s132                               # 
│      ├─arm5_no_packs                  # 工程目录
│      │  ├─RTE                         # 
│      │  │  └─Device                   # 
│      │  │      └─nRF52832_xxAA        # 
│      │  └─_build                      # 编译产物
│      └─config                         # nordic平台SDK配置文件
└─qcloud_iot_explorer_ble               # 
    ├─config                            # 配置文件
    ├─docs                              # 连连Sync蓝牙设备接入协议
    ├─inc                               # 外部头文件，此目录下的内容不建议用户修改
    ├─ref-impl                          # nordic平台参考实现，用户可根据实际需求进行修改
    └─src                               # qiot源码，此目录下的内容不建议用户修改
        ├─core                          # 核心业务处理
        ├─internal_inc                  # 内部头文件
        └─utils                         # 通用组件
```


# 3.开发说明
## 3.1 文件说明
`qcloud_iot_explorer_ble\inc\ble_qiot_export.h` 为 qiot explorer ble sdk 提供给用户的接口、数据结构、配置等
`qcloud_iot_explorer_ble\inc\ble_qiot_import.h` 为需要用户实现的接口

## 3.2 新平台开发流程  
1.新建目录存放平台相关代码，例如`\ref-impl`  

2.复制`qcloud_iot_explorer_ble\config\ble_qiot_config.h`到平台目录下，例如
```
├─ref-impl
│      ble_qiot_config.h
```
基于目标平台修改`#define BLE_QIOT_DEBUG_PRINT printf  // debug`等5个打印的实现  
用户根据需要添加其他平台相关的配置  
用户工程中添加头文件`qcloud_iot_explorer_ble\ref-impl\ble_qiot_config.h`，不要添加`qcloud_iot_explorer_ble\config\ble_qiot_config.h`  

3.添加源文件实现`qcloud_iot_explorer_ble\inc\ble_qiot_import.h`中的接口，例如
```
├─ref-impl
│      ble_qiot_ble_device.c
```

4.用户调用`const qiot_service_init_s *ble_get_qiot_services(void)`获取 qiot explorer ble sdk 所需的蓝牙服务并加入蓝牙协议栈内，例如
```
├─ref-impl
│      nrf52832_xxaa_service.c
│      nrf52832_xxaa_service.h
```

5.在初始化流程中调用`ble_qiot_ret_status_t ble_qiot_explorer_init(void)`初始化 qiot explorer ble sdk  

6.开发用户业务
