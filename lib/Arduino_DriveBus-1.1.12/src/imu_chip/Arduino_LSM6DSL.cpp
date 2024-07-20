/*
 * @Description: Arduino_LSM6DSL.cpp
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2023-11-16 15:42:22
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-05-13 10:31:09
 * @License: GPL 3.0
 */
#include "Arduino_LSM6DSL.h"

Arduino_LSM6DSL::Arduino_LSM6DSL(
    std::shared_ptr<Arduino_IIC_DriveBus> bus, uint8_t device_address, int8_t rst, int8_t iqr)
    : Arduino_IIC(bus, device_address, rst, iqr)
{
}

bool Arduino_LSM6DSL::begin(int32_t speed)
{
    return Arduino_IIC::begin(speed);
}

bool Arduino_LSM6DSL::IIC_Initialization(void)
{
    if (_rst != DRIVEBUS_DEFAULT_VALUE)
    {
    }
    else
    {
        // Software Rest
    }

    if (_bus->BufferOperation(_device_address, LSM6DSL_Initialization_BufferOperations,
                              sizeof(LSM6DSL_Initialization_BufferOperations)) == false)
    {
        return false;
    }

    return true;
}

int32_t Arduino_LSM6DSL::IIC_Device_ID(void)
{
    uint8_t temp_buf = 0;
    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_DEVICE_ID, &temp_buf) == true)
    {
        return (int32_t)temp_buf;
    }
    return -1;
}

bool Arduino_LSM6DSL::IIC_Write_Device_State(uint32_t device, uint8_t state)
{
    uint8_t temp_buf = 0;
    uint8_t temp_buf_2 = 0;

    switch (device)
    {
    case Arduino_IIC_IMU::Device::IMU_ACCELERATION_POWER_MODE:
        switch (state)
        {
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_OFF_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf) == true)
            {
                temp_buf &= 0B00001111;

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_ULTRA_LOW_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL6_C, &temp_buf) == true)
            {
                temp_buf |= 0B00010000; // 启动低性能模式

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL6_C, temp_buf) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_2) == true)
                    {
                        temp_buf_2 &= 0B00001111;                    // 清空
                        temp_buf_2 = temp_buf_2 | (0B00001011 << 4); // 选择模式输出速率 1.6 Hz

                        if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, temp_buf_2) == true)
                        {
                            return true;
                        }
                    }
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_LOW_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL6_C, &temp_buf) == true)
            {
                temp_buf |= 0B00010000; // 启动低性能模式

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL6_C, temp_buf) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_2) == true)
                    {
                        temp_buf_2 &= 0B00001111;                    // 清空
                        temp_buf_2 = temp_buf_2 | (0B00000011 << 4); // 选择模式输出速率 52 Hz

                        if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, temp_buf_2) == true)
                        {
                            return true;
                        }
                    }
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_NORMAL_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL6_C, &temp_buf) == true)
            {
                temp_buf |= 0B00010000; // 启动低性能模式

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL6_C, temp_buf) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_2) == true)
                    {
                        temp_buf_2 &= 0B00001111;                    // 清空
                        temp_buf_2 = temp_buf_2 | (0B00000101 << 4); // 选择模式输出速率 208 Hz

                        if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, temp_buf_2) == true)
                        {
                            return true;
                        }
                    }
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_HIGH_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL6_C, &temp_buf) == true)
            {
                temp_buf &= 0B11101111; // 启动高性能模式

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL6_C, temp_buf) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_2) == true)
                    {
                        temp_buf_2 &= 0B00001111;                    // 清空
                        temp_buf_2 = temp_buf_2 | (0B00000111 << 4); // 选择模式输出速率 833 Hz

                        if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, temp_buf_2) == true)
                        {
                            return true;
                        }
                    }
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_ULTRA_HIGH_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL6_C, &temp_buf) == true)
            {
                temp_buf &= 0B11101111; // 启动高性能模式

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL6_C, temp_buf) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_2) == true)
                    {
                        temp_buf_2 &= 0B00001111;                    // 清空
                        temp_buf_2 = temp_buf_2 | (0B00001000 << 4); // 选择模式输出速率 1.66 kHz

                        if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, temp_buf_2) == true)
                        {
                            return true;
                        }
                    }
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_IMU::Device::IMU_GYROSCOPE_POWER_MODE:
        switch (state)
        {
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_OFF_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf) == true)
            {
                temp_buf &= 0B00001111;

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_ULTRA_LOW_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL7_G, &temp_buf) == true)
            {
                temp_buf |= 0B10000000; // 启动低性能模式

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL7_G, temp_buf) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_2) == true)
                    {
                        temp_buf_2 &= 0B00001111;                    // 清空
                        temp_buf_2 = temp_buf_2 | (0B00000001 << 4); // 选择模式输出速率 12.5 Hz

                        if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, temp_buf_2) == true)
                        {
                            return true;
                        }
                    }
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_LOW_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL7_G, &temp_buf) == true)
            {
                temp_buf |= 0B10000000; // 启动低性能模式

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL7_G, temp_buf) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_2) == true)
                    {
                        temp_buf_2 &= 0B00001111;                    // 清空
                        temp_buf_2 = temp_buf_2 | (0B00000011 << 4); // 选择模式输出速率 52 Hz

                        if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, temp_buf_2) == true)
                        {
                            return true;
                        }
                    }
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_NORMAL_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL7_G, &temp_buf) == true)
            {
                temp_buf |= 0B10000000; // 启动低性能模式

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL7_G, temp_buf) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_2) == true)
                    {
                        temp_buf_2 &= 0B00001111;                    // 清空
                        temp_buf_2 = temp_buf_2 | (0B00000101 << 4); // 选择模式输出速率 208 Hz

                        if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, temp_buf_2) == true)
                        {
                            return true;
                        }
                    }
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_HIGH_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL7_G, &temp_buf) == true)
            {
                temp_buf &= 0B01111111; // 启动高性能模式

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL7_G, temp_buf) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_2) == true)
                    {
                        temp_buf_2 &= 0B00001111;                    // 清空
                        temp_buf_2 = temp_buf_2 | (0B00000111 << 4); // 选择模式输出速率 833 Hz

                        if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, temp_buf_2) == true)
                        {
                            return true;
                        }
                    }
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_ULTRA_HIGH_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL7_G, &temp_buf) == true)
            {
                temp_buf &= 0B01111111; // 启动高性能模式

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL7_G, temp_buf) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_2) == true)
                    {
                        temp_buf_2 &= 0B00001111;                    // 清空
                        temp_buf_2 = temp_buf_2 | (0B00001000 << 4); // 选择模式输出速率 1.66 kHz(选太高会出现数据读取错误问题)

                        if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, temp_buf_2) == true)
                        {
                            return true;
                        }
                    }
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_IMU::Device::IMU_GYROSCOPE_SLEEP_MODE:
        switch (state)
        {
        case Arduino_IIC_IMU::Device_State::IMU_DEVICE_ON:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL4_C, &temp_buf) == true)
            {
                temp_buf &= 0B10111111;
                temp_buf = temp_buf | (0B00000001 << 6);

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL4_C, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_IMU::Device_State::IMU_DEVICE_OFF:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL4_C, &temp_buf) == true)
            {
                temp_buf &= 0B10111111;

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL4_C, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_IMU::Device::IMU_FIFO_POWER_MODE:
        switch (state)
        {
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_OFF_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, &temp_buf) == true)
            {
                temp_buf &= 0B10000111;

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_ULTRA_LOW_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, &temp_buf) == true)
            {
                temp_buf &= 0B10000111;                  // 清空
                temp_buf = temp_buf | (0B00000001 << 3); // 选择模式输出速率 12.5 Hz

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_LOW_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, &temp_buf) == true)
            {
                temp_buf &= 0B10000111;                  // 清空
                temp_buf = temp_buf | (0B00000011 << 3); // 选择模式输出速率 52 Hz

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_NORMAL_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, &temp_buf) == true)
            {
                temp_buf &= 0B10000111;                  // 清空
                temp_buf = temp_buf | (0B00000101 << 3); // 选择模式输出速率 208 Hz

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_HIGH_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, &temp_buf) == true)
            {
                temp_buf &= 0B10000111;                  // 清空
                temp_buf = temp_buf | (0B00000111 << 3); // 选择模式输出速率 833 Hz

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_DEVICE_ULTRA_HIGH_POWER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, &temp_buf) == true)
            {
                temp_buf &= 0B10000111;                  // 清空
                temp_buf = temp_buf | (0B00001000 << 3); // 选择模式输出速率 1.66 kHz

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_IMU::Device::IMU_FIFO_WORK_MODE:
        switch (state)
        {
        case Arduino_IIC_IMU::Device_Mode::IMU_FIFO_BYPASS:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, &temp_buf) == true)
            {
                temp_buf &= 0B11111000;

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_FIFO_NORMAL:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, &temp_buf) == true)
            {
                temp_buf &= 0B11111000;
                temp_buf |= 0B00000001;

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_FIFO_CONTINUE:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, &temp_buf) == true)
            {
                temp_buf &= 0B11111000;
                temp_buf |= 0B00000110;

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_FIFO_CONTINUE_TO_NORMAL:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, &temp_buf) == true)
            {
                temp_buf &= 0B11111000;
                temp_buf |= 0B00000011;

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_FIFO_BYPASS_TO_CONTINUE:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, &temp_buf) == true)
            {
                temp_buf &= 0B11111000;
                temp_buf |= 0B00000100;

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL5, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_IMU::Device::IMU_FIFO_DATA_TYPE:
        switch (state)
        {
        case Arduino_IIC_IMU::Device_Mode::IMU_FIFO_DATA_ACCELERATION_GYROSCOPE:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_MASTER_CONFIG, &temp_buf) == true)
            {
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL2, &temp_buf_2) == true)
                {
                    temp_buf &= 0B10111111;
                    temp_buf_2 &= 0B10111111;

                    if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_MASTER_CONFIG, temp_buf) == true)
                    {
                        if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL2, temp_buf_2) == true)
                        {
                            return true;
                        }
                    }
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_FIFO_DATA_PEDOMETER:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_MASTER_CONFIG, &temp_buf) == true)
            {
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL2, &temp_buf_2) == true)
                {
                    temp_buf &= 0B10111111;
                    temp_buf_2 &= 0B10111111;

                    temp_buf_2 = temp_buf_2 | (0B00000001 << 6);

                    if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_MASTER_CONFIG, temp_buf) == true)
                    {
                        if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL2, temp_buf_2) == true)
                        {
                            return true;
                        }
                    }
                }
            }
            break;
        case Arduino_IIC_IMU::Device_Mode::IMU_FIFO_DATA_SENSOR_HUB:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_MASTER_CONFIG, &temp_buf) == true)
            {
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL2, &temp_buf_2) == true)
                {
                    temp_buf &= 0B10111111;
                    temp_buf_2 &= 0B10111111;

                    temp_buf = temp_buf | (0B00000001 << 6);

                    if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_MASTER_CONFIG, temp_buf) == true)
                    {
                        if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL2, temp_buf_2) == true)
                        {
                            return true;
                        }
                    }
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_IMU::Device::IMU_PEDOMETER_MODE:
        switch (state)
        {
        case Arduino_IIC_IMU::Device_State::IMU_DEVICE_ON:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL10_C, &temp_buf) == true)
            {
                temp_buf &= 0B11101011;
                temp_buf = temp_buf | (0B00000101 << 2);

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL10_C, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_IMU::Device_State::IMU_DEVICE_OFF:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL10_C, &temp_buf) == true)
            {
                temp_buf &= 0B11101011;

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL10_C, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_IMU::Device::IMU_PEDOMETER_RESET: // 0 清0复位
        switch (state)
        {
        case Arduino_IIC_IMU::Device_State::IMU_DEVICE_ON:
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL10_C, &temp_buf) == true)
            {
                temp_buf &= 0B11111101; // 清空
                temp_buf = temp_buf | (0B00000001 << 1);
                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL10_C, temp_buf) == true) // 置1复位步数器的步数数据
                {
                    temp_buf &= 0B11111101;
                    if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL10_C, temp_buf) == true) // 还原计数器的状态
                    {
                        return true;
                    }
                }
            }
            break;
        case Arduino_IIC_IMU::Device_State::IMU_DEVICE_OFF:
            break;
        default:
            break;
        }

        break;

    default:
        break;
    }
    return false;
}

