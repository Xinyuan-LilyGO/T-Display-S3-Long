/*
 * @Description: PCF8567定时中断示例程序
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2024-03-27 10:02:26
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-06-06 18:26:25
 * @License: GPL 3.0
 */
#include "Arduino_DriveBus_Library.h"

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

    // 关闭定时器
    PCF8563->IIC_Write_Device_State(PCF8563->Arduino_IIC_RTC::Device::RTC_TIMER,
                                    PCF8563->Arduino_IIC_RTC::Device_State::RTC_DEVICE_OFF);

    // 清除定时器标志TF
    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_TIMER_FLAG_TF,
                                    0);
    // 下面设定10秒定时
    //  设置定时器频率的值为1Hz（选择的频率越高越精准）
    PCF8563->IIC_Write_Device_State(PCF8563->Arduino_IIC_RTC::Device::RTC_TIMER_FREQUENCY_VALUE,
                                    PCF8563->Arduino_IIC_RTC::Device_Mode::RTC_TIMER_FREQUENCY_1);
    // 设置定时器的n值（定时器定时周期的时间=定时器的n值/定时器频率）
    PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_TIMER_N_VALUE,
                                    10);

    // 开启定时中断
    PCF8563->IIC_Write_Device_State(PCF8563->Arduino_IIC_RTC::Device::RTC_INT_TIE,
                                    PCF8563->Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON);

    // 开启定时器
    PCF8563->IIC_Write_Device_State(PCF8563->Arduino_IIC_RTC::Device::RTC_TIMER,
                                    PCF8563->Arduino_IIC_RTC::Device_State::RTC_DEVICE_ON);

    Serial.printf("ID: %#X \n\n", (int32_t)PCF8563->IIC_Device_ID());

    Serial.println("Timer Start");
}

void loop()
{
    if (PCF8563->IIC_Interrupt_Flag == true)
    {
        PCF8563->IIC_Interrupt_Flag = false;
        if ((int32_t)PCF8563->IIC_Read_Device_Value(PCF8563->Arduino_IIC_RTC::Value_Information::RTC_TIMER_FLAG_TF_INFORMATION) == 1)
        {
            Serial.println("RTC_TIMER_FLAG_TF_INFORMATION triggered");

            // 清除定时器标志TF
            PCF8563->IIC_Write_Device_Value(PCF8563->Arduino_IIC_RTC::Device_Value::RTC_TIMER_FLAG_TF,
                                            0);
            delay(1000);
        }
        delay(1000);
    }
}