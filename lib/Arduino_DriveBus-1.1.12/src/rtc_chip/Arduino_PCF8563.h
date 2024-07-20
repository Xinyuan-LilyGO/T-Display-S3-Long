/*
 * @Description(CN):
 *      基于Arduino_DriveBus库的PCF8563芯片库
 *      编写了主要功能 部分功能未列出
 *      以下是操作PCF8563的相关枚举（控制PCF8563芯片时请调用以下枚举的参数）：
 *
 *      class Arduino_IIC_RTC
 *      {
 *      public:
 *          enum Device_State
 *          {
 *              RTC_DEVICE_ON,  // 设备功能关
 *              RTC_DEVICE_OFF, // 设备功能开
 *          };
 *          enum Device
 *          {
 *              RTC_CLOCK_RTC,    // RTC
 *      
 *              RTC_CLOCK_OUTPUT_VALUE, // 时钟输出的值
 *      
 *              RTC_INT_AIE, // 日程报警中断
 *              RTC_INT_TIE, // 定时器报警中断
 *      
 *              RTC_TIMER_FREQUENCY_VALUE, // 定时器频率的值
 *      
 *              RTC_TIMER,            // 定时器
 *              RTC_AE_MINUTE_ALARM,  // 分钟计划报警器
 *              RTC_AE_HOUR_ALARM,    // 小时计划报警器
 *              RTC_AE_DAY_ALARM,     // 天计划报警器
 *          };
 *          enum Device_Mode
 *          {
 *              RTC_CLOCK_OUTPUT_36768, // 时钟输出36.768kHz
 *              RTC_CLOCK_OUTPUT_1024,  // 时钟输出1.024kHz
 *              RTC_CLOCK_OUTPUT_32,    // 时钟输出32Hz
 *              RTC_CLOCK_OUTPUT_1,     // 时钟输出1Hz
 *      
 *              RTC_TIMER_FREQUENCY_4096, // 定时器频率4096Hz
 *              RTC_TIMER_FREQUENCY_64,   // 定时器频率64Hz
 *              RTC_TIMER_FREQUENCY_1,    // 定时器频率1Hz
 *              RTC_TIMER_FREQUENCY_1_60, // 定时器频率1/60Hz
 *              RTC_CLOCK_OUTPUT_OFF,   // 关闭时钟输出
 *          };
 *          enum Device_Value
 *          {
 *              RTC_SET_SECONDS_DATA, // 设置秒数据
 *              RTC_SET_MINUTES_DATA, // 设置分数据
 *              RTC_SET_HOURS_DATA,   // 设置时数据
 *              RTC_SET_DAYS_DATA,    // 设置天数据
 *              RTC_SET_MONTHS_DATA,  // 设置月数据
 *              RTC_SET_YEARS_DATA,   // 设置年数据
 *      
 *              RTC_TIMER_N_VALUE, // 定时器n的值
 *      
 *              RTC_TIMER_FLAG_AF, // 定时器AF标志
 *              RTC_TIMER_FLAG_TF, // 定时器TF标志
 *      
 *              RTC_SET_ALARM_MINUTES_DATA, // 设置报警分钟的数据
 *              RTC_SET_ALARM_HOUR_DATA,    // 设置报警小时的数据
 *              RTC_SET_ALARM_DAY_DATA,     // 设置报警天的数据
 *          };
 *          enum Status_Information
 *          {
 *              RTC_WEEKDAYS_DATA,
 *          };
 *          enum Value_Information
 *          {
 *              RTC_SECONDS_DATA,
 *              RTC_MINUTES_DATA,
 *              RTC_HOURS_DATA,
 *              RTC_DAYS_DATA,
 *              RTC_MONTHS_DATA,
 *              RTC_YEARS_DATA,
 *      
 *              RTC_ALARM_FLAG_AF_INFORMATION, // 报警器AF标志
 *              RTC_TIMER_FLAG_TF_INFORMATION, // 定时器TF标志
 *          };
 *      };
 * 
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2023-11-16 15:42:22
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-06-05 10:08:02
 * @License: GPL 3.0
 */
#pragma once

#include "../Arduino_IIC.h"

#define PCF8563_DEVICE_ADDRESS 0x51

#define PCF8563_RD_WR_CONTROL_STATUS_1 0x00
#define PCF8563_RD_WR_CONTROL_STATUS_2 0x01
#define PCF8563_RD_WR_VL_SECONDS 0x02
#define PCF8563_RD_WR_MINUTES 0x03
#define PCF8563_RD_WR_HOURS 0x04
#define PCF8563_RD_WR_DAYS 0x05
#define PCF8563_RD_WR_WEEKDAY 0x06
#define PCF8563_RD_WR_CENTURY_MONTHS 0x07
#define PCF8563_RD_WR_YEARS 0x08
#define PCF8563_RD_WR_MINUTE_ALARM 0x09
#define PCF8563_RD_WR_HOUR_ALARM 0x0A
#define PCF8563_RD_WR_DAY_ALARM 0x0B
#define PCF8563_RD_WR_WEEKDAY_ALARM 0x0C
#define PCF8563_RD_WR_CLKOUT_CONTROL 0x0D
#define PCF8563_RD_WR_TIMER_CONTROL 0x0E
#define PCF8563_RD_WR_TIMER 0x0F

#define PCF8563_RD_DEVICE_ID 0x00 // Device ID Register

static const uint8_t PCF8563_Initialization_BufferOperations[] = {
    // BO_BEGIN_TRANSMISSION,

    // BO_END_TRANSMISSION,
    // BO_DELAY, 100
};

class Arduino_PCF8563 : public Arduino_IIC
{
public:
    Arduino_PCF8563(std::shared_ptr<Arduino_IIC_DriveBus> bus, uint8_t device_address,
                    int8_t rst = DRIVEBUS_DEFAULT_VALUE, int8_t iqr = DRIVEBUS_DEFAULT_VALUE,
                    void (*Interrupt_Function)() = nullptr);

    bool begin(int32_t speed = DRIVEBUS_DEFAULT_VALUE) override;
    int32_t IIC_Device_ID(void) override;
    bool IIC_Write_Device_State(uint32_t device, uint8_t state) override;
    bool IIC_Write_Device_Value(uint32_t device, int64_t value) override;
    String IIC_Read_Device_State(uint32_t information) override;
    double IIC_Read_Device_Value(uint32_t information) override;

protected:
    bool IIC_Initialization(void) override;
};