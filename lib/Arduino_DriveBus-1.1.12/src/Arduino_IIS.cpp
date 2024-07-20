/*
 * @Description: Arduino_IIS.cpp
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2023-11-16 16:58:05
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-02-28 14:53:14
 * @License: GPL 3.0
 */
#include "Arduino_IIS.h"

Arduino_IIS::Arduino_IIS(std::shared_ptr<Arduino_IIS_DriveBus> bus, int8_t sd)
    : _bus(bus), _sd(sd)
{
}

bool Arduino_IIS::begin(int8_t device_state, int32_t sample_rate, int32_t bits_per_sample)
{
    if (_sd != DRIVEBUS_DEFAULT_VALUE)
    {
        pinMode(_sd, OUTPUT);
        digitalWrite(_sd, HIGH); // 开启
    }

    if (_bus->begin(device_state, sample_rate, bits_per_sample) == false)
    {
        log_e("->_bus->begin(device_state, sample_rate, bits_per_sample) fail");
        return false;
    }

    return true;
}

bool Arduino_IIS::IIS_Device_Switch(bool device_switch)
{
    if (_sd != DRIVEBUS_DEFAULT_VALUE)
    {
        if (device_switch == Device_Switch::CHANNEL_OFF)
        {
            digitalWrite(_sd, LOW); // 关闭
            return true;
        }
        if (device_switch == Device_Switch::CHANNEL_ON)
        {
            digitalWrite(_sd, HIGH); // 开启
            return true;
        }
        log_e("->Enable Device_ Switch fail");
        return false;
    }
    log_e("->Unknown SD");
    return false;
}

bool Arduino_IIS::IIS_Read_Data(char *data, size_t len)
{
    log_e("No 'IIS_Read_Data' fictional function has been created.");
    return -1;
}

bool Arduino_IIS::IIS_Write_Data(char *data, size_t len)
{
    log_e("No 'IIS_Write_Data' fictional function has been created.");
    return -1;
}
