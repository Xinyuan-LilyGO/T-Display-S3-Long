/*
 * @Description: None
 * @version: V1.0.0
 * @Author: None
 * @Date: 2024-03-12 09:24:12
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-06-06 17:04:47
 * @License: GPL 3.0
 */
/*
 * @Description(CN):
 *      属于Arduino_IIC的一个分支 用于存储IIC相关的芯片数据
 *
 * @Description(EN):
 *      A sub-branch of Arduino_IIC, used for storing IIC-related Power chip data
 *
 * @version: V1.1.5
 * @Author: Xk_w
 * @Date: 2023-11-16 16:58:05
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-03-11 14:38:57
 * @License: GPL 3.0
 */
#pragma once

#include <Arduino.h>

class Arduino_IIC_Power
{
public:
    enum Device_State
    {
        POWER_DEVICE_ON,  // 设备功能关
        POWER_DEVICE_OFF, // 设备功能开
    };

    enum Device
    {
        POWER_DEVICE_ADC_MEASURE,          // ADC测量功能
        POWER_DEVICE_HIZ_MODE,             // 高阻态模式（内部总线输出切换为只能电池输出）
        POWER_DEVICE_WATCHDOG_TIMER_RESET, // 看门狗定时器重置（喂狗）
        POWER_DEVICE_OTG_MODE,             // OTG模式
        POWER_DEVICE_CHARGING_MODE,        // 充电模式
        POWER_DEVICE_WATCHDOG_MODE,        // 看门狗模式
        POWER_DEVICE_NTC_MODE,             // 电池NTC热检测模式
        POWER_DEVICE_PCB_OTP_MODE,         // PCB过温保护模式

        POWER_BATFET_MODE, // 电池开关
    };

    enum Device_Value
    {
        POWER_DEVICE_MINIMUM_INPUT_VOLTAGE_LIMIT,                  // 设置最小输入电压限制值
        POWER_DEVICE_CHARGING_TARGET_VOLTAGE_LIMIT,                // 设置充电目标电压限制值
        POWER_DEVICE_MINIMUM_SYSTEM_VOLTAGE_LIMIT,                 // 设置最小系统电压限制值
        POWER_DEVICE_SYSTEM_VOLTAGE_LIMIT,                         // 设置系统电压限制值
        POWER_DEVICE_OTG_VOLTAGE_LIMIT,                            // 设置OTG电压输出值
        POWER_DEVICE_INPUT_CURRENT_LIMIT,                          // 设置输入电流限制值
        POWER_DEVICE_FAST_CHARGING_CURRENT_LIMIT,                  // 设置快速充电电流限制值
        POWER_DEVICE_PRECHARGE_CHARGING_CURRENT_LIMIT,             // 设置预充电电流限制值
        POWER_DEVICE_TERMINATION_CHARGING_CURRENT_LIMIT,           // 设置终止充电电流限制值
        POWER_DEVICE_TERMINATION_PRECHARGE_CHARGING_CURRENT_LIMIT, // 设置终止充电和预充电电流限制值
        POWER_DEVICE_OTG_CHARGING_LIMIT,                           // 设置OTG电流输出值
        POWER_DEVICE_BAT_TO_SYS_DISCHARGE_CURRENT_LIMIT,           // BAT到SYS的放电电流限制值

        POWER_DEVICE_WATCHDOG_TIMER,               // 设置看门狗定时器值
        POWER_DEVICE_THERMAL_REGULATION_THRESHOLD, // 设置热调节阈值值
        POWER_DEVICE_BATTERY_UVLO_THRESHOLD        // 设置电池低电压锁定阈值
    };

    enum Status_Information
    {
        POWER_BUS_STATUS,                // 总线状态
        POWER_BUS_CONNECTION_STATUS,     // 总线连接状态
        POWER_CHARGING_STATUS,           // 充电状态
        POWER_INPUT_SOURCE_STATUS,       // 输入源的状态
        POWER_INPUT_USB_STATUS,          // 输入USB状态
        POWER_SYSTEM_VOLTAGE_STATUS,     // 系统电压状态
        POWER_THERMAL_REGULATION_STATUS, // 热调节状态

