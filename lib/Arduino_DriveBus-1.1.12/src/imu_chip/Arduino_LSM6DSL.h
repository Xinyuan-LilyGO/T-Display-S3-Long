/*
 * @Description(CN):
 *      基于Arduino_DriveBus库的LSM6DSL芯片库
 *      编写了主要功能 部分功能未列出
 *      以下是操作LSM6DSL的相关枚举（控制LSM6DSL芯片时请调用以下枚举的参数）：
 *
 *      enum Device_State
 *      {
 *          IMU_DEVICE_ON,  // 设备功能关
 *          IMU_DEVICE_OFF, // 设备功能开
 *      };
 *      enum Device
 *      {
 *          IMU_ACCELERATION_POWER_MODE, // 加速度传感器电源功耗模式
 *          IMU_GYROSCOPE_POWER_MODE,    // 陀螺仪传感器电源功耗模式
 * 
 *          IMU_GYROSCOPE_SLEEP_MODE // 陀螺仪传感器睡眠模式
 *
 *          IMU_FIFO_POWER_MODE, // 传感器FIFO的电源功耗模式
 *          IMU_FIFO_WORK_MODE,  // 传感器FIFO的工作模式
 *
 *          IMU_FIFO_DATA_TYPE, // 传感器FIFO选择需要获取的数据类型
 *
 *          IMU_PEDOMETER_MODE, // 传感器计步器模式
 *          IMU_PEDOMETER_RESET, // 复位传感器计步器的值
 *      };
 *      enum Device_Mode
 *      {
 *          // 传感器设备功耗模式
 *          IMU_DEVICE_OFF,              // 下电模式
 *          IMU_DEVICE_ULTRA_LOW_POWER,  // 超低功耗模式
 *          IMU_DEVICE_LOW_POWER,        // 低功耗模式
 *          IMU_DEVICE_NORMAL_POWER,     // 正常模式
 *          IMU_DEVICE_HIGH_POWER,       // 高性能模式
 *          IMU_DEVICE_ULTRA_HIGH_POWER, // 超高性能模式
 *
 *          // 传感器FIFO模式
 *          IMU_FIFO_BYPASS,             // Bypass模式 FIFO禁用
 *          IMU_FIFO_NORMAL,             // 正常FIFO模式 当FIFO满时停止采集数据
 *          IMU_FIFO_CONTINUE,           // Continue模式 如果FIFO已满则新的采样将会覆盖原有值
 *          IMU_FIFO_CONTINUE_TO_NORMAL, // 直至触发条件满足前均为Continue模式 条件满足后进入正常FIFO模式
 *          IMU_FIFO_BYPASS_TO_CONTINUE, // 直至触发条件满足前均为Bypass模式 条件满足后进入Continue模式
 *
 *          IMU_FIFO_DATA_ACCELERATION_GYROSCOPE, // 加速度传感器以及陀螺仪传感器的数据
 *          IMU_FIFO_DATA_PEDOMETER,              // 计步器的数据
 *          IMU_FIFO_DATA_SENSOR_HUB,             // 传感器集合的数据
 *
 *      };
 *      enum Device_Value
 *      {
 *          IMU_ACCELERATION_SENSITIVITY, // 设置加速度传感器量程范围
 *          IMU_GYROSCOPE_SENSITIVITY,    // 设置陀螺仪传感器量程范围
 *
 *          IMU_ACCELERATION_CORRECTION, // 加速度传感器校正
 *          IMU_GYROSCOPE_CORRECTION,    // 陀螺仪传感器校正
 *
 *          IMU_FIFO_ACCELERATION_FACTOR, // 加速度传感器FIFO的抽取因子
 *          IMU_FIFO_GYROSCOPE_FACTOR,    // 加速度传感器FIFO的抽取因子
 *      };
 *      enum Status_Information
 *      {
 *      };
 *      enum Value_Information
 *      {
 *          IMU_TEMPERATURE_VALUE, // 传感器温度值
 *
 *          IMU_ACCELERATION_X, // 加速度传感器X方向值 原始值
 *          IMU_ACCELERATION_Y, // 加速度传感器Y方向值 原始值
 *          IMU_ACCELERATION_Z, // 加速度传感器Z方向值 原始值
 *
 *          IMU_GYROSCOPE_X, // 陀螺仪传感器X方向值 原始值
 *          IMU_GYROSCOPE_Y, // 陀螺仪传感器Y方向值 原始值
 *          IMU_GYROSCOPE_Z, // 陀螺仪传感器Z方向值 原始值
 *
 *          IMU_ACCELERATION_X_SIGNED, // 加速度传感器X方向值 带符号
 *          IMU_ACCELERATION_Y_SIGNED, // 加速度传感器Y方向值 带符号
 *          IMU_ACCELERATION_Z_SIGNED, // 加速度传感器Z方向值 带符号
 *
 *          IMU_GYROSCOPE_X_SIGNED, // 陀螺仪传感器X方向值 带符号
 *          IMU_GYROSCOPE_Y_SIGNED, // 陀螺仪传感器Y方向值 带符号
 *          IMU_GYROSCOPE_Z_SIGNED, // 陀螺仪传感器Z方向值 带符号
 *
 *          IMU_FIFO_DATA_PATTERN, // 用于判断下次读取时读取哪个传感器哪个轴上的数据
 *          IMU_FIFO_DATA,         // FIFO 上的数据 原始值
 *
 *          IMU_PEDOMETER_VALUE, // 传感器计步器的值
 *      };
 *
 *      注意事项：
 *      1. LSM6DSL在过高的性能模式下采集数据时数据波动可能会较大
 *      2. LSM6DSL直接读取数据时，在过低性能模式下直接读取速率时候建议做个延时，否则读取数据将会失败
 *      3. LSM6DSL使用时候需要进行校正，校正时请将设备正放并静止，校正顺序从陀螺仪传感器到加速度传感器
 *
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2023-11-16 15:42:22
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-02-28 14:30:06
 * @License: GPL 3.0
 */
