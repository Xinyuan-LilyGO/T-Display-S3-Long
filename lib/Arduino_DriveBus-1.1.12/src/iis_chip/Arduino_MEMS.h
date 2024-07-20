/*
 * @Description(CN): MEMS相关麦克风均兼容
 *
 * @Description(EN): All MEMS-related microphones are compatible.
 *
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2023-12-21 14:23:07
 * @LastEditors: Xk_w
 * @LastEditTime: 2023-12-22 14:18:35
 * @License: GPL 3.0
 */
#pragma once

#include "../Arduino_IIS.h"

class Arduino_MEMS : public Arduino_IIS
{
public:
    Arduino_MEMS(std::shared_ptr<Arduino_IIS_DriveBus> bus, int8_t sd = DRIVEBUS_DEFAULT_VALUE);

    bool begin(int8_t device_state = DRIVEBUS_DEFAULT_VALUE,
               int32_t sample_rate = 44100UL, int32_t bits_per_sample = 16) override;

    bool IIS_Read_Data(char *data, size_t length) override;
};