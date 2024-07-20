/*
 * @Description: PCF85063示例程序，向串口打印RTC时间数据
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2024-03-27 10:02:26
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-06-06 18:26:39
 * @License: GPL 3.0
 */

#include "Arduino_DriveBus_Library.h"

static size_t CycleTime = 0;
static int8_t temp_count = -1;

std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
    std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire);

void Arduino_IIC_Touch_Interrupt(void);

std::unique_ptr<Arduino_IIC> PCF85063(new Arduino_PCF85063(IIC_Bus, PCF85063_DEVICE_ADDRESS,
                                                           DRIVEBUS_DEFAULT_VALUE, PCF85063_INT, Arduino_IIC_Touch_Interrupt));

void Arduino_IIC_Touch_Interrupt(void)
{
    PCF85063->IIC_Interrupt_Flag = true;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Ciallo");

    pinMode(0, INPUT_PULLUP);

    while (PCF85063->begin() == false)
    {
        Serial.println("PCF85063 initialization fail");
        delay(2000);
    }
    Serial.println("PCF85063 initialization successfully");

    // 设置时间格式为24小时制
    PCF85063->IIC_Write_Device_State(PCF85063->Arduino_IIC_RTC::Device::RTC_CLOCK_TIME_FORMAT,
                                     PCF85063->Arduino_IIC_RTC::Device_Mode::RTC_CLOCK_TIME_FORMAT_24);

    // 关闭时钟输出
    PCF85063->IIC_Write_Device_State(PCF85063->Arduino_IIC_RTC::Device::RTC_CLOCK_OUTPUT_VALUE,
                                     PCF85063->Arduino_IIC_RTC::Device_Mode::RTC_CLOCK_OUTPUT_OFF);

    // 开启RTC
    PCF85063->IIC_Write_Device_State(PCF85063->Arduino_IIC_RTC::Device::RTC_CLOCK_RTC,
                                     PCF85063->Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON);

    Serial.printf("ID: %#X \n\n", (int32_t)PCF85063->IIC_Device_ID());
    delay(1000);
}