        POWER_WATCHDOG_FAULT_STATUS,            // 看门狗故障状态（故障后自动重置限制类的功能值）
        POWER_INPUT_FAULT_STATUS,               // 输入故障状态
        POWER_THERMAL_SHUTDOWN_FAULT_STATUS,    // 热关闭故障状态
        POWER_OTG_FAULT_STATUS,                 // 升压OTG模式故障状态
        POWER_CHARGING_FAULT_STATUS,            // 充电故障状态
        POWER_BATTERY_FAULT_STATUS,             // 电池故障状态
        POWER_SAFETY_TIMER_STATUS_FAULT_STATUS, // 安全定时器故障状态
        POWER_NTC_FAULT_STATUS,                 // NTC故障状态
    };

    enum Value_Information
    {
        POWER_INPUT_VOLTAGE,                // 输入电压值
        POWER_BATTERY_VOLTAGE,              // 电池电压值
        POWER_SYSTEM_VOLTAGE,               // 系统电压值
        POWER_NTC_VOLTAGE_PERCENTAGE,       // NTC电压百分比值
        POWER_CHARGING_CURRENT,             // 充电电流值
        POWER_THERMAL_REGULATION_THRESHOLD, // 热调节阈值值

        POWER_MINIMUM_INPUT_VOLTAGE_LIMIT,                  // 最小输入电压限制值
        POWER_CHARGING_TARGET_VOLTAGE_LIMIT,                // 充电目标电压限制值
        POWER_MINIMUM_SYSTEM_VOLTAGE_LIMIT,                 // 最小系统电压限制值
        POWER_SYSTEM_VOLTAGE_LIMIT,                         // 系统电压限制值
        POWER_OTG_VOLTAGE_LIMIT,                            // OTG模式电压限制
        POWER_INPUT_CURRENT_LIMIT,                          // 输入电流限制值
        POWER_FAST_CHARGING_CURRENT_LIMIT,                  // 快速充电电流限制
        POWER_PRECHARGE_CHARGING_CURRENT_LIMIT,             // 预充电电流限制
        POWER_TERMINATION_CHARGING_CURRENT_LIMIT,           // 终止充电电流限制
        POWER_TERMINATION_PRECHARGE_CHARGING_CURRENT_LIMIT, // 终止充电和预充电电流限制
        POWER_OTG_CURRENT_LIMIT,                            // OTG模式电流限制
        POWER_BAT_TO_SYS_DISCHARGE_CURRENT_LIMIT,           // BAT到SYS的放电电流限制
    };

protected:
};

class Arduino_IIC_Touch
{
public:
    enum Device_State
    {
        TOUCH_DEVICE_ON,  // 设备功能关
        TOUCH_DEVICE_OFF, // 设备功能开
    };
    enum Device_Mode
    {
        // 触摸中断模式
        TOUCH_DEVICE_INTERRUPT_TEST,     //  中断引脚测试，使能后自动周期性发出低脉冲
        TOUCH_DEVICE_INTERRUPT_PERIODIC, // 检测到触摸时，发出低脉冲
        TOUCH_DEVICE_INTERRUPT_CHANGE,   // 检测到触摸状态变化时，发出低脉冲
        TOUCH_DEVICE_INTERRUPT_MOTION,   //  检测到手势时，发出低脉冲
        TOUCH_DEVICE_INTERRUPT_ONCEWLP,  //  长按手势只发出一个低脉冲信号

        // 触摸功耗模式
        TOUCH_POWER_ACTIVE,    // 激活模式
        TOUCH_POWER_MONITOR,   // 监听触发模式
        TOUCH_POWER_STANDBY,   // 待机模式
        TOUCH_POWER_HIBERNATE, // 休眠模式
    };
    enum Device
    {
        TOUCH_DEVICE_INTERRUPT_MODE, // 触摸中断模式
        TOUCH_DEVICE_SLEEP_MODE,     // 休眠模式

        TOUCH_POWER_MODE,             // 触摸芯片工作功耗模式
        TOUCH_PROXIMITY_SENSING_MODE, // 近距离感应模式
        TOUCH_GESTUREID_MODE,         // 特殊手势模式

