/*
 * @Description: Arduino_AW8624.cpp
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2024-05-13 11:41:29
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-05-15 11:33:15
 * @License: GPL 3.0
 */
#include "Arduino_AW8624.h"

Arduino_AW8624::Arduino_AW8624(
    std::shared_ptr<Arduino_IIC_DriveBus> bus, uint8_t device_address, int8_t rst, int8_t iqr,
    void (*Interrupt_Function)())
    : Arduino_IIC(bus, device_address, rst, iqr, Interrupt_Function)
{
}

bool Arduino_AW8624::begin(int32_t speed)
{
    return Arduino_IIC::begin(speed);
}

bool Arduino_AW8624::IIC_Initialization(void)
{
    if (_rst != DRIVEBUS_DEFAULT_VALUE)
    {
        pinMode(_rst, OUTPUT);
        digitalWrite(_rst, HIGH);
        delay(100);
        digitalWrite(_rst, LOW);
        delay(10);
        digitalWrite(_rst, HIGH);
        delay(200);
    }
    else
    {
        // Software Rest
    }

    if (_bus->BufferOperation(_device_address, AW8624_Initialization_BufferOperations,
                              sizeof(AW8624_Initialization_BufferOperations)) == false)
    {
        return false;
    }

    return true;
}

int32_t Arduino_AW8624::IIC_Device_ID(void)
{
    uint8_t temp_buf = 0;
    if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_DEVICE_ID, &temp_buf) == true)
    {
        return (int32_t)temp_buf;
    }
    return -1;
}

int32_t Arduino_AW8624::IIC_Device_Reset(void)
{
    uint8_t temp_buf = 0xAA;
    if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_DEVICE_ID, temp_buf) == true)
    {
        return true;
    }
    return -1;
}