#pragma once

#include "../Arduino_IIC.h"

#define LSM6DSL_DEVICE_ADDRESS 0x6A // SDO/SA0 Pin Low
// #define LSM6DSL_DEVICE_ADDRESS 0x6B // SDO/SA0 Pin High

#define LSM6DSL_RD_WR_FIFO_CTRL2 0x07    // FIFO control register
#define LSM6DSL_RD_WR_FIFO_CTRL3 0x08    // FIFO control register
#define LSM6DSL_RD_WR_FIFO_CTRL5 0x0A    // FIFO control register
#define LSM6DSL_RD_WR_CTRL1_XL 0x10      // Linear acceleration sensor control register 1
#define LSM6DSL_RD_WR_CTRL2_G 0x11       // Angular rate sensor control register 2
#define LSM6DSL_RD_WR_CTRL3_C 0x12       // Control register 3
#define LSM6DSL_RD_WR_CTRL4_C 0x13       // Control register 4
#define LSM6DSL_RD_WR_CTRL6_C 0x15       // Angular rate sensor control register 6
#define LSM6DSL_RD_WR_CTRL7_G 0x16       // Angular rate sensor control register 7
#define LSM6DSL_RD_WR_CTRL10_C 0x19      // Control register 10
#define LSM6DSL_RD_WR_MASTER_CONFIG 0x1A // Master configuration register
#define LSM6DSL_RD_STATUS_REG 0x1E       // The STATUS_REG register is read by the SPI/I2C interface
#define LSM6DSL_RD_OUT_TEMP_L 0x20       // Temperature output data registers
#define LSM6DSL_RD_OUT_TEMP_H 0x21       // Temperature output data registers
#define LSM6DSL_RD_OUTX_L_G 0x22         // Angular rate sensor pitch axis (X) angular rate output register
#define LSM6DSL_RD_OUTX_H_G 0x23         // Angular rate sensor pitch axis (X) angular rate output register
#define LSM6DSL_RD_OUTY_L_G 0x24         // Angular rate sensor pitch axis (Y) angular rate output register
#define LSM6DSL_RD_OUTY_H_G 0x25         // Angular rate sensor pitch axis (Y) angular rate output register
#define LSM6DSL_RD_OUTZ_L_G 0x26         // Angular rate sensor pitch axis (Z) angular rate output register
#define LSM6DSL_RD_OUTZ_H_G 0x27         // Angular rate sensor pitch axis (Z) angular rate output register
#define LSM6DSL_RD_OUTX_L_XL 0x28        // Linear acceleration sensor X-axis output register
#define LSM6DSL_RD_OUTX_H_XL 0x29        // Linear acceleration sensor X-axis output register
#define LSM6DSL_RD_OUTY_L_XL 0x2A        // Linear acceleration sensor Y-axis output register
#define LSM6DSL_RD_OUTY_H_XL 0x2B        // Linear acceleration sensor Y-axis output register
#define LSM6DSL_RD_OUTZ_L_XL 0x2C        // Linear acceleration sensor Z-axis output register
#define LSM6DSL_RD_OUTZ_H_XL 0x2D        // Linear acceleration sensor Z-axis output register
#define LSM6DSL_RD_FIFO_STATUS3 0x3C     // FIFO status control register
#define LSM6DSL_RD_FIFO_STATUS4 0x3D     // FIFO status control register
#define LSM6DSL_RD_FIFO_DATA_OUT_L 0x3E  // FIFO data output register
#define LSM6DSL_RD_FIFO_DATA_OUT_H 0x3F  // FIFO data output register
#define LSM6DSL_RD_STEP_COUNTER_L 0x4B   // Step counter output register
#define LSM6DSL_RD_STEP_COUNTER_H 0x4C   // Step counter output register

#define LSM6DSL_RD_DEVICE_ID 0x0F // Device ID Register

static const uint8_t LSM6DSL_Initialization_BufferOperations[] = {
    BO_BEGIN_TRANSMISSION,
    BO_WRITE_C8_D8, LSM6DSL_RD_WR_CTRL3_C, 0B01000100, // 开启BUD
    BO_END_TRANSMISSION,

    BO_DELAY, 100};

class Arduino_LSM6DSL : public Arduino_IIC
{
public:
    Arduino_LSM6DSL(std::shared_ptr<Arduino_IIC_DriveBus> bus, uint8_t device_address,
                    int8_t rst = DRIVEBUS_DEFAULT_VALUE, int8_t iqr = DRIVEBUS_DEFAULT_VALUE);

    bool begin(int32_t speed = DRIVEBUS_DEFAULT_VALUE) override;
    int32_t IIC_Device_ID(void) override;
    bool IIC_Write_Device_State(uint32_t device, uint8_t state) override;
    bool IIC_Write_Device_Value(uint32_t device, int64_t value) override;
    double IIC_Read_Device_Value(uint32_t information) override;

protected:
    bool IIC_Initialization(void) override;
    double _acceleration_x_correction = 0, _acceleration_y_correction = 0, _acceleration_z_correction = 0;
    double _gyroscope_x_correction = 0, _gyroscope_y_correction = 0, _gyroscope_z_correction = 0;
};