        TOUCH_AUTOMATICALLY_MONITOR_MODE, // 自动进入Monitor模式
    };
    enum Device_Value
    {
        TOUCH_AUTOMATICALLY_MONITOR_TIME, // 自动进入Monitor模式的时间
    };
    enum Status_Information
    {
        TOUCH_GESTURE_ID, // 触摸手势ID
    };
    enum Value_Information
    {
        TOUCH_FINGER_NUMBER, // 触摸手指个数
        TOUCH_COORDINATE_X,  // 触摸坐标X 默认是第一个手指
        TOUCH_COORDINATE_Y,  // 触摸坐标Y 默认是第一个手指

        TOUCH1_COORDINATE_X, // 触摸1坐标X
        TOUCH1_COORDINATE_Y, // 触摸1坐标Y
        TOUCH2_COORDINATE_X, // 触摸2坐标X
        TOUCH2_COORDINATE_Y, // 触摸2坐标Y
        TOUCH3_COORDINATE_X, // 触摸3坐标X
        TOUCH3_COORDINATE_Y, // 触摸3坐标Y
        TOUCH4_COORDINATE_X, // 触摸4坐标X
        TOUCH4_COORDINATE_Y, // 触摸4坐标Y
        TOUCH5_COORDINATE_X, // 触摸5坐标X
        TOUCH5_COORDINATE_Y, // 触摸5坐标Y

        TOUCH1_PRESSURE_VALUE, // 触摸1手指压力值
        TOUCH2_PRESSURE_VALUE, // 触摸2手指压力值
        TOUCH3_PRESSURE_VALUE, // 触摸3手指压力值
        TOUCH4_PRESSURE_VALUE, // 触摸4手指压力值
        TOUCH5_PRESSURE_VALUE, // 触摸5手指压力值
    };
};

class Arduino_IIC_IMU
{
public:
    enum Device_State
    {
        IMU_DEVICE_ON,  // 设备功能关
        IMU_DEVICE_OFF, // 设备功能开
    };
    enum Device
    {
        IMU_ACCELERATION_POWER_MODE, // 加速度传感器电源功耗模式
        IMU_GYROSCOPE_POWER_MODE,    // 陀螺仪传感器电源功耗模式

        IMU_GYROSCOPE_SLEEP_MODE, // 陀螺仪传感器睡眠模式

        IMU_FIFO_POWER_MODE, // 传感器FIFO的电源功耗模式
        IMU_FIFO_WORK_MODE,  // 传感器FIFO的工作模式

        IMU_FIFO_DATA_TYPE, // 传感器FIFO选择需要获取的数据类型

        IMU_PEDOMETER_MODE,  // 传感器计步器模式
        IMU_PEDOMETER_RESET, // 复位传感器计步器的值
    };
    enum Device_Mode
    {
        // 传感器设备功耗模式
        IMU_DEVICE_OFF_POWER,        // 下电模式
        IMU_DEVICE_ULTRA_LOW_POWER,  // 超低功耗模式
        IMU_DEVICE_LOW_POWER,        // 低功耗模式
        IMU_DEVICE_NORMAL_POWER,     // 正常模式
        IMU_DEVICE_HIGH_POWER,       // 高性能模式
        IMU_DEVICE_ULTRA_HIGH_POWER, // 超高性能模式

        // 传感器FIFO模式
        IMU_FIFO_BYPASS,             // Bypass模式 FIFO禁用
        IMU_FIFO_NORMAL,             // 正常FIFO模式 当FIFO满时停止采集数据
        IMU_FIFO_CONTINUE,           // Continue模式 如果FIFO已满则新的采样将会覆盖原有值
        IMU_FIFO_CONTINUE_TO_NORMAL, // 直至触发条件满足前均为Continue模式 条件满足后进入正常FIFO模式
        IMU_FIFO_BYPASS_TO_CONTINUE, // 直至触发条件满足前均为Bypass模式 条件满足后进入Continue模式

        IMU_FIFO_DATA_ACCELERATION_GYROSCOPE, // 加速度传感器以及陀螺仪传感器的数据
        IMU_FIFO_DATA_PEDOMETER,              // 计步器的数据
        IMU_FIFO_DATA_SENSOR_HUB,             // 传感器集合的数据

    };
    enum Device_Value
    {
        IMU_ACCELERATION_SENSITIVITY, // 设置加速度传感器量程范围
        IMU_GYROSCOPE_SENSITIVITY,    // 设置陀螺仪传感器量程范围