bool Arduino_LSM6DSL::IIC_Write_Device_Value(uint32_t device, int64_t value)
{
    uint8_t temp_buf = 0;
    uint8_t temp_buf_2 = 0;
    uint8_t temp_buf_3 = 0;
    uint8_t temp_buf_4 = 0;

    switch (device)
    {
    case Arduino_IIC_IMU::Device_Value::IMU_ACCELERATION_SENSITIVITY: // ±2g、±4g、±8g、±16g
        if (value >= 2 && value <= 16)
        {
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf) == true)
            {
                temp_buf &= 0B11110011; // 清空

                if ((value / 2) == 1) // ±2g
                {
                }
                else if ((value / 2) >= 2 && (value / 2) <= 3) // ±4g
                {
                    temp_buf = temp_buf | (0B00000010 << 2);
                }
                else if ((value / 2) >= 4 && (value / 2) <= 7) // ±8g
                {
                    temp_buf = temp_buf | (0B00000011 << 2);
                }
                else if ((value / 2) == 8) // ±16g
                {
                    temp_buf = temp_buf | (0B00000001 << 2);
                }

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_IMU::Device_Value::IMU_GYROSCOPE_SENSITIVITY: // ±125dps、±250dps、±500dps、±1000dps、±2000dps
        if (value >= 125 && value <= 2000)
        {
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf) == true)
            {
                temp_buf &= 0B11110001; // 清空

                if ((value / 125) == 1) // ±125dps
                {
                    temp_buf = temp_buf | (0B00000001 << 1);
                }
                else if ((value / 125) >= 2 && (value / 125) <= 3) // ±250dps
                {
                }
                else if ((value / 125) >= 4 && (value / 125) <= 7) // ±500dps
                {
                    temp_buf = temp_buf | (0B00000010 << 1);
                }
                else if ((value / 125) >= 8 && (value / 125) <= 15) // ±1000dps
                {
                    temp_buf = temp_buf | (0B00000100 << 1);
                }
                else if ((value / 125) == 16) // ±2000dps
                {
                    temp_buf = temp_buf | (0B00000110 << 1);
                }

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_IMU::Device_Value::IMU_ACCELERATION_CORRECTION: // 1-10000 个采样数
        if (value >= 1 && value <= 10000)
        {
            // uint8_t temp_current_power = 0;
            // _bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_current_power); // 保留当前功耗模式

            // temp_buf = temp_current_power & 0B00001111;                             // 清空
            // temp_buf = temp_buf | (0B00000111 << 4);                                // 选择模式输出速率 833 Hz
            // _bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, temp_buf); // 将功耗模式置为高功耗模式

            std::vector<double> temp_outx_current_sampling_value;
            std::vector<double> temp_outy_current_sampling_value;
            std::vector<double> temp_outz_current_sampling_value;

            for (uint32_t i = 0; i < value; i++) // 采样次数
            {
                // X轴
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
                {
                    if (((temp_buf & 0B00000010) >> 1) == true) // 判断是否有陀螺仪传感器新数据
                    {
                        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTX_L_XL, &temp_buf_2) == true)
                        {
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTX_H_XL, &temp_buf_3) == true)
                            {
                                switch (temp_buf_3 >> 7) // 判断正负号
                                {
                                case 0:
                                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_4) == true)
                                    {
                                        switch (temp_buf_4 & 0B00001100)
                                        {
                                        case 0B00000000: // ±2g灵敏度为 0.061 mg/LSB
                                            temp_outx_current_sampling_value.push_back(0.061 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                            break;
                                        case 0B00001000: // ±4g灵敏度为 0.122 mg/LSB
                                            temp_outx_current_sampling_value.push_back(0.122 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                            break;
                                        case 0B00001100: // ±8g灵敏度为 0.244 mg/LSB
                                            temp_outx_current_sampling_value.push_back(0.244 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                            break;
                                        case 0B00000100: // ±16g灵敏度为 0.488 mg/LSB
                                            temp_outx_current_sampling_value.push_back(0.488 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    break;
                                case 1:
                                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_4) == true)
                                    {
                                        switch (temp_buf_4 & 0B00001100)
                                        {
                                        case 0B00000000: // ±2g灵敏度为 0.061 mg/LSB
                                            temp_outx_current_sampling_value.push_back((-1) * 0.061 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1));
                                            break;
                                        case 0B00001000: // ±4g灵敏度为 0.122 mg/LSB
                                            temp_outx_current_sampling_value.push_back((-1) * 0.122 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1));
                                            break;
                                        case 0B00001100: // ±8g灵敏度为 0.244 mg/LSB
                                            temp_outx_current_sampling_value.push_back((-1) * 0.244 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1));
                                            break;
                                        case 0B00000100: // ±16g灵敏度为 0.488 mg/LSB
                                            temp_outx_current_sampling_value.push_back((-1) * 0.488 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1));
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    break;

                                default:
                                    break;
                                }
                            }
                        }
                    }
                }

                delay(5); // 防止数据读取出错

                // Y轴
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
                {
                    if (((temp_buf & 0B00000010) >> 1) == true) // 判断是否有陀螺仪传感器新数据
                    {
                        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTY_L_XL, &temp_buf_2) == true)
                        {
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTY_H_XL, &temp_buf_3) == true)
                            {
                                switch (temp_buf_3 >> 7) // 判断正负号
                                {
                                case 0:
                                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_4) == true)
                                    {
                                        switch (temp_buf_4 & 0B00001100)
                                        {
                                        case 0B00000000: // ±2g灵敏度为 0.061 mg/LSB
                                            temp_outy_current_sampling_value.push_back(0.061 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                            break;
                                        case 0B00001000: // ±4g灵敏度为 0.122 mg/LSB
                                            temp_outy_current_sampling_value.push_back(0.122 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                            break;
                                        case 0B00001100: // ±8g灵敏度为 0.244 mg/LSB
                                            temp_outy_current_sampling_value.push_back(0.244 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                            break;
                                        case 0B00000100: // ±16g灵敏度为 0.488 mg/LSB
                                            temp_outy_current_sampling_value.push_back(0.488 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    break;
                                case 1:
                                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_4) == true)
                                    {
                                        switch (temp_buf_4 & 0B00001100)
                                        {
                                        case 0B00000000: // ±2g灵敏度为 0.061 mg/LSB
                                            temp_outy_current_sampling_value.push_back((-1) * 0.061 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1));
                                            break;
                                        case 0B00001000: // ±4g灵敏度为 0.122 mg/LSB
                                            temp_outy_current_sampling_value.push_back((-1) * 0.122 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1));
                                            break;
                                        case 0B00001100: // ±8g灵敏度为 0.244 mg/LSB
                                            temp_outy_current_sampling_value.push_back((-1) * 0.244 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1));
                                            break;
                                        case 0B00000100: // ±16g灵敏度为 0.488 mg/LSB
                                            temp_outy_current_sampling_value.push_back((-1) * 0.488 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1));
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    break;

                                default:
                                    break;
                                }
                            }
                        }
                    }
                }

                delay(5); // 防止数据读取出错

                // Y轴
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
                {
                    if (((temp_buf & 0B00000010) >> 1) == true) // 判断是否有陀螺仪传感器新数据
                    {
                        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTZ_L_XL, &temp_buf_2) == true)
                        {
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTZ_H_XL, &temp_buf_3) == true)
                            {
                                switch (temp_buf_3 >> 7) // 判断正负号
                                {
                                case 0:
                                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_4) == true)
                                    {
                                        switch (temp_buf_4 & 0B00001100)
                                        {
                                        case 0B00000000: // ±2g灵敏度为 0.061 mg/LSB
                                            temp_outz_current_sampling_value.push_back(0.061 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                            break;
                                        case 0B00001000: // ±4g灵敏度为 0.122 mg/LSB
                                            temp_outz_current_sampling_value.push_back(0.122 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                            break;
                                        case 0B00001100: // ±8g灵敏度为 0.244 mg/LSB
                                            temp_outz_current_sampling_value.push_back(0.244 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                            break;
                                        case 0B00000100: // ±16g灵敏度为 0.488 mg/LSB
                                            temp_outz_current_sampling_value.push_back(0.488 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    break;
                                case 1:
                                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_4) == true)
                                    {
                                        switch (temp_buf_4 & 0B00001100)
                                        {
                                        case 0B00000000: // ±2g灵敏度为 0.061 mg/LSB
                                            temp_outz_current_sampling_value.push_back((-1) * 0.061 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1));
                                            break;
                                        case 0B00001000: // ±4g灵敏度为 0.122 mg/LSB
                                            temp_outz_current_sampling_value.push_back((-1) * 0.122 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1));
                                            break;
                                        case 0B00001100: // ±8g灵敏度为 0.244 mg/LSB
                                            temp_outz_current_sampling_value.push_back((-1) * 0.244 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1));
                                            break;
                                        case 0B00000100: // ±16g灵敏度为 0.488 mg/LSB
                                            temp_outz_current_sampling_value.push_back((-1) * 0.488 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1));
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    break;

                                default:
                                    break;
                                }
                            }
                        }
                    }
                }

                delay(5); // 防止数据读取出错
            }

            double temp_x_mean = 0.0;
            double temp_x_accum = 0.0;
            double temp_x_variance = 0.0;

            double temp_y_mean = 0.0;
            double temp_y_accum = 0.0;
            double temp_y_variance = 0.0;

            double temp_z_mean = 0.0;
            double temp_z_accum = 0.0;
            double temp_z_variance = 0.0;

            // X-axis
            //  求得均值
            temp_x_mean = std::accumulate(std::begin(temp_outx_current_sampling_value),
                                          std::end(temp_outx_current_sampling_value), 0.0) /
                          temp_outx_current_sampling_value.size();
            std::for_each(std::begin(temp_outx_current_sampling_value), std::end(temp_outx_current_sampling_value),
                          [&](const double d)
                          { temp_x_accum += (d - temp_x_mean) * (d - temp_x_mean); });
            // 求得标准方差
            temp_x_variance = sqrt(temp_x_accum / (temp_outx_current_sampling_value.size() - 1));

            // Y-axis
            //  求得均值
            temp_y_mean = std::accumulate(std::begin(temp_outy_current_sampling_value),
                                          std::end(temp_outy_current_sampling_value), 0.0) /
                          temp_outy_current_sampling_value.size();
            std::for_each(std::begin(temp_outy_current_sampling_value), std::end(temp_outy_current_sampling_value),
                          [&](const double d)
                          { temp_y_accum += (d - temp_y_mean) * (d - temp_y_mean); });
            // 求得标准方差
            temp_y_variance = sqrt(temp_y_accum / (temp_outy_current_sampling_value.size() - 1));

            // Z-axis
            //  求得均值
            temp_z_mean = std::accumulate(std::begin(temp_outz_current_sampling_value),
                                          std::end(temp_outz_current_sampling_value), 0.0) /
                          temp_outz_current_sampling_value.size();
            std::for_each(std::begin(temp_outz_current_sampling_value), std::end(temp_outz_current_sampling_value),
                          [&](const double d)
                          { temp_z_accum += (d - temp_z_mean) * (d - temp_z_mean); });
            // 求得标准方差
            temp_z_variance = sqrt(temp_z_accum / (temp_outz_current_sampling_value.size() - 1));

            // _bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, temp_current_power); // 将功耗模式还原

            // Serial.printf("Acceleration X Mean: %f\n", temp_x_mean);
            // Serial.printf("Acceleration X Variance: %f\n", temp_x_variance);

            // Serial.printf("Acceleration Y Mean: %f\n", temp_y_mean);
            // Serial.printf("Acceleration Y Variance: %f\n", temp_y_variance);

            // Serial.printf("Acceleration Z Mean: %f\n", temp_z_mean);
            // Serial.printf("Acceleration Z Variance: %f\n", temp_z_variance);

            // X-axis Y-axis Z-axis
            if (temp_x_variance > 10 ||
                temp_y_variance > 10 ||
                temp_z_variance > 10) // 一般情况下设备静止时的标准方差小于10
            {
                return false;
            }

            if (temp_z_mean < 0) // 将设备反放的时候 Z轴的值将小于0
            {
                return false;
            }

            _acceleration_x_correction = (-1) * temp_x_mean;
            _acceleration_y_correction = (-1) * temp_y_mean;
            _acceleration_z_correction = (-1) * (temp_z_mean - 1000); // 正放的情况下Z轴为1g

            return true;
        }
        break;
    case Arduino_IIC_IMU::Device_Value::IMU_GYROSCOPE_CORRECTION: // 1-10000 个采样数
        if (value >= 1 && value <= 10000)
        {
            // uint8_t temp_current_power = 0;

            // _bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_current_power); // 保留当前功耗模式

            // temp_buf = temp_current_power & 0B00001111;                            // 清空
            // temp_buf = temp_buf | (0B00000111 << 4);                               // 选择模式输出速率 833 Hz
            // _bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, temp_buf); // 将功耗模式置为高功耗模式

            std::vector<double> temp_outx_current_sampling_value;
            std::vector<double> temp_outy_current_sampling_value;
            std::vector<double> temp_outz_current_sampling_value;

            for (uint32_t i = 0; i < value; i++) // 采样次数
            {
                // X轴
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
                {
                    if (((temp_buf & 0B00000010) >> 1) == true) // 判断是否有陀螺仪传感器新数据
                    {
                        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTX_L_G, &temp_buf_2) == true)
                        {
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTX_H_G, &temp_buf_3) == true)
                            {
                                switch (temp_buf_3 >> 7) // 判断正负号
                                {
                                case 0:
                                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_4) == true) // 判断dps
                                    {
                                        switch (temp_buf_4 & 0B00001110)
                                        {
                                        case 0B00000010:                                                                                              // ±125dps
                                            temp_outx_current_sampling_value.push_back((4.375 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2))); // ±125dps灵敏度为 4.375 mdps/LSB
                                            break;
                                        case 0B00000000:                                                                                            // ±250dps
                                            temp_outx_current_sampling_value.push_back(8.750 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)); // ±250dps灵敏度为 8.75 mdps/LSB
                                            break;
                                        case 0B00000100:                                                                                             // ±500dps
                                            temp_outx_current_sampling_value.push_back(17.500 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)); // ±500dps灵敏度为 17.50 mdps/LSB
                                            break;
                                        case 0B00001000:                                                                                             // ±1000dps
                                            temp_outx_current_sampling_value.push_back(35.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)); // ±1000dps灵敏度为 35 mdps/LSB
                                            break;
                                        case 0B00001100:                                                                                             // ±2000dps
                                            temp_outx_current_sampling_value.push_back(70.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)); // ±2000dps灵敏度为 70 mdps/LSB
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    break;
                                case 1:
                                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_4) == true)
                                    {
                                        switch (temp_buf_4 & 0B00001110)
                                        {
                                        case 0B00000010:                                                                                                                     // ±125dps
                                            temp_outx_current_sampling_value.push_back((-1) * 4.375 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)); // ±125dps灵敏度为 4.375 mdps/LSB
                                            break;
                                        case 0B00000000:                                                                                                                     // ±250dps
                                            temp_outx_current_sampling_value.push_back((-1) * 8.750 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)); // ±250dps灵敏度为 8.75 mdps/LSB
                                            break;
                                        case 0B00000100:                                                                                                                      // ±500dps
                                            temp_outx_current_sampling_value.push_back((-1) * 17.500 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)); // ±500dps灵敏度为 17.50 mdps/LSB
                                            break;
                                        case 0B00001000:                                                                                                                      // ±1000dps
                                            temp_outx_current_sampling_value.push_back((-1) * 35.000 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)); // ±1000dps灵敏度为 35 mdps/LSB
                                            break;
                                        case 0B00001100:                                                                                                                      // ±2000dps
                                            temp_outx_current_sampling_value.push_back((-1) * 70.000 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)); // ±2000dps灵敏度为 70 mdps/LSB
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    break;

                                default:
                                    break;
                                }
                            }
                        }
                    }
                }

                delay(5); // 防止数据读取出错

                // Y轴
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
                {
                    if (((temp_buf & 0B00000010) >> 1) == true) // 判断是否有陀螺仪传感器新数据
                    {
                        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTY_L_G, &temp_buf_2) == true)
                        {
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTY_H_G, &temp_buf_3) == true)
                            {
                                switch (temp_buf_3 >> 7) // 判断正负号
                                {
                                case 0:
                                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_4) == true) // 判断dps
                                    {
                                        switch (temp_buf_4 & 0B00001110)
                                        {
                                        case 0B00000010:                                                                                            // ±125dps
                                            temp_outy_current_sampling_value.push_back(4.375 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)); // ±125dps灵敏度为 4.375 mdps/LSB
                                            break;
                                        case 0B00000000:                                                                                            // ±250dps
                                            temp_outy_current_sampling_value.push_back(8.750 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)); // ±250dps灵敏度为 8.75 mdps/LSB
                                            break;
                                        case 0B00000100:                                                                                             // ±500dps
                                            temp_outy_current_sampling_value.push_back(17.500 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)); // ±500dps灵敏度为 17.50 mdps/LSB
                                            break;
                                        case 0B00001000:                                                                                             // ±1000dps
                                            temp_outy_current_sampling_value.push_back(35.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)); // ±1000dps灵敏度为 35 mdps/LSB
                                            break;
                                        case 0B00001100:                                                                                             // ±2000dps
                                            temp_outy_current_sampling_value.push_back(70.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)); // ±2000dps灵敏度为 70 mdps/LSB
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    break;
                                case 1:
                                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_4) == true)
                                    {
                                        switch (temp_buf_4 & 0B00001110)
                                        {
                                        case 0B00000010:                                                                                                                     // ±125dps
                                            temp_outy_current_sampling_value.push_back((-1) * 4.375 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)); // ±125dps灵敏度为 4.375 mdps/LSB
                                            break;
                                        case 0B00000000:                                                                                                                     // ±250dps
                                            temp_outy_current_sampling_value.push_back((-1) * 8.750 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)); // ±250dps灵敏度为 8.75 mdps/LSB
                                            break;
                                        case 0B00000100:                                                                                                                      // ±500dps
                                            temp_outy_current_sampling_value.push_back((-1) * 17.500 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)); // ±500dps灵敏度为 17.50 mdps/LSB
                                            break;
                                        case 0B00001000:                                                                                                                      // ±1000dps
                                            temp_outy_current_sampling_value.push_back((-1) * 35.000 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)); // ±1000dps灵敏度为 35 mdps/LSB
                                            break;
                                        case 0B00001100:                                                                                                                      // ±2000dps
                                            temp_outy_current_sampling_value.push_back((-1) * 70.000 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)); // ±2000dps灵敏度为 70 mdps/LSB
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    break;

                                default:
                                    break;
                                }
                            }
                        }
                    }
                }

                delay(5); // 防止数据读取出错

                // Z轴
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
                {
                    if (((temp_buf & 0B00000010) >> 1) == true) // 判断是否有陀螺仪传感器新数据
                    {
                        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTZ_L_G, &temp_buf_2) == true)
                        {
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTZ_H_G, &temp_buf_3) == true)
                            {
                                switch (temp_buf_3 >> 7) // 判断正负号
                                {
                                case 0:
                                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_4) == true) // 判断dps
                                    {
                                        switch (temp_buf_4 & 0B00001110)
                                        {
                                        case 0B00000010:                                                                                            // ±125dps
                                            temp_outz_current_sampling_value.push_back(4.375 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)); // ±125dps灵敏度为 4.375 mdps/LSB
                                            break;
                                        case 0B00000000:                                                                                            // ±250dps
                                            temp_outz_current_sampling_value.push_back(8.750 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)); // ±250dps灵敏度为 8.75 mdps/LSB
                                            break;
                                        case 0B00000100:                                                                                             // ±500dps
                                            temp_outz_current_sampling_value.push_back(17.500 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)); // ±500dps灵敏度为 17.50 mdps/LSB
                                            break;
                                        case 0B00001000:                                                                                             // ±1000dps
                                            temp_outz_current_sampling_value.push_back(35.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)); // ±1000dps灵敏度为 35 mdps/LSB
                                            break;
                                        case 0B00001100:                                                                                             // ±2000dps
                                            temp_outz_current_sampling_value.push_back(70.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)); // ±2000dps灵敏度为 70 mdps/LSB
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    break;
                                case 1:
                                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_4) == true)
                                    {
                                        switch (temp_buf_4 & 0B00001110)
                                        {
                                        case 0B00000010:                                                                                                                     // ±125dps
                                            temp_outz_current_sampling_value.push_back((-1) * 4.375 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)); // ±125dps灵敏度为 4.375 mdps/LSB
                                            break;
                                        case 0B00000000:                                                                                                                     // ±250dps
                                            temp_outz_current_sampling_value.push_back((-1) * 8.750 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)); // ±250dps灵敏度为 8.75 mdps/LSB
                                            break;
                                        case 0B00000100:                                                                                                                      // ±500dps
                                            temp_outz_current_sampling_value.push_back((-1) * 17.500 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)); // ±500dps灵敏度为 17.50 mdps/LSB
                                            break;
                                        case 0B00001000:                                                                                                                      // ±1000dps
                                            temp_outz_current_sampling_value.push_back((-1) * 35.000 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)); // ±1000dps灵敏度为 35 mdps/LSB
                                            break;
                                        case 0B00001100:                                                                                                                      // ±2000dps
                                            temp_outz_current_sampling_value.push_back((-1) * 70.000 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)); // ±2000dps灵敏度为 70 mdps/LSB
                                            break;

                                        default:
                                            break;
                                        }
                                    }
                                    break;

                                default:
                                    break;
                                }
                            }
                        }
                    }
                }

                delay(5); // 防止数据读取出错
            }

            double temp_x_mean = 0.0;
            double temp_x_accum = 0.0;
            double temp_x_variance = 0.0;

            double temp_y_mean = 0.0;
            double temp_y_accum = 0.0;
            double temp_y_variance = 0.0;

            double temp_z_mean = 0.0;
            double temp_z_accum = 0.0;
            double temp_z_variance = 0.0;

            // X-axis
            //  求得均值
            temp_x_mean = std::accumulate(std::begin(temp_outx_current_sampling_value),
                                          std::end(temp_outx_current_sampling_value), 0.0) /
                          temp_outx_current_sampling_value.size();
            std::for_each(std::begin(temp_outx_current_sampling_value), std::end(temp_outx_current_sampling_value),
                          [&](const double d)
                          { temp_x_accum += (d - temp_x_mean) * (d - temp_x_mean); });
            // 求得标准方差
            temp_x_variance = sqrt(temp_x_accum / (temp_outx_current_sampling_value.size() - 1));

            // Y-axis
            //  求得均值
            temp_y_mean = std::accumulate(std::begin(temp_outy_current_sampling_value),
                                          std::end(temp_outy_current_sampling_value), 0.0) /
                          temp_outy_current_sampling_value.size();
            std::for_each(std::begin(temp_outy_current_sampling_value), std::end(temp_outy_current_sampling_value),
                          [&](const double d)
                          { temp_y_accum += (d - temp_y_mean) * (d - temp_y_mean); });
            // 求得标准方差
            temp_y_variance = sqrt(temp_y_accum / (temp_outy_current_sampling_value.size() - 1));

            // Z-axis
            //  求得均值
            temp_z_mean = std::accumulate(std::begin(temp_outz_current_sampling_value),
                                          std::end(temp_outz_current_sampling_value), 0.0) /
                          temp_outz_current_sampling_value.size();
            std::for_each(std::begin(temp_outz_current_sampling_value), std::end(temp_outz_current_sampling_value),
                          [&](const double d)
                          { temp_z_accum += (d - temp_z_mean) * (d - temp_z_mean); });
            // 求得标准方差
            temp_z_variance = sqrt(temp_z_accum / (temp_outz_current_sampling_value.size() - 1));

            // _bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, temp_current_power); // 将功耗模式还原

            // Serial.printf("Gyroscope X Mean: %f\n", temp_x_mean);
            // Serial.printf("Gyroscope X Variance: %f\n", temp_x_variance);

            // Serial.printf("Gyroscope Y Mean: %f\n", temp_y_mean);
            // Serial.printf("Gyroscope Y Variance: %f\n", temp_y_variance);

            // Serial.printf("Gyroscope Z Mean: %f\n", temp_z_mean);
            // Serial.printf("Gyroscope Z Variance: %f\n", temp_z_variance);

            // X-axis Y-axis Z-axis
            if (temp_x_variance > 200 ||
                temp_y_variance > 200 ||
                temp_z_variance > 200) // 一般情况下设备静止时的标准方差小于200
            {
                return false;
            }

            _gyroscope_x_correction = (-1) * temp_x_mean;
            _gyroscope_y_correction = (-1) * temp_y_mean;
            _gyroscope_z_correction = (-1) * temp_z_mean;
            return true;
        }
        break;
    case Arduino_IIC_IMU::Device_Value::IMU_FIFO_ACCELERATION_FACTOR: // 1、2、3、4、8、16、32
        if (value >= 1 && value <= 32)
        {
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL3, &temp_buf) == true)
            {
                temp_buf &= 0B11111000; // 清空

                if (value == 1) // 1
                {
                    temp_buf |= 0B00000001;
                }
                else if (value == 2) // 2
                {
                    temp_buf |= 0B00000010;
                }
                else if (value == 3) // 3
                {
                    temp_buf |= 0B00000011;
                }
                else if (value == 4) // 4
                {
                    temp_buf |= 0B00000100;
                }
                else if ((value >= 5) && (value <= 8)) // 8
                {
                    temp_buf |= 0B00000101;
                }
                else if ((value >= 9) && (value <= 16)) // 16
                {
                    temp_buf |= 0B00000110;
                }
                else if ((value >= 17) && (value <= 32)) // 32
                {
                    temp_buf |= 0B00000111;
                }

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL3, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_IMU::Device_Value::IMU_FIFO_GYROSCOPE_FACTOR: // 1、2、3、4、8、16、32
        if (value >= 1 && value <= 32)
        {
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL3, &temp_buf) == true)
            {
                temp_buf &= 0B11000111; // 清空

                if (value == 1) // 1
                {
                    temp_buf = temp_buf | (0B00000001 << 3);
                }
                else if (value == 2) // 2
                {
                    temp_buf = temp_buf | (0B00000010 << 3);
                }
                else if (value == 3) // 3
                {
                    temp_buf = temp_buf | (0B00000011 << 3);
                }
                else if (value == 4) // 4
                {
                    temp_buf = temp_buf | (0B00000100 << 3);
                }
                else if ((value >= 5) && (value <= 8)) // 8
                {
                    temp_buf = temp_buf | (0B00000101 << 3);
                }
                else if ((value >= 9) && (value <= 16)) // 16
                {
                    temp_buf = temp_buf | (0B00000110 << 3);
                }
                else if ((value >= 17) && (value <= 32)) // 32
                {
                    temp_buf = temp_buf | (0B00000111 << 3);
                }

                if (_bus->IIC_WriteC8D8(_device_address, LSM6DSL_RD_WR_FIFO_CTRL3, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;

    default:
        break;
    }

    return false;
}

double Arduino_LSM6DSL::IIC_Read_Device_Value(uint32_t information)
{
    uint8_t temp_buf = 0;
    uint8_t temp_buf_2 = 0;
    uint8_t temp_buf_3 = 0;
    uint8_t temp_buf_4 = 0;

    switch (information)
    {
    case Arduino_IIC_IMU::Value_Information::IMU_TEMPERATURE_VALUE: // 0摄氏度-50摄氏度
        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
        {
            if (((temp_buf & 0B00000100) >> 2) == true) // 判断是否有温度新数据
            {
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUT_TEMP_L, &temp_buf_2) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUT_TEMP_H, &temp_buf_3) == true)
                    {
                        switch (temp_buf_3 >> 7) // 判断正负号
                        {
                        case 0:
                            return double(25 + ((((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2) / 256.0)); // 灵敏度为+256 LSB/°C
                            break;
                        case 1:
                            return double(25 + ((-1) * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1) / 256.0)); // 灵敏度为+256 LSB/°C
                            break;

                        default:
                            break;
                        }
                    }
                }
            }
        }
        break;
    case Arduino_IIC_IMU::Value_Information::IMU_ACCELERATION_X: // ±2、±4g、±8g、±16g
        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
        {
            if ((temp_buf & 0B00000001) == true) // 判断是否有加速度传感器新数据
            {
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTX_L_XL, &temp_buf_2) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTX_H_XL, &temp_buf_3) == true)
                    {
                        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_4) == true)
                        {
                            switch (temp_buf_4 & 0B00001100)
                            {
                            case 0B00000000: // ±2g灵敏度为 0.061 mg/LSB
                                return double(0.061 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00001000: // ±4g灵敏度为 0.122 mg/LSB
                                return double(0.122 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00001100: // ±8g灵敏度为 0.244 mg/LSB
                                return double(0.244 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00000100: // ±16g灵敏度为 0.488 mg/LSB
                                return double(0.488 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;

                            default:
                                break;
                            }
                        }
                    }
                }
            }
        }
        break;
    case Arduino_IIC_IMU::Value_Information::IMU_ACCELERATION_Y: // ±2、±4g、±8g、±16g
        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
        {
            if ((temp_buf & 0B00000001) == true) // 判断是否有加速度传感器新数据
            {
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTY_L_XL, &temp_buf_2) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTY_H_XL, &temp_buf_3) == true)
                    {
                        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_4) == true)
                        {
                            switch (temp_buf_4 & 0B00001100)
                            {
                            case 0B00000000: // ±2g灵敏度为 0.061 mg/LSB
                                return double(0.061 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00001000: // ±4g灵敏度为 0.122 mg/LSB
                                return double(0.122 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00001100: // ±8g灵敏度为 0.244 mg/LSB
                                return double(0.244 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00000100: // ±16g灵敏度为 0.488 mg/LSB
                                return double(0.488 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;

                            default:
                                break;
                            }
                        }
                    }
                }
            }
        }
        break;
    case Arduino_IIC_IMU::Value_Information::IMU_ACCELERATION_Z: // ±2、±4g、±8g、±16g
        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
        {
            if ((temp_buf & 0B00000001) == true) // 判断是否有加速度传感器新数据
            {
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTZ_L_XL, &temp_buf_2) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTZ_H_XL, &temp_buf_3) == true)
                    {
                        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_4) == true)
                        {
                            switch (temp_buf_4 & 0B00001100)
                            {
                            case 0B00000000: // ±2g灵敏度为 0.061 mg/LSB
                                return double(0.061 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00001000: // ±4g灵敏度为 0.122 mg/LSB
                                return double(0.122 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00001100: // ±8g灵敏度为 0.244 mg/LSB
                                return double(0.244 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00000100: // ±16g灵敏度为 0.488 mg/LSB
                                return double(0.488 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;

                            default:
                                break;
                            }
                        }
                    }
                }
            }
        }
        break;
    case Arduino_IIC_IMU::Value_Information::IMU_GYROSCOPE_X: // ±125dps、±250dps、±500dps、±1000dps、±2000dps
        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
        {
            if (((temp_buf & 0B00000010) >> 1) == true) // 判断是否有陀螺仪传感器新数据
            {
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTX_L_G, &temp_buf_2) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTX_H_G, &temp_buf_3) == true)
                    {
                        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_4) == true)
                        {
                            switch (temp_buf_4 & 0B00001110)
                            {
                            case 0B00000010: // ±125dps灵敏度为 4.375 mdps/LSB
                                return double(4.375 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00000000: // ±250dps灵敏度为 8.75 mdps/LSB
                                return double(8.750 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00000100: // ±500dps灵敏度为 17.50 mdps/LSB
                                return double(17.500 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00001000: // ±1000dps灵敏度为 35 mdps/LSB
                                return double(35.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00001100: // ±2000dps灵敏度为 70 mdps/LSB
                                return double(70.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;

                            default:
                                break;
                            }
                        }
                    }
                }
            }
        }
        break;
    case Arduino_IIC_IMU::Value_Information::IMU_GYROSCOPE_Y: // ±125dps、±250dps、±500dps、±1000dps、±2000dps
        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
        {
            if (((temp_buf & 0B00000010) >> 1) == true) // 判断是否有陀螺仪传感器新数据
            {
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTY_L_G, &temp_buf_2) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTY_H_G, &temp_buf_3) == true)
                    {
                        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_4) == true)
                        {
                            switch (temp_buf_4 & 0B00001110)
                            {
                            case 0B00000010: // ±125dps灵敏度为 4.375 mdps/LSB
                                return double(4.375 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00000000: // ±250dps灵敏度为 8.75 mdps/LSB
                                return double(8.750 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00000100: // ±500dps灵敏度为 17.50 mdps/LSB
                                return double(17.500 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00001000: // ±1000dps灵敏度为 35 mdps/LSB
                                return double(35.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00001100: // ±2000dps灵敏度为 70 mdps/LSB
                                return double(70.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;

                            default:
                                break;
                            }
                        }
                    }
                }
            }
        }
        break;
    case Arduino_IIC_IMU::Value_Information::IMU_GYROSCOPE_Z: // ±125dps、±250dps、±500dps、±1000dps、±2000dps
        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
        {
            if (((temp_buf & 0B00000010) >> 1) == true) // 判断是否有陀螺仪传感器新数据
            {
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTZ_L_G, &temp_buf_2) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTZ_H_G, &temp_buf_3) == true)
                    {
                        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_4) == true)
                        {
                            switch (temp_buf_4 & 0B00001110)
                            {
                            case 0B00000010: // ±125dps灵敏度为 4.375 mdps/LSB
                                return double(4.375 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00000000: // ±250dps灵敏度为 8.75 mdps/LSB
                                return double(8.750 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00000100: // ±500dps灵敏度为 17.50 mdps/LSB
                                return double(17.500 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00001000: // ±1000dps灵敏度为 35 mdps/LSB
                                return double(35.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;
                            case 0B00001100: // ±2000dps灵敏度为 70 mdps/LSB
                                return double(70.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2));
                                break;

                            default:
                                break;
                            }
                        }
                    }
                }
            }
        }
        break;
    case Arduino_IIC_IMU::Value_Information::IMU_ACCELERATION_X_SIGNED: // ±2、±4g、±8g、±16g
        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
        {
            if ((temp_buf & 0B00000001) == true) // 判断是否有加速度传感器新数据
            {
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTX_L_XL, &temp_buf_2) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTX_H_XL, &temp_buf_3) == true)
                    {
                        switch (temp_buf_3 >> 7) // 判断正负号
                        {
                        case 0:
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_4) == true)
                            {
                                switch (temp_buf_4 & 0B00001100)
                                {
                                case 0B00000000: // ±2g灵敏度为 0.061 mg/LSB
                                    return double((0.061 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _acceleration_x_correction);
                                    break;
                                case 0B00001000: // ±4g灵敏度为 0.122 mg/LSB
                                    return double((0.122 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _acceleration_x_correction);
                                    break;
                                case 0B00001100: // ±8g灵敏度为 0.244 mg/LSB
                                    return double((0.244 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _acceleration_x_correction);
                                    break;
                                case 0B00000100: // ±16g灵敏度为 0.488 mg/LSB
                                    return double((0.488 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _acceleration_x_correction);
                                    break;

                                default:
                                    break;
                                }
                            }
                            break;
                        case 1:
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_4) == true)
                            {
                                switch (temp_buf_4 & 0B00001100)
                                {
                                case 0B00000000: // ±2g灵敏度为 0.061 mg/LSB
                                    return double(((-1) * 0.061 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _acceleration_x_correction);
                                    break;
                                case 0B00001000: // ±4g灵敏度为 0.122 mg/LSB
                                    return double(((-1) * 0.122 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _acceleration_x_correction);
                                    break;
                                case 0B00001100: // ±8g灵敏度为 0.244 mg/LSB
                                    return double(((-1) * 0.244 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _acceleration_x_correction);
                                    break;
                                case 0B00000100: // ±16g灵敏度为 0.488 mg/LSB
                                    return double(((-1) * 0.488 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _acceleration_x_correction);
                                    break;

                                default:
                                    break;
                                }
                            }
                            break;

                        default:
                            break;
                        }
                    }
                }
            }
        }
        break;
    case Arduino_IIC_IMU::Value_Information::IMU_ACCELERATION_Y_SIGNED: // ±2、±4g、±8g、±16g
        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
        {
            if ((temp_buf & 0B00000001) == true) // 判断是否有加速度传感器新数据
            {
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTY_L_XL, &temp_buf_2) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTY_H_XL, &temp_buf_3) == true)
                    {
                        switch (temp_buf_3 >> 7) // 判断正负号
                        {
                        case 0:
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_4) == true)
                            {
                                switch (temp_buf_4 & 0B00001100)
                                {
                                case 0B00000000: // ±2g灵敏度为 0.061 mg/LSB
                                    return double((0.061 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _acceleration_y_correction);
                                    break;
                                case 0B00001000: // ±4g灵敏度为 0.122 mg/LSB
                                    return double((0.122 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _acceleration_y_correction);
                                    break;
                                case 0B00001100: // ±8g灵敏度为 0.244 mg/LSB
                                    return double((0.244 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _acceleration_y_correction);
                                    break;
                                case 0B00000100: // ±16g灵敏度为 0.488 mg/LSB
                                    return double((0.488 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _acceleration_y_correction);
                                    break;

                                default:
                                    break;
                                }
                            }
                            break;
                        case 1:
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_4) == true)
                            {
                                switch (temp_buf_4 & 0B00001100)
                                {
                                case 0B00000000: // ±2g灵敏度为 0.061 mg/LSB
                                    return double(((-1) * 0.061 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _acceleration_y_correction);
                                    break;
                                case 0B00001000: // ±4g灵敏度为 0.122 mg/LSB
                                    return double(((-1) * 0.122 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _acceleration_y_correction);
                                    break;
                                case 0B00001100: // ±8g灵敏度为 0.244 mg/LSB
                                    return double(((-1) * 0.244 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _acceleration_y_correction);
                                    break;
                                case 0B00000100: // ±16g灵敏度为 0.488 mg/LSB
                                    return double(((-1) * 0.488 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _acceleration_y_correction);
                                    break;

                                default:
                                    break;
                                }
                            }
                            break;

                        default:
                            break;
                        }
                    }
                }
            }
        }
        break;
    case Arduino_IIC_IMU::Value_Information::IMU_ACCELERATION_Z_SIGNED: // ±2、±4g、±8g、±16g
        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
        {
            if ((temp_buf & 0B00000001) == true) // 判断是否有加速度传感器新数据
            {
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTZ_L_XL, &temp_buf_2) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTZ_H_XL, &temp_buf_3) == true)
                    {
                        switch (temp_buf_3 >> 7) // 判断正负号
                        {
                        case 0:
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_4) == true)
                            {
                                switch (temp_buf_4 & 0B00001100)
                                {
                                case 0B00000000: // ±2g灵敏度为 0.061 mg/LSB
                                    return double((0.061 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _acceleration_z_correction);
                                    break;
                                case 0B00001000: // ±4g灵敏度为 0.122 mg/LSB
                                    return double((0.122 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _acceleration_z_correction);
                                    break;
                                case 0B00001100: // ±8g灵敏度为 0.244 mg/LSB
                                    return double((0.244 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _acceleration_z_correction);
                                    break;
                                case 0B00000100: // ±16g灵敏度为 0.488 mg/LSB
                                    return double((0.488 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _acceleration_z_correction);
                                    break;

                                default:
                                    break;
                                }
                            }
                            break;
                        case 1:
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL1_XL, &temp_buf_4) == true)
                            {
                                switch (temp_buf_4 & 0B00001100)
                                {
                                case 0B00000000: // ±2g灵敏度为 0.061 mg/LSB
                                    return double(((-1) * 0.061 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _acceleration_z_correction);
                                    break;
                                case 0B00001000: // ±4g灵敏度为 0.122 mg/LSB
                                    return double(((-1) * 0.122 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _acceleration_z_correction);
                                    break;
                                case 0B00001100: // ±8g灵敏度为 0.244 mg/LSB
                                    return double(((-1) * 0.244 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _acceleration_z_correction);
                                    break;
                                case 0B00000100: // ±16g灵敏度为 0.488 mg/LSB
                                    return double(((-1) * 0.488 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _acceleration_z_correction);
                                    break;

                                default:
                                    break;
                                }
                            }
                            break;

                        default:
                            break;
                        }
                    }
                }
            }
        }
        break;
    case Arduino_IIC_IMU::Value_Information::IMU_GYROSCOPE_X_SIGNED: // ±125dps、±250dps、±500dps、±1000dps、±2000dps
        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
        {
            if (((temp_buf & 0B00000010) >> 1) == true) // 判断是否有陀螺仪传感器新数据
            {
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTX_L_G, &temp_buf_2) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTX_H_G, &temp_buf_3) == true)
                    {
                        switch (temp_buf_3 >> 7) // 判断正负号
                        {
                        case 0:
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_4) == true)
                            {
                                switch (temp_buf_4 & 0B00001110)
                                {
                                case 0B00000010: // ±125dps灵敏度为 4.375 mdps/LSB
                                    return double((4.375 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _gyroscope_x_correction);
                                    break;
                                case 0B00000000: // ±250dps灵敏度为 8.75 mdps/LSB
                                    return double((8.750 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _gyroscope_x_correction);
                                    break;
                                case 0B00000100: // ±500dps灵敏度为 17.50 mdps/LSB
                                    return double((17.500 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _gyroscope_x_correction);
                                    break;
                                case 0B00001000: // ±1000dps灵敏度为 35 mdps/LSB
                                    return double((35.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _gyroscope_x_correction);
                                    break;
                                case 0B00001100: // ±2000dps灵敏度为 70 mdps/LSB
                                    return double((70.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _gyroscope_x_correction);
                                    break;

                                default:
                                    break;
                                }
                            }
                            break;
                        case 1:
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_4) == true)
                            {
                                switch (temp_buf_4 & 0B00001110)
                                {
                                case 0B00000010: // ±125dps灵敏度为 4.375 mdps/LSB
                                    return double(((-1) * 4.375 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _gyroscope_x_correction);
                                    break;
                                case 0B00000000: // ±250dps灵敏度为 8.75 mdps/LSB
                                    return double(((-1) * 8.750 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _gyroscope_x_correction);
                                    break;
                                case 0B00000100: // ±500dps灵敏度为 17.50 mdps/LSB
                                    return double(((-1) * 17.500 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _gyroscope_x_correction);
                                    break;
                                case 0B00001000: // ±1000dps灵敏度为 35 mdps/LSB
                                    return double(((-1) * 35.000 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _gyroscope_x_correction);
                                    break;
                                case 0B00001100: // ±2000dps灵敏度为 70 mdps/LSB
                                    return double(((-1) * 70.000 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _gyroscope_x_correction);
                                    break;

                                default:
                                    break;
                                }
                            }
                            break;

                        default:
                            break;
                        }
                    }
                }
            }
        }
        break;
    case Arduino_IIC_IMU::Value_Information::IMU_GYROSCOPE_Y_SIGNED: // ±125dps、±250dps、±500dps、±1000dps、±2000dps
        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
        {
            if (((temp_buf & 0B00000010) >> 1) == true) // 判断是否有陀螺仪传感器新数据
            {
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTY_L_G, &temp_buf_2) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTY_H_G, &temp_buf_3) == true)
                    {
                        switch (temp_buf_3 >> 7) // 判断正负号
                        {
                        case 0:
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_4) == true)
                            {
                                switch (temp_buf_4 & 0B00001110)
                                {
                                case 0B00000010: // ±125dps灵敏度为 4.375 mdps/LSB
                                    return double((4.375 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _gyroscope_y_correction);
                                    break;
                                case 0B00000000: // ±250dps灵敏度为 8.75 mdps/LSB
                                    return double((8.750 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _gyroscope_y_correction);
                                    break;
                                case 0B00000100: // ±500dps灵敏度为 17.50 mdps/LSB
                                    return double((17.500 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _gyroscope_y_correction);
                                    break;
                                case 0B00001000: // ±1000dps灵敏度为 35 mdps/LSB
                                    return double((35.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _gyroscope_y_correction);
                                    break;
                                case 0B00001100: // ±2000dps灵敏度为 70 mdps/LSB
                                    return double((70.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _gyroscope_y_correction);
                                    break;

                                default:
                                    break;
                                }
                            }
                            break;
                        case 1:
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_4) == true)
                            {
                                switch (temp_buf_4 & 0B00001110)
                                {
                                case 0B00000010: // ±125dps灵敏度为 4.375 mdps/LSB
                                    return double(((-1) * 4.375 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _gyroscope_y_correction);
                                    break;
                                case 0B00000000: // ±250dps灵敏度为 8.75 mdps/LSB
                                    return double(((-1) * 8.750 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _gyroscope_y_correction);
                                    break;
                                case 0B00000100: // ±500dps灵敏度为 17.50 mdps/LSB
                                    return double(((-1) * 17.500 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _gyroscope_y_correction);
                                    break;
                                case 0B00001000: // ±1000dps灵敏度为 35 mdps/LSB
                                    return double(((-1) * 35.000 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _gyroscope_y_correction);
                                    break;
                                case 0B00001100: // ±2000dps灵敏度为 70 mdps/LSB
                                    return double(((-1) * 70.000 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _gyroscope_y_correction);
                                    break;

                                default:
                                    break;
                                }
                            }
                            break;

                        default:
                            break;
                        }
                    }
                }
            }
        }
        break;
    case Arduino_IIC_IMU::Value_Information::IMU_GYROSCOPE_Z_SIGNED: // ±125dps、±250dps、±500dps、±1000dps、±2000dps
        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STATUS_REG, &temp_buf) == true)
        {
            if (((temp_buf & 0B00000010) >> 1) == true) // 判断是否有陀螺仪传感器新数据
            {
                if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTZ_L_G, &temp_buf_2) == true)
                {
                    if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_OUTZ_H_G, &temp_buf_3) == true)
                    {
                        switch (temp_buf_3 >> 7) // 判断正负号
                        {
                        case 0:
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_4) == true)
                            {
                                switch (temp_buf_4 & 0B00001110)
                                {
                                case 0B00000010: // ±125dps灵敏度为 4.375 mdps/LSB
                                    return double((4.375 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _gyroscope_z_correction);
                                    break;
                                case 0B00000000: // ±250dps灵敏度为 8.75 mdps/LSB
                                    return double((8.750 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _gyroscope_z_correction);
                                    break;
                                case 0B00000100: // ±500dps灵敏度为 17.50 mdps/LSB
                                    return double((17.500 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _gyroscope_z_correction);
                                    break;
                                case 0B00001000: // ±1000dps灵敏度为 35 mdps/LSB
                                    return double((35.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _gyroscope_z_correction);
                                    break;
                                case 0B00001100: // ±2000dps灵敏度为 70 mdps/LSB
                                    return double((70.000 * (((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + _gyroscope_z_correction);
                                    break;

                                default:
                                    break;
                                }
                            }
                            break;
                        case 1:
                            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_WR_CTRL2_G, &temp_buf_4) == true)
                            {
                                switch (temp_buf_4 & 0B00001110)
                                {
                                case 0B00000010: // ±125dps灵敏度为 4.375 mdps/LSB
                                    return double(((-1) * 4.375 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _gyroscope_z_correction);
                                    break;
                                case 0B00000000: // ±250dps灵敏度为 8.75 mdps/LSB
                                    return double(((-1) * 8.750 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _gyroscope_z_correction);
                                    break;
                                case 0B00000100: // ±500dps灵敏度为 17.50 mdps/LSB
                                    return double(((-1) * 17.500 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _gyroscope_z_correction);
                                    break;
                                case 0B00001000: // ±1000dps灵敏度为 35 mdps/LSB
                                    return double(((-1) * 35.000 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _gyroscope_z_correction);
                                    break;
                                case 0B00001100: // ±2000dps灵敏度为 70 mdps/LSB
                                    return double(((-1) * 70.000 * ((~(int16_t)(((int16_t)temp_buf_3 << 8) | (int16_t)temp_buf_2)) + 1)) + _gyroscope_z_correction);
                                    break;

                                default:
                                    break;
                                }
                            }
                            break;

                        default:
                            break;
                        }
                    }
                }
            }
        }
        break;
    case Arduino_IIC_IMU::Value_Information::IMU_FIFO_DATA_PATTERN: // 0-1023
        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_FIFO_STATUS3, &temp_buf) == true)
        {
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_FIFO_STATUS4, &temp_buf_2) == true)
            {
                return ((int16_t)temp_buf_2 << 8) | (int16_t)temp_buf;
            }
        }
        break;
    case Arduino_IIC_IMU::Value_Information::IMU_FIFO_DATA:
        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_FIFO_DATA_OUT_L, &temp_buf) == true)
        {
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_FIFO_DATA_OUT_H, &temp_buf_2) == true)
            {
                return ((int16_t)temp_buf_2 << 8) | (int16_t)temp_buf;
            }
        }
        break;
    case Arduino_IIC_IMU::Value_Information::IMU_PEDOMETER_VALUE: // 0-65535 步
        if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STEP_COUNTER_L, &temp_buf) == true)
        {
            if (_bus->IIC_ReadC8D8(_device_address, LSM6DSL_RD_STEP_COUNTER_H, &temp_buf_2) == true)
            {
                return ((int16_t)temp_buf_2 << 8) | (int16_t)temp_buf;
            }
        }
        break;

    default:
        break;
    }
    return -1;
}