void loop()
{
    if (millis() > CycleTime)
    {
        Serial.printf("--------------------PCF85063--------------------\n");

        Serial.printf("System running time: %d\n\n", (uint32_t)millis() / 1000);
        Serial.printf("IIC_Bus.use_count(): %d\n\n", (int32_t)IIC_Bus.use_count());

        Serial.printf("ID: %#X \n", (int32_t)PCF85063->IIC_Device_ID());

        Serial.printf("PCF85063  Weekday: %s\n",
                      PCF85063->IIC_Read_Device_State(PCF85063->Arduino_IIC_RTC::Status_Information::RTC_WEEKDAYS_DATA).c_str());
        Serial.printf("PCF85063  Year: %d\n",
                      (int32_t)PCF85063->IIC_Read_Device_Value(PCF85063->Arduino_IIC_RTC::Value_Information::RTC_YEARS_DATA));
        Serial.printf("PCF85063 Date: %d.%d\n",
                      (int32_t)PCF85063->IIC_Read_Device_Value(PCF85063->Arduino_IIC_RTC::Value_Information::RTC_MONTHS_DATA),
                      (int32_t)PCF85063->IIC_Read_Device_Value(PCF85063->Arduino_IIC_RTC::Value_Information::RTC_DAYS_DATA));
        Serial.printf("PCF85063 Time: %d:%d:%d\n",
                      (int32_t)PCF85063->IIC_Read_Device_Value(PCF85063->Arduino_IIC_RTC::Value_Information::RTC_HOURS_DATA),
                      (int32_t)PCF85063->IIC_Read_Device_Value(PCF85063->Arduino_IIC_RTC::Value_Information::RTC_MINUTES_DATA),
                      (int32_t)PCF85063->IIC_Read_Device_Value(PCF85063->Arduino_IIC_RTC::Value_Information::RTC_SECONDS_DATA));

        Serial.printf("--------------------PCF85063--------------------\n");

        CycleTime = millis() + 1000;
    }

    if (digitalRead(0) == LOW)
    {
        delay(300);

        temp_count++;
        switch (temp_count)
        {
        case 0:
            // 关闭时钟输出
            PCF85063->IIC_Write_Device_State(PCF85063->Arduino_IIC_RTC::Device::RTC_CLOCK_OUTPUT_VALUE,
                                             PCF85063->Arduino_IIC_RTC::Device_Mode::RTC_CLOCK_OUTPUT_OFF);
            break;
        case 1:
            PCF85063->IIC_Write_Device_State(PCF85063->Arduino_IIC_RTC::Device::RTC_CLOCK_OUTPUT_VALUE,
                                             PCF85063->Arduino_IIC_RTC::Device_Mode::RTC_CLOCK_OUTPUT_36768);
            break;
        case 2:
            PCF85063->IIC_Write_Device_State(PCF85063->Arduino_IIC_RTC::Device::RTC_CLOCK_OUTPUT_VALUE,
                                             PCF85063->Arduino_IIC_RTC::Device_Mode::RTC_CLOCK_OUTPUT_16384);
            break;
        case 3:
            PCF85063->IIC_Write_Device_State(PCF85063->Arduino_IIC_RTC::Device::RTC_CLOCK_OUTPUT_VALUE,
                                             PCF85063->Arduino_IIC_RTC::Device_Mode::RTC_CLOCK_OUTPUT_8192);
            break;
        case 4:
            PCF85063->IIC_Write_Device_State(PCF85063->Arduino_IIC_RTC::Device::RTC_CLOCK_OUTPUT_VALUE,
                                             PCF85063->Arduino_IIC_RTC::Device_Mode::RTC_CLOCK_OUTPUT_4096);
            break;
        case 5:
            PCF85063->IIC_Write_Device_State(PCF85063->Arduino_IIC_RTC::Device::RTC_CLOCK_OUTPUT_VALUE,
                                             PCF85063->Arduino_IIC_RTC::Device_Mode::RTC_CLOCK_OUTPUT_2048);
            break;
        case 6:
            PCF85063->IIC_Write_Device_State(PCF85063->Arduino_IIC_RTC::Device::RTC_CLOCK_OUTPUT_VALUE,
                                             PCF85063->Arduino_IIC_RTC::Device_Mode::RTC_CLOCK_OUTPUT_1024);
            break;
        case 7:
            PCF85063->IIC_Write_Device_State(PCF85063->Arduino_IIC_RTC::Device::RTC_CLOCK_OUTPUT_VALUE,
                                             PCF85063->Arduino_IIC_RTC::Device_Mode::RTC_CLOCK_OUTPUT_1);
            break;

        default:
            break;
        }

        if (temp_count > 7)
        {
            temp_count = -1;
        }

        // 关闭RTC
        PCF85063->IIC_Write_Device_State(PCF85063->Arduino_IIC_RTC::Device::RTC_CLOCK_RTC,
                                         PCF85063->Arduino_IIC_RTC::Device_State::RTC_DEVICE_OFF);
        // 时钟传感器设置秒
        PCF85063->IIC_Write_Device_Value(PCF85063->Arduino_IIC_RTC::Device_Value::RTC_SET_SECOND_DATA,
                                         58);
        // 时钟传感器设置分
        PCF85063->IIC_Write_Device_Value(PCF85063->Arduino_IIC_RTC::Device_Value::RTC_SET_MINUTE_DATA,
                                         59);
        // 时钟传感器设置时
        PCF85063->IIC_Write_Device_Value(PCF85063->Arduino_IIC_RTC::Device_Value::RTC_SET_HOUR_DATA,
                                         23);
        // 时钟传感器设置天
        PCF85063->IIC_Write_Device_Value(PCF85063->Arduino_IIC_RTC::Device_Value::RTC_SET_DAY_DATA,
                                         31);
        // 时钟传感器设置月
        PCF85063->IIC_Write_Device_Value(PCF85063->Arduino_IIC_RTC::Device_Value::RTC_SET_MONTH_DATA,
                                         12);
        // 时钟传感器设置年
        PCF85063->IIC_Write_Device_Value(PCF85063->Arduino_IIC_RTC::Device_Value::RTC_SET_YEAR_DATA,
                                         99);
        // 开启RTC
        PCF85063->IIC_Write_Device_State(PCF85063->Arduino_IIC_RTC::Device::RTC_CLOCK_RTC,
                                         PCF85063->Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON);
    }
}