/*
 * @Description(CN): DMIC系列麦克风测试程序
 *      在串口上发送左右声道信号数据（值的范围在40000 ~ -40000左右）
 *
 * @Description(EN): DMIC series microphone testing program
 *      Send left and right channel signal data on the serial port (value range between 40000 and -40000)
 *
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2023-12-20 16:24:06
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-05-25 17:57:27
 * @License: GPL 3.0
 */
#include "Arduino_DriveBus_Library.h"

#define IIS_SAMPLE_RATE 44100 // 采样速率
#define IIS_DATA_BIT 16       // 数据位数

std::shared_ptr<Arduino_IIS_DriveBus> IIS_Bus =
    std::make_shared<Arduino_HWIIS>(I2S_NUM_1, MSM261_BCLK, MSM261_WS, MSM261_DATA);

std::unique_ptr<Arduino_IIS> MSM261(new Arduino_MEMS(IIS_Bus));

char IIS_Read_Buff[100];

void setup()
{
    Serial.begin(115200);

    while (MSM261->begin(Arduino_IIS_DriveBus::Device_Data_Mode::DATA_IN,
                         IIS_SAMPLE_RATE, IIS_DATA_BIT) == false)
    {
        Serial.println("MSM261 initialization fail");
        delay(2000);
    }
    Serial.println("MSM261 initialization successfully");
}

void loop()
{
    if (MSM261->IIS_Read_Data(IIS_Read_Buff, 100) == true)
    {
        // 输出左右声道数据
        // Serial.printf("Left: %d\n",((int16_t)IIS_Read_Buff[0] | (int16_t)IIS_Read_Buff[1] << 8));
        // Serial.printf("Right: %d\n",((int16_t)IIS_Read_Buff[2] | (int16_t)IIS_Read_Buff[3] << 8));

        // Arduino
        Serial.println(((int16_t)IIS_Read_Buff[0] | (int16_t)IIS_Read_Buff[1] << 8));
        Serial.print(",");
        Serial.print(((int16_t)IIS_Read_Buff[2] | (int16_t)IIS_Read_Buff[3] << 8));
    }
    else
    {
        Serial.printf("Failed to read MSM261 data");
    }

    delay(50);
}
