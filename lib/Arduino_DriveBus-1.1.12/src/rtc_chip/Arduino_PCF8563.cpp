/*
 * @Description: PCF8563.cpp
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2023-11-16 15:42:22
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-06-06 17:38:30
 * @License: GPL 3.0
 */
#include "Arduino_PCF8563.h"

Arduino_PCF8563::Arduino_PCF8563(
    std::shared_ptr<Arduino_IIC_DriveBus> bus, uint8_t device_address, int8_t rst, int8_t iqr,
    void (*Interrupt_Function)())
    : Arduino_IIC(bus, device_address, rst, iqr, Interrupt_Function)
{
}

bool Arduino_PCF8563::begin(int32_t speed)
{
    return Arduino_IIC::begin(speed);
}

bool Arduino_PCF8563::IIC_Initialization(void)
{
    if (_rst != DRIVEBUS_DEFAULT_VALUE)
    {
    }
    else
    {
        // Software Rest Or NULL
    }

    if (_bus->BufferOperation(_device_address, PCF8563_Initialization_BufferOperations,
                              sizeof(PCF8563_Initialization_BufferOperations)) == false)
    {
        return false;
    }

    return true;
}

int32_t Arduino_PCF8563::IIC_Device_ID(void)
{
    uint8_t temp_buf = 0;
    if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_DEVICE_ID, &temp_buf) == true)
    {
        return (int32_t)temp_buf;
    }
    return -1;
}

bool Arduino_PCF8563::IIC_Write_Device_State(uint32_t device, uint8_t state)
{
    uint8_t temp_buf = 0;

    switch (device)
    {
    case Arduino_IIC_RTC::Device::RTC_CLOCK_RTC:
        switch (state)
        {
        case Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_1, &temp_buf) == true)
            {
                temp_buf &= 0B11011111;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_1, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_RTC::Device_State::RTC_DEVICE_OFF:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_1, &temp_buf) == true)
            {
                temp_buf |= 0B00100000;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_1, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_RTC::Device::RTC_CLOCK_OUTPUT_VALUE:
        switch (state)
        {
        case Arduino_IIC_RTC::Device_Mode::RTC_CLOCK_OUTPUT_36768:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CLKOUT_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B01111100;
                temp_buf |= 0B10000000;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_CLKOUT_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_RTC::Device_Mode::RTC_CLOCK_OUTPUT_1024:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CLKOUT_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B01111100;
                temp_buf |= 0B10000001;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_CLKOUT_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_RTC::Device_Mode::RTC_CLOCK_OUTPUT_32:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CLKOUT_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B01111100;
                temp_buf |= 0B10000010;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_CLKOUT_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_RTC::Device_Mode::RTC_CLOCK_OUTPUT_1:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CLKOUT_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B01111100;
                temp_buf |= 0B10000011;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_CLKOUT_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_RTC::Device_Mode::RTC_CLOCK_OUTPUT_OFF:

            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CLKOUT_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_CLKOUT_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_RTC::Device::RTC_INT_AIE:
        switch (state)
        {
        case Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_2, &temp_buf) == true)
            {
                temp_buf &= 0B11111101;
                temp_buf |= 0B00000010;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_2, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_RTC::Device_State::RTC_DEVICE_OFF:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_2, &temp_buf) == true)
            {
                temp_buf &= 0B11111101;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_2, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_RTC::Device::RTC_INT_TIE:
        switch (state)
        {
        case Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_2, &temp_buf) == true)
            {
                temp_buf &= 0B11111110;
                temp_buf |= 0B00000001;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_2, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_RTC::Device_State::RTC_DEVICE_OFF:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_2, &temp_buf) == true)
            {
                temp_buf &= 0B11111110;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_2, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_RTC::Device::RTC_TIMER_FREQUENCY_VALUE:
        switch (state)
        {
        case Arduino_IIC_RTC::Device_Mode::RTC_TIMER_FREQUENCY_4096:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_TIMER_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B11111100;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_TIMER_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_RTC::Device_Mode::RTC_TIMER_FREQUENCY_64:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_TIMER_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B11111100;
                temp_buf |= 0B00000001;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_TIMER_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_RTC::Device_Mode::RTC_TIMER_FREQUENCY_1:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_TIMER_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B11111100;
                temp_buf |= 0B00000010;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_TIMER_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_RTC::Device_Mode::RTC_TIMER_FREQUENCY_1_60:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_TIMER_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B11111100;
                temp_buf |= 0B00000011;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_TIMER_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_RTC::Device::RTC_TIMER:
        switch (state)
        {
        case Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_TIMER_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;
                temp_buf |= 0B10000000;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_TIMER_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_RTC::Device_State::RTC_DEVICE_OFF:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_TIMER_CONTROL, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_TIMER_CONTROL, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_RTC::Device::RTC_AE_MINUTE_ALARM:
        switch (state)
        {
        case Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_MINUTE_ALARM, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_MINUTE_ALARM, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_RTC::Device_State::RTC_DEVICE_OFF:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_MINUTE_ALARM, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;
                temp_buf |= 0B10000000;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_MINUTE_ALARM, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_RTC::Device::RTC_AE_HOUR_ALARM:
        switch (state)
        {
        case Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_HOUR_ALARM, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_HOUR_ALARM, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_RTC::Device_State::RTC_DEVICE_OFF:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_HOUR_ALARM, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;
                temp_buf |= 0B10000000;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_HOUR_ALARM, temp_buf) == true)
                {
                    return true;
                }
            }
            break;

        default:
            break;
        }
        break;
    case Arduino_IIC_RTC::Device::RTC_AE_DAY_ALARM:
        switch (state)
        {
        case Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_DAY_ALARM, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_DAY_ALARM, temp_buf) == true)
                {
                    return true;
                }
            }
            break;
        case Arduino_IIC_RTC::Device_State::RTC_DEVICE_OFF:
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_DAY_ALARM, &temp_buf) == true)
            {
                temp_buf &= 0B01111111;
                temp_buf |= 0B10000000;

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_DAY_ALARM, temp_buf) == true)
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

