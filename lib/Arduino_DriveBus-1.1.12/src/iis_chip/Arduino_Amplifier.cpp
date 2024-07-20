/*
 * @Description: Arduino_Amplifier.cpp
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2023-12-21 14:04:34
 * @LastEditors: Xk_w
 * @LastEditTime: 2023-12-22 14:11:18
 * @License: GPL 3.0
 */

#include "Arduino_Amplifier.h"

Arduino_Amplifier::Arduino_Amplifier(std::shared_ptr<Arduino_IIS_DriveBus> bus)
    : Arduino_IIS(bus)
{
}

bool Arduino_Amplifier::begin(int8_t device_state, int32_t sample_rate, int32_t bits_per_sample)
{
    return Arduino_IIS::begin(device_state, sample_rate, bits_per_sample);
}

bool Arduino_Amplifier::IIS_Write_Data(char *data, size_t length)
{
    return _bus->IIS_Write(data, length);
}