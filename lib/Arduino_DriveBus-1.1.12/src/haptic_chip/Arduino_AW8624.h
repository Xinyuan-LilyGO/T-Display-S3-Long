/*
 * @Description(CN):
 *      基于Arduino_DriveBus库的AW8624芯片库
 *      编写了主要功能 部分功能未列出
 *      以下是操作AW8624的相关枚举（控制AW8624芯片时请调用以下枚举的参数）：
 *
 *      enum Device_State
 *      {
 *          HAPTIC_DEVICE_ON,  // 设备功能关
 *          HAPTIC_DEVICE_OFF, // 设备功能开
 *      };
 *      enum Device
 *      {
 *          HAPTIC_INITIALIZATION_SRAM, // 初始化SRAM
 *  
 *          HAPTIC_PLAY_MODE, // 播放模式
 *  
 *          HAPTIC_BRAKE1_MODE, // 制动1模式
 *          HAPTIC_BRAKE2_MODE, // 制动2模式
 *  
 *          HAPTIC_CHIP_MODE, // 芯片模式
 *  
 *          HAPTIC_PLAYBACK_GO_MODE, // RAM/RTP/CONT 模式回放触发位开关
 *  
 *          HAPTIC_MOTOR_TYPE, // 电机类型
 *  
 *          HAPTIC_WAVEFORM_FILLER_1_MODE, // 波形填装器1的模式
 *          HAPTIC_WAVEFORM_FILLER_2_MODE, // 波形填装器2的模式
 *          HAPTIC_WAVEFORM_FILLER_3_MODE, // 波形填装器3的模式
 *          HAPTIC_WAVEFORM_FILLER_4_MODE, // 波形填装器4的模式
 *          HAPTIC_WAVEFORM_FILLER_5_MODE, // 波形填装器5的模式
 *          HAPTIC_WAVEFORM_FILLER_6_MODE, // 波形填装器6的模式
 *          HAPTIC_WAVEFORM_FILLER_7_MODE, // 波形填装器7的模式
 *          HAPTIC_WAVEFORM_FILLER_8_MODE, // 波形填装器8的模式
 *  
 *          HAPTIC_PWM_SAMPLE_RATE_MODE, // PWM采样率模式
 *      };
 *      enum Device_Mode
 *      {
 *          HAPTIC_RAM_MODE,  // RAM模式
 *          HAPTIC_RTP_MODE,  // 实时播放模式
 *          HAPTIC_CONT_MODE, // 连续模式
 *  
 *          HAPTIC_ACTIVE_MODE,  // 激活模式
 *          HAPTIC_STANDBY_MODE, // 待机模式
 *  
 *          HAPTIC_ERM_MOTOR_MODE, // ERM电机模式
 *          HAPTIC_LRA_MOTOR_MODE, // LRA电机模式
 *  
 *          HAPTIC_WAVEFORM_FILLER_OUTPUT_MODE, // 波形填装器波形输出模式
 *          HAPTIC_WAVEFORM_FILLER_DELAY_MODE,  // 波形填装器延时模式
 *  
 *          HAPTIC_PWM_SAMPLE_RATE_48KB, // PWM采样率48KB
 *          HAPTIC_PWM_SAMPLE_RATE_24KB, // PWM采样率24KB
 *          HAPTIC_PWM_SAMPLE_RATE_12KB, // PWM采样率12KB
 *      };
 *      enum Device_Value
 *      {
 *          HAPTIC_SET_BRAKE0_LEVEL, // 设置制动0的等级
 *          HAPTIC_SET_BRAKE1_LEVEL, // 设置制动1的等级
 *          HAPTIC_SET_BRAKE2_LEVEL, // 设置制动2的等级
 *  
 *          HAPTIC_SET_BRAKE0_P_NUM, // 设置制动0的波形数量
 *          HAPTIC_SET_BRAKE1_P_NUM, // 设置制动1的波形数量
 *          HAPTIC_SET_BRAKE2_P_NUM, // 设置制动2的波形数量
 *  
 *          HAPTIC_RTP_DATA, // RTP数据寄存器
 *  
 *          HAPTIC_GAIN_VALUE, // 对波形数据的增益设置 它是对所有波形数据的全局设置
 *  
 *          HAPTIC_SET_SRAM_BASE_ADDRESS, // 设置波形SRAM的起始基地址
 *          HAPTIC_SET_SRAM_ADDRESS,      // 设置SRAM地址
 *          HAPTIC_SRAM_DATA,             // 波形数据SRAM寄存器
 *  
 *          HAPTIC_WAVEFORM_FILLER_1_OUTPUT_NUMBER_DELAY,   // 波形填装器1输出的波形号数或延时
 *          HAPTIC_WAVEFORM_FILLER_2_OUTPUT_NUMBER_DELAY,   // 波形填装器2输出的波形号数
 *          HAPTIC_WAVEFORM_FILLER_3_OUTPUT_NUMBER_DELAY,   // 波形填装器3输出的波形号数
 *          HAPTIC_WAVEFORM_FILLER_4_OUTPUT_NUMBER_DELAY,   // 波形填装器4输出的波形号数
 *          HAPTIC_WAVEFORM_FILLER_5_OUTPUT_NUMBER_DELAY,   // 波形填装器5输出的波形号数
 *          HAPTIC_WAVEFORM_FILLER_6_OUTPUT_NUMBER_DELAY,   // 波形填装器6输出的波形号数
 *          HAPTIC_WAVEFORM_FILLER_7_OUTPUT_NUMBER_DELAY,   // 波形填装器7输出的波形号数
 *          HAPTIC_WAVEFORM_FILLER_8_OUTPUT_NUMBER_DELAY,   // 波形填装器8输出的波形号数
 *          HAPTIC_WAVEFORM_FILLER_1_OUTPUT_PLAYBACK_TIMES, // 波形填装器1输出回放次数
 *          HAPTIC_WAVEFORM_FILLER_2_OUTPUT_PLAYBACK_TIMES, // 波形填装器2输出回放次数
 *          HAPTIC_WAVEFORM_FILLER_3_OUTPUT_PLAYBACK_TIMES, // 波形填装器3输出回放次数
 *          HAPTIC_WAVEFORM_FILLER_4_OUTPUT_PLAYBACK_TIMES, // 波形填装器4输出回放次数
 *          HAPTIC_WAVEFORM_FILLER_5_OUTPUT_PLAYBACK_TIMES, // 波形填装器5输出回放次数
 *          HAPTIC_WAVEFORM_FILLER_6_OUTPUT_PLAYBACK_TIMES, // 波形填装器6输出回放次数
 *          HAPTIC_WAVEFORM_FILLER_7_OUTPUT_PLAYBACK_TIMES, // 波形填装器7输出回放次数
 *          HAPTIC_WAVEFORM_FILLER_8_OUTPUT_PLAYBACK_TIMES, // 波形填装器8输出回放次数
 *      };
 *      enum Status_Information
 *      {
 *          HAPTIC_OVS_STATUS,             // 波形错误或者DPWM直流错误
 *          HAPTIC_UVLS_STATUS,            // 欠压状态
 *          HAPTIC_RTP_FIFO_MEMORY_STATUS, // RTP的FIFO 的剩余内存状态
 *          HAPTIC_OCDS_STATUS,            // 过流状态
 *          HAPTIC_OTS_STATUS,             // 过温状态
 *          HAPTIC_DONES_STATUS,           // 回放提示已完成
 *      };
 *      enum Value_Information
 *      {
 *          HAPTIC_CHIP_INPUT_VOLTAGE, // 芯片输入电压
 *      };
 *
 *      注意事项：
 *      1. 欠压保护功能有可能会误报
 *      2. IIC的速率必须大于400Khz以上，建议设置1MHz
 *
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2024-05-13 11:41:29
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-05-15 11:33:15
 * @License: GPL 3.0
 */