bool Arduino_PCF8563::IIC_Write_Device_Value(uint32_t device, int64_t value)
{
    uint8_t temp_buf = 0;

    switch (device)
    {
    case Arduino_IIC_RTC::Device_Value::RTC_SET_SECOND_DATA: // 0-59
        if (value >= 0 && value <= 59)
        {
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_VL_SECONDS, &temp_buf) == true)
            {
                temp_buf &= 0B10000000; // 清空
                temp_buf = temp_buf | (((value / 10) << 4) | (value % 10));

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_VL_SECONDS, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_RTC::Device_Value::RTC_SET_MINUTE_DATA: // 0-59
        if (value >= 0 && value <= 59)
        {
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_MINUTES, &temp_buf) == true)
            {
                temp_buf &= 0B10000000; // 清空
                temp_buf = temp_buf | (((value / 10) << 4) | (value % 10));

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_MINUTES, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_RTC::Device_Value::RTC_SET_HOUR_DATA: // 0-23
        if (value >= 0 && value <= 23)
        {
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_HOURS, &temp_buf) == true)
            {
                temp_buf &= 0B11000000; // 清空
                temp_buf = temp_buf | (((value / 10) << 4) | (value % 10));

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_HOURS, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_RTC::Device_Value::RTC_SET_DAY_DATA: // 1-31
        if (value >= 1 && value <= 31)
        {
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_DAYS, &temp_buf) == true)
            {
                temp_buf &= 0B11000000; // 清空
                temp_buf = temp_buf | (((value / 10) << 4) | (value % 10));

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_DAYS, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_RTC::Device_Value::RTC_SET_MONTH_DATA: // 1-12
        if (value >= 1 && value <= 12)
        {
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CENTURY_MONTHS, &temp_buf) == true)
            {
                temp_buf &= 0B11100000; // 清空
                temp_buf = temp_buf | (((value / 10) << 4) | (value % 10));

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_CENTURY_MONTHS, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_RTC::Device_Value::RTC_SET_YEAR_DATA: // 0-99
        if (value >= 0 && value <= 99)
        {
            temp_buf = (((value / 10) << 4) | (value % 10));

            if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_YEARS, temp_buf) == true)
            {
                return true;
            }
        }
        break;

    case Arduino_IIC_RTC::Device_Value::RTC_TIMER_N_VALUE: // 0-255
        if (value >= 0 && value <= 255)
        {
            temp_buf = value;

            if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_TIMER, temp_buf) == true)
            {
                return true;
            }
        }
        break;
    case Arduino_IIC_RTC::Device_Value::RTC_TIMER_FLAG_AF: // 0
        if (value == 0)
        {
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_2, &temp_buf) == true)
            {
                temp_buf &= 0B11110111; // 清空

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_2, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_RTC::Device_Value::RTC_TIMER_FLAG_TF: // 0
        if (value == 0)
        {
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_2, &temp_buf) == true)
            {
                temp_buf &= 0B11111011; // 清空

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_2, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_RTC::Device_Value::RTC_SET_ALARM_MINUTE_DATA: // 0-59
        if (value >= 0 && value <= 59)
        {
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_MINUTE_ALARM, &temp_buf) == true)
            {
                temp_buf &= 0B10000000; // 清空
                temp_buf = temp_buf | (((value / 10) << 4) | (value % 10));

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_MINUTE_ALARM, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_RTC::Device_Value::RTC_SET_ALARM_HOUR_DATA: // 0-23
        if (value >= 0 && value <= 23)
        {
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_HOUR_ALARM, &temp_buf) == true)
            {
                temp_buf &= 0B10000000; // 清空
                temp_buf = temp_buf | (((value / 10) << 4) | (value % 10));

                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_HOUR_ALARM, temp_buf) == true)
                {
                    return true;
                }
            }
        }
        break;
    case Arduino_IIC_RTC::Device_Value::RTC_SET_ALARM_DAY_DATA: // 1-31
        if (value >= 1 && value <= 31)
        {
            if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_DAY_ALARM, &temp_buf) == true)
            {
                temp_buf &= 0B11000000; // 清空
                temp_buf = temp_buf | (((value / 10) << 4) | (value % 10));
                if (_bus->IIC_WriteC8D8(_device_address, PCF8563_RD_WR_DAY_ALARM, temp_buf) == true)
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

String Arduino_PCF8563::IIC_Read_Device_State(uint32_t information)
{
    uint8_t temp_buf = 0;

    switch (information)
    {
    case Arduino_IIC_RTC::Status_Information::RTC_WEEKDAYS_DATA:
        if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_WEEKDAY, &temp_buf) == true)
        {
            temp_buf &= 0B00000111;

            switch (temp_buf)
            {
            case 0B00000000:
                return "Sunday";
                break;
            case 0B00000001:
                return "Monday";
                break;
            case 0B00000010:
                return "Tuesday";
                break;
            case 0B00000011:
                return "Wednesday";
                break;
            case 0B00000100:
                return "Thursday";
                break;
            case 0B00000101:
                return "Friday";
                break;
            case 0B00000110:
                return "Saturday";
                break;

            default:
                break;
            }

            return "->Read PCF8563_RD_WR_WEEKDAY fail";
        }
        break;

    default:
        break;
    }

    return "->Error reading IIC_Read_Information";
}

