/*
 * @Description: Arduino_MEMS.cpp
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2023-12-21 14:04:34
 * @LastEditors: Xk_w
 * @LastEditTime: 2023-12-22 14:16:59
 * @License: GPL 3.0
 */

#include "Arduino_MEMS.h"

Arduino_MEMS::Arduino_MEMS(std::shared_ptr<Arduino_IIS_DriveBus> bus, int8_t sd)
    : Arduino_IIS(bus, sd)
{
}

bool Arduino_MEMS::begin(int8_t device_state, int32_t sample_rate, int32_t bits_per_sample)
{
    return Arduino_IIS::begin(device_state, sample_rate, bits_per_sample);
}

bool Arduino_MEMS::IIS_Read_Data(char *data, size_t length)
{
    return _bus->IIS_Read(data, length);
}