#pragma once

#include "../Arduino_IIC.h"

#define AW8624_DEVICE_ADDRESS 0x5A

#define AW8624_RD_WR_DEVICE_ID 0x00               // Device ID Register 0x24
#define AW8624_RD_WR_SYSTEM_STATUS 0x01           // System status Register
#define AW8624_RD_WR_SYSTEM_CONTROL 0x04          // System control Register
#define AW8624_RD_WR_PROCESS_CONTROL 0x05         // Process control Register
#define AW8624_RD_WR_RTP_DATA 0x06                // RTP mode data Register
#define AW8624_RD_WR_WAVEFORM_1 0x07              // First Waveform Register
#define AW8624_RD_WR_WAVEFORM_2 0x08              // Second Waveform Register
#define AW8624_RD_WR_WAVEFORM_3 0x09              // Third Waveform Register
#define AW8624_RD_WR_WAVEFORM_4 0x0A              // Fourth Waveform Register
#define AW8624_RD_WR_WAVEFORM_5 0x0B              // Fifth Waveform Register
#define AW8624_RD_WR_WAVEFORM_6 0x0C              // Sixth Waveform Register
#define AW8624_RD_WR_WAVEFORM_7 0x0D              // Seventh Waveform Register
#define AW8624_RD_WR_WAVEFORM_8 0x0E              // Eighth Waveform Register
#define AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_1 0x0F // Waveform loop control Register 1
#define AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_2 0x10 // Waveform loop control Register 2
#define AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_3 0x11 // Waveform loop control Register 3
#define AW8624_RD_WR_WAVEFORM_LOOP_CONTROL_4 0x12 // Waveform loop control Register 4
#define AW8624_RD_WR_BASE_SRAM_ADDRESS_HIGH 0x21  //   High five bits of Wave SRAM Base Register
#define AW8624_RD_WR_BASE_SRAM_ADDRESS_LOW 0x22   //   Low eight bits of Wave SRAM Base Register
#define AW8624_RD_WR_PWM_DEBUG 0x2E               //    PWM debug register
#define AW8624_RD_WR_WAVE_CTRL 0x31               //  Wave ctrl register
#define AW8624_RD_WR_BRAKE0_CTRL 0x32             //  Brake0 ctrl register
#define AW8624_RD_WR_BRAKE1_CTRL 0x33             //  Brake1 ctrl register
#define AW8624_RD_WR_BRAKE2_CTRL 0x34             //  Brake2 ctrl register
#define AW8624_RD_WR_BRAKE_NUMBER_CTRL 0x35       //  Brake number ctrl register
#define AW8624_RD_WR_DATA_GAIN 0x3B               //   DATA GAIN register
#define AW8624_RD_WR_SRAM_ADDRESS_HIGH 0x40       //   SRAM Address 0xhigh register
#define AW8624_RD_WR_SRAM_ADDRESS_LOW 0x41        //   SRAM Address  0xlow register
#define AW8624_RD_WR_DATA_GAIN 0x3B               //   DATA GAIN register
#define AW8624_RD_WR_SRAM_DATA 0x42               //    SRAM data register
#define AW8624_RD_WR_DETECTION_CONTROL 0x5F       //   Detection control register
#define AW8624_RD_WR_DETECTION_VBAT 0x62          //    Detected VBAT register