        IMU_ACCELERATION_CORRECTION, // 加速度传感器校正
        IMU_GYROSCOPE_CORRECTION,    // 陀螺仪传感器校正

        IMU_FIFO_ACCELERATION_FACTOR, // 加速度传感器FIFO的抽取因子
        IMU_FIFO_GYROSCOPE_FACTOR,    // 加速度传感器FIFO的抽取因子
    };
    enum Status_Information
    {
    };
    enum Value_Information
    {
        IMU_TEMPERATURE_VALUE, // 传感器温度值

        IMU_ACCELERATION_X, // 加速度传感器X方向值 原始值
        IMU_ACCELERATION_Y, // 加速度传感器Y方向值 原始值
        IMU_ACCELERATION_Z, // 加速度传感器Z方向值 原始值

        IMU_GYROSCOPE_X, // 陀螺仪传感器X方向值 原始值
        IMU_GYROSCOPE_Y, // 陀螺仪传感器Y方向值 原始值
        IMU_GYROSCOPE_Z, // 陀螺仪传感器Z方向值 原始值

        IMU_ACCELERATION_X_SIGNED, // 加速度传感器X方向值 带符号
        IMU_ACCELERATION_Y_SIGNED, // 加速度传感器Y方向值 带符号
        IMU_ACCELERATION_Z_SIGNED, // 加速度传感器Z方向值 带符号

        IMU_GYROSCOPE_X_SIGNED, // 陀螺仪传感器X方向值 带符号
        IMU_GYROSCOPE_Y_SIGNED, // 陀螺仪传感器Y方向值 带符号
        IMU_GYROSCOPE_Z_SIGNED, // 陀螺仪传感器Z方向值 带符号

        IMU_FIFO_DATA_PATTERN, // 用于判断下次读取时读取哪个传感器哪个轴上的数据
        IMU_FIFO_DATA,         // FIFO 上的数据 原始值

        IMU_PEDOMETER_VALUE, // 传感器计步器的值
    };
};

class Arduino_IIC_RTC
{
public:
    enum Device_State
    {
        RTC_DEVICE_ON,  // 设备功能关
        RTC_DEVICE_OFF, // 设备功能开
    };
    enum Device
    {
        RTC_CLOCK_RTC,    // RTC

        RTC_CLOCK_OUTPUT_VALUE, // 时钟输出的值

        RTC_INT_AIE, // 日程报警中断
        RTC_INT_TIE, // 定时器报警中断

        RTC_TIMER_FREQUENCY_VALUE, // 定时器频率的值

        RTC_TIMER,           // 定时器
        RTC_AE_SECOND_ALARM, // 秒计划报警器
        RTC_AE_MINUTE_ALARM, // 分钟计划报警器
        RTC_AE_HOUR_ALARM,   // 小时计划报警器
        RTC_AE_DAY_ALARM,    // 天计划报警器

        RTC_CLOCK_TIME_FORMAT, // RTC 时钟格式
    };
    enum Device_Mode
    {
        RTC_CLOCK_OUTPUT_36768, // 时钟输出36768Hz
        RTC_CLOCK_OUTPUT_16384, // 时钟输出16384Hz
        RTC_CLOCK_OUTPUT_8192,  // 时钟输出8192Hz
        RTC_CLOCK_OUTPUT_4096,  // 时钟输出4096Hz
        RTC_CLOCK_OUTPUT_2048,  // 时钟输出2048Hz
        RTC_CLOCK_OUTPUT_1024,  // 时钟输出1024Hz
        RTC_CLOCK_OUTPUT_32,    // 时钟输出32Hz
        RTC_CLOCK_OUTPUT_1,     // 时钟输出1Hz
        RTC_CLOCK_OUTPUT_OFF,   // 关闭时钟输出

        RTC_TIMER_FREQUENCY_4096, // 定时器频率4096Hz
        RTC_TIMER_FREQUENCY_64,   // 定时器频率64Hz
        RTC_TIMER_FREQUENCY_1,    // 定时器频率1Hz
        RTC_TIMER_FREQUENCY_1_60, // 定时器频率1/60Hz

