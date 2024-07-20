/*
 * @Description: 触感芯片AW8624波形数据RTP播放示例适用于EMR电机
 * @version: V1.0.0
 * @Author: None
 * @Date: 2024-05-07 10:53:17
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-06-06 18:28:22
 * @License: GPL 3.0
 */

#include "Arduino_DriveBus_Library.h"
#include "pin_config.h"

static size_t CycleTime = 0;

std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
    std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire);

void Arduino_IIC_Haptic_Interrupt(void);

std::unique_ptr<Arduino_IIC> AW8624(new Arduino_AW8624(IIC_Bus, AW8624_DEVICE_ADDRESS,
                                                       DRIVEBUS_DEFAULT_VALUE, AW8624_TRIG_INT, Arduino_IIC_Haptic_Interrupt));

void Arduino_IIC_Haptic_Interrupt(void)
{
    AW8624->IIC_Interrupt_Flag = true;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Ciallo");

    pinMode(RT9080_EN, OUTPUT);
    digitalWrite(RT9080_EN, HIGH);

    while (AW8624->begin(500000) == false) // 速度必须400KHz以上
    {
        Serial.println("AW8624 initialization fail");
        delay(2000);
    }
    Serial.println("AW8624 initialization successfully");

    Serial.printf("ID: %#X \n\n", (int32_t)AW8624->IIC_Device_ID());

    // 设置电机类型为ERM电机
    AW8624->IIC_Write_Device_State(AW8624->Arduino_IIC_HAPTIC::Device::HAPTIC_MOTOR_TYPE,
                                   AW8624->Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_ERM_MOTOR_MODE);
    // 设置播放模式为实时播放模式
    AW8624->IIC_Write_Device_State(AW8624->Arduino_IIC_HAPTIC::Device::HAPTIC_PLAY_MODE,
                                   AW8624->Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_RTP_MODE);
    // 设置Brake0制动等级
    AW8624->IIC_Write_Device_Value(AW8624->Arduino_IIC_HAPTIC::Device_Value::HAPTIC_SET_BRAKE0_LEVEL,
                                   127);
    // 设置Brake0制动波形数量
    AW8624->IIC_Write_Device_Value(AW8624->Arduino_IIC_HAPTIC::Device_Value::HAPTIC_SET_BRAKE0_P_NUM,
                                   3);
    // 开启Brake1波形
    AW8624->IIC_Write_Device_State(AW8624->Arduino_IIC_HAPTIC::Device::HAPTIC_BRAKE1_MODE,
                                   AW8624->Arduino_IIC_HAPTIC::Device_State::HAPTIC_DEVICE_ON);
    // 设置Brake1制动等级
    AW8624->IIC_Write_Device_Value(AW8624->Arduino_IIC_HAPTIC::Device_Value::HAPTIC_SET_BRAKE1_LEVEL,
                                   31);
    // 设置Brake1制动波形数量
    AW8624->IIC_Write_Device_Value(AW8624->Arduino_IIC_HAPTIC::Device_Value::HAPTIC_SET_BRAKE1_P_NUM,
                                   3);
    // 开启Brake2波形
    AW8624->IIC_Write_Device_State(AW8624->Arduino_IIC_HAPTIC::Device::HAPTIC_BRAKE2_MODE,
                                   AW8624->Arduino_IIC_HAPTIC::Device_State::HAPTIC_DEVICE_ON);
    // 设置Brake2制动等级
    AW8624->IIC_Write_Device_Value(AW8624->Arduino_IIC_HAPTIC::Device_Value::HAPTIC_SET_BRAKE2_LEVEL,
                                   8);
    // 设置Brake2制动波形数量
    AW8624->IIC_Write_Device_Value(AW8624->Arduino_IIC_HAPTIC::Device_Value::HAPTIC_SET_BRAKE2_P_NUM,
                                   1);
    //  设置所有波形增益
    AW8624->IIC_Write_Device_Value(AW8624->Arduino_IIC_HAPTIC::Device_Value::HAPTIC_GAIN_VALUE,
                                   128);
    // 设置芯片模式为激活模式
    AW8624->IIC_Write_Device_State(AW8624->Arduino_IIC_HAPTIC::Device::HAPTIC_CHIP_MODE,
                                   AW8624->Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_ACTIVE_MODE);
    // 开启波形回放
    AW8624->IIC_Write_Device_State(AW8624->Arduino_IIC_HAPTIC::Device::HAPTIC_PLAYBACK_GO_MODE,
                                   AW8624->Arduino_IIC_HAPTIC::Device_State::HAPTIC_DEVICE_ON);
}

void loop()
{
    // 设置芯片模式为激活模式
    AW8624->IIC_Write_Device_State(AW8624->Arduino_IIC_HAPTIC::Device::HAPTIC_CHIP_MODE,
                                   AW8624->Arduino_IIC_HAPTIC::Device_Mode::HAPTIC_ACTIVE_MODE);
    // 开启波形回放
    AW8624->IIC_Write_Device_State(AW8624->Arduino_IIC_HAPTIC::Device::HAPTIC_PLAYBACK_GO_MODE,
                                   AW8624->Arduino_IIC_HAPTIC::Device_State::HAPTIC_DEVICE_ON);

    for (int i = 0; i < 40; i++)
    {
        AW8624->IIC_Write_Device_Data(AW8624->Arduino_IIC_HAPTIC::Device_Value::HAPTIC_RTP_DATA,
                                      haptic_waveform_ordinary, sizeof(haptic_waveform_ordinary));
    }

    delay(1000);

    if (millis() > CycleTime)
    {
        Serial.printf("System running time: %d\n\n", (uint32_t)millis() / 1000);

        Serial.printf("ID: %#X \n", (int32_t)AW8624->IIC_Device_ID());

        Serial.printf("Wave data overflow or DPWM DC Status: %s\n",
                      AW8624->IIC_Read_Device_State(AW8624->Arduino_IIC_HAPTIC::Status_Information::HAPTIC_OVS_STATUS).c_str());
        Serial.printf("Under Voltage Lock Out Status: %s\n",
                      AW8624->IIC_Read_Device_State(AW8624->Arduino_IIC_HAPTIC::Status_Information::HAPTIC_UVLS_STATUS).c_str());
        Serial.printf("RTP FIFO Memory Status: %s\n",
                      AW8624->IIC_Read_Device_State(AW8624->Arduino_IIC_HAPTIC::Status_Information::HAPTIC_RTP_FIFO_MEMORY_STATUS).c_str());
        Serial.printf("Over Current Status: %s\n",
                      AW8624->IIC_Read_Device_State(AW8624->Arduino_IIC_HAPTIC::Status_Information::HAPTIC_OCDS_STATUS).c_str());
        Serial.printf("Over Temperature Status: %s\n",
                      AW8624->IIC_Read_Device_State(AW8624->Arduino_IIC_HAPTIC::Status_Information::HAPTIC_OTS_STATUS).c_str());
        Serial.printf("The Indication Of Playback Done Status: %s\n",
                      AW8624->IIC_Read_Device_State(AW8624->Arduino_IIC_HAPTIC::Status_Information::HAPTIC_DONES_STATUS).c_str());

        Serial.printf("Chip input voltage: %.03f mV\n",
                      AW8624->IIC_Read_Device_Value(AW8624->Arduino_IIC_HAPTIC::Value_Information::HAPTIC_CHIP_INPUT_VOLTAGE));

        CycleTime = millis() + 100;
    }
}