bool Arduino_AW8624::IIC_Write_Device_State(uint32_t device, uint8_t state)
{
    uint8_t temp_buf = 0;

    switch (device)
    {
    case Arduino_IIC_HAPTIC::Device::HAPTIC_INITIALIZATION_SRAM:
        switch (state)
        {
        case Arduino_IIC_HAPTIC::Device_State::HAPTIC_DEVICE_ON:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_SYSTEM_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B00100000;
                temp_buf |= 0B00100000;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_SYSTEM_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_State::HAPTIC_DEVICE_OFF:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_SYSTEM_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B00100000;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_SYSTEM_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_HAPTIC::Device::HAPTIC_PLAY_MODE:
        switch (state)
        {
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_RAM_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_SYSTEM_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B11110011;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_SYSTEM_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_RTP_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_SYSTEM_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B11110011;
                temp_buf |= 0B00000100;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_SYSTEM_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_CONT_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_SYSTEM_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B11110011;
                temp_buf |= 0B00001000;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_SYSTEM_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_HAPTIC::Device::HAPTIC_BRAKE1_MODE:
        switch (state)
        {
        case Arduino_IIC_HAPTIC::Device_State::HAPTIC_DEVICE_ON:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_BRAKE1_CTRL, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;
                temp_buf |= 0B10000000;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_BRAKE1_CTRL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_State::HAPTIC_DEVICE_OFF:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_BRAKE1_CTRL, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_BRAKE1_CTRL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_HAPTIC::Device::HAPTIC_BRAKE2_MODE:
        switch (state)
        {
        case Arduino_IIC_HAPTIC::Device_State::HAPTIC_DEVICE_ON:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_BRAKE2_CTRL, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;
                temp_buf |= 0B10000000;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_BRAKE2_CTRL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_State::HAPTIC_DEVICE_OFF:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_BRAKE2_CTRL, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_BRAKE2_CTRL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_HAPTIC::Device::HAPTIC_CHIP_MODE:
        switch (state)
        {
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_ACTIVE_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_SYSTEM_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B11111110;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_SYSTEM_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_STANDBY_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_SYSTEM_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B11111110;
                temp_buf |= 0B00000001;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_SYSTEM_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_HAPTIC::Device::HAPTIC_PLAYBACK_GO_MODE:
        switch (state)
        {
        case Arduino_IIC_HAPTIC::Device_State::HAPTIC_DEVICE_ON:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_PROCESS_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B11111110;
                temp_buf |= 0B00000001;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_PROCESS_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_State::HAPTIC_DEVICE_OFF:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_PROCESS_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_PROCESS_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_HAPTIC::Device::HAPTIC_MOTOR_TYPE:
        switch (state)
        {
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_ERM_MOTOR_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVE_CTRL, &temp_buf) == true)
            {
                temp_buf &= 0B11111110;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVE_CTRL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_LRA_MOTOR_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVE_CTRL, &temp_buf) == true)
            {
                temp_buf &= 0B11111110;
                temp_buf |= 0B00000001;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVE_CTRL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_HAPTIC::Device::HAPTIC_WAVEFORM_FILLER_1_MODE:
        switch (state)
        {
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_WAVEFORM_FILLER_OUTPUT_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_1, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_1, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_WAVEFORM_FILLER_DELAY_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_1, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;
                temp_buf |= 0B10000000;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_1, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_HAPTIC::Device::HAPTIC_WAVEFORM_FILLER_2_MODE:
        switch (state)
        {
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_WAVEFORM_FILLER_OUTPUT_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_2, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_2, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_WAVEFORM_FILLER_DELAY_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_2, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;
                temp_buf |= 0B10000000;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_2, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_HAPTIC::Device::HAPTIC_WAVEFORM_FILLER_3_MODE:
        switch (state)
        {
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_WAVEFORM_FILLER_OUTPUT_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_3, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_3, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_WAVEFORM_FILLER_DELAY_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_3, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;
                temp_buf |= 0B10000000;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_3, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_HAPTIC::Device::HAPTIC_WAVEFORM_FILLER_4_MODE:
        switch (state)
        {
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_WAVEFORM_FILLER_OUTPUT_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_4, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_4, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_WAVEFORM_FILLER_DELAY_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_4, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;
                temp_buf |= 0B10000000;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_4, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_HAPTIC::Device::HAPTIC_WAVEFORM_FILLER_5_MODE:
        switch (state)
        {
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_WAVEFORM_FILLER_OUTPUT_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_5, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_5, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_WAVEFORM_FILLER_DELAY_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_5, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;
                temp_buf |= 0B10000000;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_5, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_HAPTIC::Device::HAPTIC_WAVEFORM_FILLER_6_MODE:
        switch (state)
        {
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_WAVEFORM_FILLER_OUTPUT_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_6, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_6, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_WAVEFORM_FILLER_DELAY_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_6, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;
                temp_buf |= 0B10000000;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_6, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_HAPTIC::Device::HAPTIC_WAVEFORM_FILLER_7_MODE:
        switch (state)
        {
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_WAVEFORM_FILLER_OUTPUT_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_7, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_7, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_WAVEFORM_FILLER_DELAY_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_7, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;
                temp_buf |= 0B10000000;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_7, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_HAPTIC::Device::HAPTIC_WAVEFORM_FILLER_8_MODE:
        switch (state)
        {
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_WAVEFORM_FILLER_OUTPUT_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_8, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_8, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_WAVEFORM_FILLER_DELAY_MODE:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_8, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;
                temp_buf |= 0B10000000;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_8, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_HAPTIC::Device::HAPTIC_PWM_SAMPLE_RATE_MODE:
        switch (state)
        {
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_PWM_SAMPLE_RATE_48KB:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_PWM_DEBUG, &temp_buf) == true)
            {
                temp_buf &= 0B10011111;
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_PWM_DEBUG, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_PWM_SAMPLE_RATE_24KB:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_PWM_DEBUG, &temp_buf) == true)
            {
                temp_buf &= 0B10011111;
                temp_buf |= 0B01000000;
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_PWM_DEBUG, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        case Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_PWM_SAMPLE_RATE_12KB:
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_PWM_DEBUG, &temp_buf) == true)
            {
                temp_buf &= 0B10011111;
                temp_buf |= 0B01100000;
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_PWM_DEBUG, temp_buf) == true)
                {
                    return true;
                }
            }
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

bool Arduino_AW8624::IIC_Write_Device_Value(uint32_t device, int64_t value)
{
    uint8_t temp_buf = 0;

    switch (device)
    {
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_SET_BRAKE0_LEVEL: // 0-127
        if (value >= 0 && value <= 127)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_BRAKE0_CTRL, &temp_buf) == true)
            {
                temp_buf &= 0B10000000; // 清空
                temp_buf |= value;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_BRAKE0_CTRL, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_SET_BRAKE1_LEVEL: // 0-127
        if (value >= 0 && value <= 127)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_BRAKE1_CTRL, &temp_buf) == true)
            {
                temp_buf &= 0B10000000; // 清空
                temp_buf |= value;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_BRAKE1_CTRL, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_SET_BRAKE2_LEVEL: // 0-127
        if (value >= 0 && value <= 127)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_BRAKE2_CTRL, &temp_buf) == true)
            {
                temp_buf &= 0B10000000; // 清空
                temp_buf |= value;

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_BRAKE2_CTRL, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_SET_BRAKE0_P_NUM: // 1-8
        if (value >= 1 && value <= 8)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_BRAKE_NUMBER_CTRL, &temp_buf) == true)
            {
                temp_buf &= 0B11111000; // 清空
                temp_buf |= (value - 1);

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_BRAKE_NUMBER_CTRL, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_SET_BRAKE1_P_NUM: // 1-8
        if (value >= 1 && value <= 8)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_BRAKE_NUMBER_CTRL, &temp_buf) == true)
            {
                temp_buf &= 0B11000111; // 清空
                temp_buf |= ((value - 1) << 3);

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_BRAKE_NUMBER_CTRL, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_SET_BRAKE2_P_NUM: // 1-4
        if (value >= 1 && value <= 4)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_BRAKE_NUMBER_CTRL, &temp_buf) == true)
            {
                temp_buf &= 0B00111111; // 清空
                temp_buf |= ((value - 1) << 6);

                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_BRAKE_NUMBER_CTRL, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_GAIN_VALUE: // 0-255
        if (value >= 0 && value <= 255)
        {
            if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_DATA_GAIN, value) == true)
            {
                return true;
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_SET_SRAM_BASE_ADDRESS: // 0x0000-0x1FFF
        if (value >= 0 && value <= 0x1FFF)
        {
            temp_buf = (uint8_t)(value >> 8);
            if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_BASE_SRAM_ADDRESS_HIGH, temp_buf) == true)
            {
                temp_buf = (uint8_t)value;
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_BASE_SRAM_ADDRESS_LOW, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_SET_SRAM_ADDRESS: // 0x0000-0x1FFF
        if (value >= 0 && value <= 0x1FFF)
        {
            temp_buf = (uint8_t)(value >> 8);
            if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_SRAM_ADDRESS_HIGH, temp_buf) == true)
            {
                temp_buf = (uint8_t)value;
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_SRAM_ADDRESS_LOW, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_WAVEFORM_FILLER_1_OUTPUT_NUMBER_DELAY: // 0-127
        if (value >= 0 && value <= 127)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_1, &temp_buf) == true)
            {
                temp_buf &= 0B10000000;
                temp_buf |= value;
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_1, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_WAVEFORM_FILLER_2_OUTPUT_NUMBER_DELAY: // 0-127
        if (value >= 0 && value <= 127)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_2, &temp_buf) == true)
            {
                temp_buf &= 0B10000000;
                temp_buf |= value;
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_2, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_WAVEFORM_FILLER_3_OUTPUT_NUMBER_DELAY: // 0-127
        if (value >= 0 && value <= 127)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_3, &temp_buf) == true)
            {
                temp_buf &= 0B10000000;
                temp_buf |= value;
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_3, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_WAVEFORM_FILLER_4_OUTPUT_NUMBER_DELAY: // 0-127
        if (value >= 0 && value <= 127)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_4, &temp_buf) == true)
            {
                temp_buf &= 0B10000000;
                temp_buf |= value;
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_4, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_WAVEFORM_FILLER_5_OUTPUT_NUMBER_DELAY: // 0-127
        if (value >= 0 && value <= 127)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_5, &temp_buf) == true)
            {
                temp_buf &= 0B10000000;
                temp_buf |= value;
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_5, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_WAVEFORM_FILLER_6_OUTPUT_NUMBER_DELAY: // 0-127
        if (value >= 0 && value <= 127)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_6, &temp_buf) == true)
            {
                temp_buf &= 0B10000000;
                temp_buf |= value;
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_6, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_WAVEFORM_FILLER_7_OUTPUT_NUMBER_DELAY: // 0-127
        if (value >= 0 && value <= 127)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_7, &temp_buf) == true)
            {
                temp_buf &= 0B10000000;
                temp_buf |= value;
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_7, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_WAVEFORM_FILLER_8_OUTPUT_NUMBER_DELAY: // 0-127
        if (value >= 0 && value <= 127)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_8, &temp_buf) == true)
            {
                temp_buf &= 0B10000000;
                temp_buf |= value;
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_8, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_WAVEFORM_FILLER_1_OUTPUT_PLAYBACK_TIMES: // 1-16 设置16为无限循环
        if (value >= 1 && value <= 16)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_1, &temp_buf) == true)
            {
                temp_buf &= 0B00001111;
                temp_buf = temp_buf | ((value - 1) << 4);
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_1, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_WAVEFORM_FILLER_2_OUTPUT_PLAYBACK_TIMES: // 1-16 设置16为无限循环
        if (value >= 1 && value <= 16)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_1, &temp_buf) == true)
            {
                temp_buf &= 0B11110000;
                temp_buf = temp_buf | (value - 1);
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_1, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_WAVEFORM_FILLER_3_OUTPUT_PLAYBACK_TIMES: // 1-16 设置16为无限循环
        if (value >= 1 && value <= 16)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_2, &temp_buf) == true)
            {
                temp_buf &= 0B00001111;
                temp_buf = temp_buf | ((value - 1) << 4);
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_2, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_WAVEFORM_FILLER_4_OUTPUT_PLAYBACK_TIMES: // 1-16 设置16为无限循环
        if (value >= 1 && value <= 16)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_2, &temp_buf) == true)
            {
                temp_buf &= 0B11110000;
                temp_buf = temp_buf | (value - 1);
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_2, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_WAVEFORM_FILLER_5_OUTPUT_PLAYBACK_TIMES: // 1-16 设置16为无限循环
        if (value >= 1 && value <= 16)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_3, &temp_buf) == true)
            {
                temp_buf &= 0B00001111;
                temp_buf = temp_buf | ((value - 1) << 4);
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_3, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_WAVEFORM_FILLER_6_OUTPUT_PLAYBACK_TIMES: // 1-16 设置16为无限循环
        if (value >= 1 && value <= 16)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_3, &temp_buf) == true)
            {
                temp_buf &= 0B11110000;
                temp_buf = temp_buf | (value - 1);
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_3, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_WAVEFORM_FILLER_7_OUTPUT_PLAYBACK_TIMES: // 1-16 设置16为无限循环
        if (value >= 1 && value <= 16)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_4, &temp_buf) == true)
            {
                temp_buf &= 0B00001111;
                temp_buf = temp_buf | ((value - 1) << 4);
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_4, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_WAVEFORM_FILLER_8_OUTPUT_PLAYBACK_TIMES: // 1-16 设置16为无限循环
        if (value >= 1 && value <= 16)
        {
            if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_4, &temp_buf) == true)
            {
                temp_buf &= 0B11110000;
                temp_buf = temp_buf | (value - 1);
                if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_4, temp_buf) == true)
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