        RTC_CLOCK_TIME_FORMAT_12, // 12小时制
        RTC_CLOCK_TIME_FORMAT_24, // 24小时制
    };
    enum Device_Value
    {
        RTC_SET_SECOND_DATA, // 设置秒数据
        RTC_SET_MINUTE_DATA, // 设置分数据
        RTC_SET_HOUR_DATA,   // 设置时数据
        RTC_SET_DAY_DATA,    // 设置天数据
        RTC_SET_MONTH_DATA,  // 设置月数据
        RTC_SET_YEAR_DATA,   // 设置年数据

        RTC_TIMER_N_VALUE, // 定时器n的值

        RTC_TIMER_FLAG_AF, // 定时器AF标志
        RTC_TIMER_FLAG_TF, // 定时器TF标志

        RTC_SET_ALARM_SECOND_DATA, // 设置报警秒的数据
        RTC_SET_ALARM_MINUTE_DATA, // 设置报警分钟的数据
        RTC_SET_ALARM_HOUR_DATA,   // 设置报警小时的数据
        RTC_SET_ALARM_DAY_DATA,    // 设置报警天的数据
    };
    enum Status_Information
    {
        RTC_WEEKDAYS_DATA,
    };
    enum Value_Information
    {
        RTC_SECONDS_DATA,
        RTC_MINUTES_DATA,
        RTC_HOURS_DATA,
        RTC_DAYS_DATA,
        RTC_MONTHS_DATA,
        RTC_YEARS_DATA,

        RTC_ALARM_FLAG_AF_INFORMATION, // 报警器AF标志
        RTC_TIMER_FLAG_TF_INFORMATION, // 定时器TF标志
    };
};

class Arduino_IIC_HAPTIC
{
public:
    enum Device_State
    {
        HAPTIC_DEVICE_ON,  // 设备功能关
        HAPTIC_DEVICE_OFF, // 设备功能开
    };
    enum Device
    {
        HAPTIC_INITIALIZATION_SRAM, // 初始化SRAM

        HAPTIC_PLAY_MODE, // 播放模式

        HAPTIC_BRAKE1_MODE, // 制动1模式
        HAPTIC_BRAKE2_MODE, // 制动2模式

        HAPTIC_CHIP_MODE, // 芯片模式

        HAPTIC_PLAYBACK_GO_MODE, // RAM/RTP/CONT 模式回放触发位开关

        HAPTIC_MOTOR_TYPE, // 电机类型

        HAPTIC_WAVEFORM_FILLER_1_MODE, // 波形填装器1的模式
        HAPTIC_WAVEFORM_FILLER_2_MODE, // 波形填装器2的模式
        HAPTIC_WAVEFORM_FILLER_3_MODE, // 波形填装器3的模式
        HAPTIC_WAVEFORM_FILLER_4_MODE, // 波形填装器4的模式
        HAPTIC_WAVEFORM_FILLER_5_MODE, // 波形填装器5的模式
        HAPTIC_WAVEFORM_FILLER_6_MODE, // 波形填装器6的模式
        HAPTIC_WAVEFORM_FILLER_7_MODE, // 波形填装器7的模式
        HAPTIC_WAVEFORM_FILLER_8_MODE, // 波形填装器8的模式

        HAPTIC_PWM_SAMPLE_RATE_MODE, // PWM采样率模式
    };
    enum Device_Mode
    {
        HAPTIC_RAM_MODE,  // RAM模式
        HAPTIC_RTP_MODE,  // 实时播放模式
        HAPTIC_CONT_MODE, // 连续模式

        HAPTIC_ACTIVE_MODE,  // 激活模式
        HAPTIC_STANDBY_MODE, // 待机模式

        HAPTIC_ERM_MOTOR_MODE, // ERM电机模式
        HAPTIC_LRA_MOTOR_MODE, // LRA电机模式

        HAPTIC_WAVEFORM_FILLER_OUTPUT_MODE, // 波形填装器波形输出模式
        HAPTIC_WAVEFORM_FILLER_DELAY_MODE,  // 波形填装器延时模式

