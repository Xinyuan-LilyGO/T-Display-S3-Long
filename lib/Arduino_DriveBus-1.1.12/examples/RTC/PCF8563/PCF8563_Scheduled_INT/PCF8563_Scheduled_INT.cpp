/*
 * @Description: PCF8567计划中断示例程序
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2024-03-27 10:02:26
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-06-06 18:24:02
 * @License: GPL 3.0
 */
#include "Arduino_DriveBus_Library.h"

static size_t CycleTime = 0;

std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
    std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire);

void Arduino_IIC_Touch_Interrupt(void);

std::unique_ptr<Arduino_IIC> PCF8563(new Arduino_PCF8563(IIC_Bus, PCF8563_DEVICE_ADDRESS,
                                                         DRIVEBUS_DEFAULT_VALUE, PCF8563_INT, Arduino_IIC_Touch_Interrupt));

void Arduino_IIC_Touch_Interrupt(void)
{
    PCF8563->IIC_Interrupt_Flag = true;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Ciallo");

    while (PCF8563->begin() == false)
    {
        Serial.println("PCF8563 initialization fail");
        delay(2000);
    }
    Serial.println("PCF8563 initialization successfully");

    // 关闭时钟输出
    PCF8563->IIC_Write_Device_State(PCF8563->Arduino_IIC_RTC::Device::RTC_CLOCK_OUTPUT_VALUE,
                                    PCF8563->Arduino_IIC_RTC::Device_Mode::RTC_CLOCK_OUTPUT_OFF);

    // 关闭RTC
    PCF8563->IIC_Write_Device_State(PCF8563->Arduino_IIC_RTC::Device::RTC_CLOCK_RTC,
                                    PCF8563->Arduino_IIC_RTC::Device_State::RTC_DEVICE_OFF);

    // 清除定时器标志AF
    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_TIMER_FLAG_AF,
                                    0);

    // 调整时钟时间
    // 时钟传感器设置秒
    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_SET_SECOND_DATA,
                                    58);
    // 时钟传感器设置分
    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_SET_MINUTE_DATA,
                                    59);
    // 时钟传感器设置时
    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_SET_HOUR_DATA,
                                    23);
    // 时钟传感器设置天
    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_SET_DAY_DATA,
                                    31);
    // 时钟传感器设置月
    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_SET_MONTH_DATA,
                                    12);
    // 时钟传感器设置年
    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_SET_YEAR_DATA,
                                    99);

    // 设置计划的时间
    // 时钟传感器设置报警分
    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_SET_ALARM_MINUTE_DATA,
                                    1);
    // 时钟传感器设置报警时
    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_SET_ALARM_HOUR_DATA,
                                    0);
    // 时钟传感器设置报警天
    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_SET_ALARM_DAY_DATA,
                                    1);

    // 开启报警
    // 开启分钟报警
    PCF8563->IIC_Write_Device_State(PCF8563->Arduino_IIC_RTC::Device::RTC_AE_MINUTE_ALARM,
                                    PCF8563->Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON);
    // 开启小时报警
    PCF8563->IIC_Write_Device_State(PCF8563->Arduino_IIC_RTC::Device::RTC_AE_HOUR_ALARM,
                                    PCF8563->Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON);
    // 开启天报警
    PCF8563->IIC_Write_Device_State(PCF8563->Arduino_IIC_RTC::Device::RTC_AE_DAY_ALARM,
                                    PCF8563->Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON);

    // 开启计划报警中断
    PCF8563->IIC_Write_Device_State(PCF8563->Arduino_IIC_RTC::Device::RTC_INT_AIE,
                                    PCF8563->Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON);

    // 开启RTC
    PCF8563->IIC_Write_Device_State(PCF8563->Arduino_IIC_RTC::Device::RTC_CLOCK_RTC,
                                    PCF8563->Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON);

    Serial.printf("ID: %#X \n\n", (int32_t)PCF8563->IIC_Device_ID());
    Serial.println("Timer Start");
}

void loop()
{
    if (millis() > CycleTime)
    {
        Serial.printf("--------------------PCF8563--------------------\n");

        Serial.printf("System running time: %d\n\n", (uint32_t)millis() / 1000);
        Serial.printf("IIC_Bus.use_count(): %d\n\n", (int32_t)IIC_Bus.use_count());

        Serial.printf("ID: %#X \n", (int32_t)PCF8563->IIC_Device_ID());

        Serial.printf("PCF8563  Weekday: %s\n",
                      PCF8563->IIC_Read_Device_State(PCF8563->Arduino_IIC_RTC::Status_Information::RTC_WEEKDAYS_DATA).c_str());
        Serial.printf("PCF8563  Year: %d\n",
                      (int32_t)PCF8563->IIC_Read_Device_Value(PCF8563->Arduino_IIC_RTC::Value_Information::RTC_YEARS_DATA));
        Serial.printf("PCF8563 Date: %d.%d\n",
                      (int32_t)PCF8563->IIC_Read_Device_Value(PCF8563->Arduino_IIC_RTC::Value_Information::RTC_MONTHS_DATA),
                      (int32_t)PCF8563->IIC_Read_Device_Value(PCF8563->Arduino_IIC_RTC::Value_Information::RTC_DAYS_DATA));
        Serial.printf("PCF8563 Time: %d:%d:%d\n",
                      (int32_t)PCF8563->IIC_Read_Device_Value(PCF8563->Arduino_IIC_RTC::Value_Information::RTC_HOURS_DATA),
                      (int32_t)PCF8563->IIC_Read_Device_Value(PCF8563->Arduino_IIC_RTC::Value_Information::RTC_MINUTES_DATA),
                      (int32_t)PCF8563->IIC_Read_Device_Value(PCF8563->Arduino_IIC_RTC::Value_Information::RTC_SECONDS_DATA));

        Serial.printf("--------------------PCF8563--------------------\n");

        CycleTime = millis() + 1000;
    }

    if (PCF8563->IIC_Interrupt_Flag == true)
    {
        PCF8563->IIC_Interrupt_Flag = false;
        if ((int32_t)PCF8563->IIC_Read_Device_Value(PCF8563->Arduino_IIC_RTC::Value_Information::RTC_ALARM_FLAG_AF_INFORMATION) == 1)
        {
            Serial.println("RTC_ALARM_FLAG_AF_INFORMATION triggered");

            // 清除定时器标志AF
            PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_TIMER_FLAG_AF,
                                            0);
            delay(1000);
        }
        delay(1000);
    }
}