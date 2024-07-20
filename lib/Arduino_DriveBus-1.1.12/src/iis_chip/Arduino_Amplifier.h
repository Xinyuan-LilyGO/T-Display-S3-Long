/*
 * @Description(CN): 功放扬声器、耳机DAC等需要IIS输出的芯片都兼容
 *
 * @Description(EN): Chipsets for amplified speakers, headphone DACs, etc.,
 *       requiring IIS output are compatible.
 * 
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2023-12-21 14:23:07
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-05-25 18:02:30
 * @License: GPL 3.0
 */
#pragma once

#include "../Arduino_IIS.h"

class Arduino_Amplifier : public Arduino_IIS
{
public:
    Arduino_Amplifier(std::shared_ptr<Arduino_IIS_DriveBus> bus);

    bool begin(int8_t device_state = DRIVEBUS_DEFAULT_VALUE,
               int32_t sample_rate = 44100UL, int32_t bits_per_sample = 16) override;

    bool IIS_Write_Data(char *data, size_t length) override;
};