bool Arduino_AW8624::IIC_Write_Device_Data(uint32_t device, const uint8_t *data, size_t length)
{
    switch (device)
    {
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_RTP_DATA: // 0x00-0xFF
        if (_bus->IIC_WriteC8_Data(_device_address, AW8624_RD_WR_RTP_DATA, data, length) == true)
        {
            return true;
        }
        break;
    case Arduino_IIC_HAPTIC::Device_Value::HAPTIC_SRAM_DATA: // 0x00-0xFF
        if (_bus->IIC_WriteC8_Data(_device_address, AW8624_RD_WR_SRAM_DATA, data, length) == true)
        {
            return true;
        }
        break;

    default:
        break;
    }

    return false;
}

String Arduino_AW8624::IIC_Read_Device_State(uint32_t information)
{
    uint8_t temp_buf = 0;

    switch (information)
    {
    case Arduino_IIC_HAPTIC::Status_Information::HAPTIC_OVS_STATUS:
        if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_SYSTEM_STATUS, &temp_buf) == true)
        {
            temp_buf &= 0B01000000;

            switch (temp_buf)
            {
            case 0B00000000:
                return "Normal";
                break;
            case 0B01000000:
                return "Wave data Overflow Or DPWM DC Error";
                break;

            default:
                break;
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Status_Information::HAPTIC_UVLS_STATUS:
        if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_SYSTEM_STATUS, &temp_buf) == true)
        {
            temp_buf &= 0B00100000;

            switch (temp_buf)
            {
            case 0B00000000:
                return "Normal"; // VDD>UVLO_THRES(2.7V)
                break;
            case 0B00100000:
                return "Under Voltage Lock Out"; // VDD<UVLO_THRES(2.7V)
                break;

            default:
                break;
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Status_Information::HAPTIC_RTP_FIFO_MEMORY_STATUS:
        if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_SYSTEM_STATUS, &temp_buf) == true)
        {
            temp_buf &= 0B00011000;

            switch (temp_buf)
            {
            case 0B00000000:
                return "Normal";
                break;
            case 0B00010000:
                return "RTP FIFO Almost Empty";
                break;
            case 0B00001000:
                return "RTP FIFO Almost Full";
                break;

            default:
                break;
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Status_Information::HAPTIC_OCDS_STATUS:
        if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_SYSTEM_STATUS, &temp_buf) == true)
        {
            temp_buf &= 0B00000100;

            switch (temp_buf)
            {
            case 0B00000000:
                return "Normal";
                break;
            case 0B00000100:
                return "Over Current";
                break;

            default:
                break;
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Status_Information::HAPTIC_OTS_STATUS:
        if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_SYSTEM_STATUS, &temp_buf) == true)
        {
            temp_buf &= 0B00000010;

            switch (temp_buf)
            {
            case 0B00000000:
                return "Normal";
                break;
            case 0B00000010:
                return "Over Temperature";
                break;

            default:
                break;
            }
        }
        break;
    case Arduino_IIC_HAPTIC::Status_Information::HAPTIC_DONES_STATUS:
        if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_SYSTEM_STATUS, &temp_buf) == true)
        {
            temp_buf &= 0B00000001;

            switch (temp_buf)
            {
            case 0B00000000:
                return "Normal";
                break;
            case 0B00000001:
                return "The Indication Of Playback Done";
                break;

            default:
                break;
            }
        }
        break;

    default:
        break;
    }

    return "->Error reading IIC_Read_Information";
}

double Arduino_AW8624::IIC_Read_Device_Value(uint32_t information)
{
    uint8_t temp_buf = 0;
    uint8_t temp_buf_2 = 0;

    switch (information)
    {
    case Arduino_IIC_HAPTIC::Value_Information::HAPTIC_CHIP_INPUT_VOLTAGE:
        if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_DETECTION_CONTROL, &temp_buf) == true)
        {
            temp_buf &= 0B11111101;
            temp_buf |= 0B00000010;
            if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_DETECTION_CONTROL, temp_buf) == true) // 开启电压测量
            {
                if (_bus->IIC_ReadC8D8(_device_address, AW8624_RD_WR_DETECTION_VBAT, &temp_buf_2) == true)
                {
                    temp_buf &= 0B11111101;
                    if (_bus->IIC_WriteC8D8(_device_address, AW8624_RD_WR_DETECTION_CONTROL, temp_buf) == true) // 关闭电压测量
                    {
                        return (6100.0 * (double)temp_buf_2) / 256.0;
                    }
                }
            }
        }

        break;

    default:
        break;
    }
    return -1;
}