double Arduino_PCF8563::IIC_Read_Device_Value(uint32_t information)
{
    uint8_t temp_buf = 0;

    switch (information)
    {
    case Arduino_IIC_RTC::Value_Information::RTC_SECONDS_DATA: // 0-59
        if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_VL_SECONDS, &temp_buf) == true)
        {
            return ((((temp_buf & 0B01110000) >> 4) * 10) + (temp_buf & 0B00001111));
        }
        break;
    case Arduino_IIC_RTC::Value_Information::RTC_MINUTES_DATA: // 0-59
        if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_MINUTES, &temp_buf) == true)
        {
            return ((((temp_buf & 0B01110000) >> 4) * 10) + (temp_buf & 0B00001111));
        }
        break;
    case Arduino_IIC_RTC::Value_Information::RTC_HOURS_DATA: // 0-23
        if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_HOURS, &temp_buf) == true)
        {
            return ((((temp_buf & 0B00110000) >> 4) * 10) + (temp_buf & 0B00001111));
        }
        break;
    case Arduino_IIC_RTC::Value_Information::RTC_DAYS_DATA: // 1-31
        if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_DAYS, &temp_buf) == true)
        {
            return ((((temp_buf & 0B00110000) >> 4) * 10) + (temp_buf & 0B00001111));
        }
        break;
    case Arduino_IIC_RTC::Value_Information::RTC_MONTHS_DATA: // 1-12
        if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CENTURY_MONTHS, &temp_buf) == true)
        {
            return ((((temp_buf & 0B00010000) >> 4) * 10) + (temp_buf & 0B00001111));
        }
        break;
    case Arduino_IIC_RTC::Value_Information::RTC_YEARS_DATA: // 0-99
        if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_YEARS, &temp_buf) == true)
        {
            return ((((temp_buf & 0B11110000) >> 4) * 10) + (temp_buf & 0B00001111));
        }
        break;
    case Arduino_IIC_RTC::Value_Information::RTC_ALARM_FLAG_AF_INFORMATION: // 0-1
        if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_2, &temp_buf) == true)
        {
            return ((temp_buf & 0B00001000) >> 3);
        }
        break;
    case Arduino_IIC_RTC::Value_Information::RTC_TIMER_FLAG_TF_INFORMATION: // 0-1
        if (_bus->IIC_ReadC8D8(_device_address, PCF8563_RD_WR_CONTROL_STATUS_2, &temp_buf) == true)
        {
            return ((temp_buf & 0B00000100) >> 2);
        }
        break;

    default:
        break;
    }
    return -1;
}