        HAPTIC_PWM_SAMPLE_RATE_48KB, // PWM采样率48KB
        HAPTIC_PWM_SAMPLE_RATE_24KB, // PWM采样率24KB
        HAPTIC_PWM_SAMPLE_RATE_12KB, // PWM采样率12KB
    };
    enum Device_Value
    {
        HAPTIC_SET_BRAKE0_LEVEL, // 设置制动0的等级
        HAPTIC_SET_BRAKE1_LEVEL, // 设置制动1的等级
        HAPTIC_SET_BRAKE2_LEVEL, // 设置制动2的等级

        HAPTIC_SET_BRAKE0_P_NUM, // 设置制动0的波形数量
        HAPTIC_SET_BRAKE1_P_NUM, // 设置制动1的波形数量
        HAPTIC_SET_BRAKE2_P_NUM, // 设置制动2的波形数量

        HAPTIC_RTP_DATA, // RTP数据寄存器

        HAPTIC_GAIN_VALUE, // 对波形数据的增益设置 它是对所有波形数据的全局设置

        HAPTIC_SET_SRAM_BASE_ADDRESS, // 设置波形SRAM的起始基地址
        HAPTIC_SET_SRAM_ADDRESS,      // 设置SRAM地址
        HAPTIC_SRAM_DATA,             // 波形数据SRAM寄存器

        HAPTIC_WAVEFORM_FILLER_1_OUTPUT_NUMBER_DELAY,   // 波形填装器1输出的波形号数或延时
        HAPTIC_WAVEFORM_FILLER_2_OUTPUT_NUMBER_DELAY,   // 波形填装器2输出的波形号数
        HAPTIC_WAVEFORM_FILLER_3_OUTPUT_NUMBER_DELAY,   // 波形填装器3输出的波形号数
        HAPTIC_WAVEFORM_FILLER_4_OUTPUT_NUMBER_DELAY,   // 波形填装器4输出的波形号数
        HAPTIC_WAVEFORM_FILLER_5_OUTPUT_NUMBER_DELAY,   // 波形填装器5输出的波形号数
        HAPTIC_WAVEFORM_FILLER_6_OUTPUT_NUMBER_DELAY,   // 波形填装器6输出的波形号数
        HAPTIC_WAVEFORM_FILLER_7_OUTPUT_NUMBER_DELAY,   // 波形填装器7输出的波形号数
        HAPTIC_WAVEFORM_FILLER_8_OUTPUT_NUMBER_DELAY,   // 波形填装器8输出的波形号数
        HAPTIC_WAVEFORM_FILLER_1_OUTPUT_PLAYBACK_TIMES, // 波形填装器1输出回放次数
        HAPTIC_WAVEFORM_FILLER_2_OUTPUT_PLAYBACK_TIMES, // 波形填装器2输出回放次数
        HAPTIC_WAVEFORM_FILLER_3_OUTPUT_PLAYBACK_TIMES, // 波形填装器3输出回放次数
        HAPTIC_WAVEFORM_FILLER_4_OUTPUT_PLAYBACK_TIMES, // 波形填装器4输出回放次数
        HAPTIC_WAVEFORM_FILLER_5_OUTPUT_PLAYBACK_TIMES, // 波形填装器5输出回放次数
        HAPTIC_WAVEFORM_FILLER_6_OUTPUT_PLAYBACK_TIMES, // 波形填装器6输出回放次数
        HAPTIC_WAVEFORM_FILLER_7_OUTPUT_PLAYBACK_TIMES, // 波形填装器7输出回放次数
        HAPTIC_WAVEFORM_FILLER_8_OUTPUT_PLAYBACK_TIMES, // 波形填装器8输出回放次数
    };
    enum Status_Information
    {
        HAPTIC_OVS_STATUS,             // 波形错误或者DPWM直流错误
        HAPTIC_UVLS_STATUS,            // 欠压状态
        HAPTIC_RTP_FIFO_MEMORY_STATUS, // RTP的FIFO 的剩余内存状态
        HAPTIC_OCDS_STATUS,            // 过流状态
        HAPTIC_OTS_STATUS,             // 过温状态
        HAPTIC_DONES_STATUS,           // 回放提示已完成
    };
    enum Value_Information
    {
        HAPTIC_CHIP_INPUT_VOLTAGE, // 芯片输入电压
    };
};