static const uint8_t AW8624_Initialization_BufferOperations[] = {
    BO_BEGIN_TRANSMISSION,
    BO_WRITE_C8_D8, AW8624_RD_WR_DEVICE_ID, 0xAA, // Software Reset
    BO_END_TRANSMISSION,
    BO_DELAY, 20};

class Arduino_AW8624 : public Arduino_IIC
{
public:
    Arduino_AW8624(std::shared_ptr<Arduino_IIC_DriveBus> bus, uint8_t device_address,
                   int8_t rst = DRIVEBUS_DEFAULT_VALUE, int8_t iqr = DRIVEBUS_DEFAULT_VALUE,
                   void (*Interrupt_Function)() = nullptr);

    bool begin(int32_t speed = DRIVEBUS_DEFAULT_VALUE) override;
    int32_t IIC_Device_ID(void) override;
    int32_t IIC_Device_Reset(void) override;
    bool IIC_Write_Device_State(uint32_t device, uint8_t state) override;
    bool IIC_Write_Device_Value(uint32_t device, int64_t value) override;
    bool IIC_Write_Device_Data(uint32_t device, const uint8_t *data, size_t length) override;

    String IIC_Read_Device_State(uint32_t information) override;
    double IIC_Read_Device_Value(uint32_t information) override;

protected:
    bool IIC_Initialization(void) override;
};