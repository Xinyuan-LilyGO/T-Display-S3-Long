/*
 * @Description(CN):
 *      基于Arduino_DriveBus库的FT3168芯片例程
 *      以下是使用该款芯片需要注意的几个点：
 *      1. 在使用Arduino_DriveBus库时，初始化阶段会对芯片进行一次初始化的写入数据，具体
 * 初始化内容可以前往芯片文件下的XXX_Initialization_Operations[]这个数组查看
 *      2. Arduino_DriveBus库使用枚举的方式寄存操作芯片的命令和值，具体的可执行命令已经在
 * 芯片文件的@Description有说明
 *
 *  * @Description(EN):
 *    FT3168 chip routine based on Arduino_DriveBus library
 *    Here are a few points to note when using this chip:
 *    1. When using the Arduino_DriveBus library, the chip is initialized with data during the initialization phase.
 *  The specific initialization content can be viewed in the XXX_Initialization_Operations[] array under the chip file.
 *    2. The Arduino_DriveBus library uses enumerations to register commands and values for chip operations.
 *  The specific executable commands are explained in the chip file under @Description.
 *
 * @version: V1.1.8
 * @Author: Xk_w
 * @Date: 2023-08-25 17:09:20
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-03-12 09:24:48
 * @License: GPL 3.0
 */
#include "Arduino_DriveBus_Library.h"

static size_t CycleTime = 0;

std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
    std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire);

void Arduino_IIC_Touch_Interrupt(void);

std::unique_ptr<Arduino_IIC> FT3168(new Arduino_FT3x68(IIC_Bus, FT3168_DEVICE_ADDRESS,
                                                       TP_RST, TP_INT, Arduino_IIC_Touch_Interrupt));

void Arduino_IIC_Touch_Interrupt(void)
{
    FT3168->IIC_Interrupt_Flag = true;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Ciallo");

    while (FT3168->begin() == false)
    {
        Serial.println("FT3168 initialization fail");
        delay(2000);
    }
    Serial.println("FT3168 initialization successfully");

    // 触摸芯片功耗模式选择
    // 激活模式
    // FT3168->IIC_Write_Device_State(FT3168->Arduino_IIC_Touch::Device::TOUCH_POWER_MODE,
    //                                FT3168->Arduino_IIC_Touch::Device_Mode::TOUCH_POWER_ACTIVE);
    // 监听触发模式
    FT3168->IIC_Write_Device_State(FT3168->Arduino_IIC_Touch::Device::TOUCH_POWER_MODE,
                                   FT3168->Arduino_IIC_Touch::Device_Mode::TOUCH_POWER_MONITOR);
    // 待机模式
    // FT3168->IIC_Write_Device_State(FT3168->Arduino_IIC_Touch::Device::TOUCH_POWER_MODE,
    //                                FT3168->Arduino_IIC_Touch::Device_Mode::TOUCH_POWER_STANDBY);
    // 休眠模式
    // FT3168->IIC_Write_Device_State(FT3168->Arduino_IIC_Touch::Device::TOUCH_POWER_MODE,
    //                                FT3168->Arduino_IIC_Touch::Device_Mode::TOUCH_POWER_HIBERNATE);

    // 开启特殊手势模式
    // FT3168->IIC_Write_Device_State(FT3168->Arduino_IIC_Touch::Device::TOUCH_GESTUREID_MODE,
    //                                FT3168->Arduino_IIC_Touch::Device_State::TOUCH_DEVICE_ON);
    // 开启近距离感应模式
    // FT3168->IIC_Write_Device_State(FT3168->Arduino_IIC_Touch::Device::TOUCH_PROXIMITY_SENSING_MODE,
    //                                FT3168->Arduino_IIC_Touch::Device_State::TOUCH_DEVICE_ON);

    // 触摸芯片是否自动进入Monitor模式
    // FT3168->IIC_Write_Device_State(FT3168->Arduino_IIC_Touch::Device::TOUCH_AUTOMATICALLY_MONITOR_MODE,
    //                                FT3168->Arduino_IIC_Touch::Device_State::TOUCH_DEVICE_ON);
    // 设定触摸芯片自动进入Monitor模式的时间为10秒
    // FT3168->IIC_Write_Device_Value(FT3168->Arduino_IIC_Touch::Device_Value::TOUCH_AUTOMATICALLY_MONITOR_TIME,
    //                                10);

    Serial.printf("ID: %#X \n\n", (int32_t)FT3168->IIC_Device_ID());
    delay(1000);
}

void loop()
{
    Serial.printf("System running time: %d\n\n", (uint32_t)millis() / 1000);

    if (FT3168->IIC_Interrupt_Flag == true)
    {
        FT3168->IIC_Interrupt_Flag = false;

        Serial.printf("ID: %#X \n", (int32_t)FT3168->IIC_Device_ID());

        Serial.printf("\nGesture:%s\n",
                      (FT3168->IIC_Read_Device_State(FT3168->Arduino_IIC_Touch::Status_Information::TOUCH_GESTURE_ID)).c_str());

        Serial.printf("Fingers Number:%d\n",
                      (int32_t)FT3168->IIC_Read_Device_Value(FT3168->Arduino_IIC_Touch::Value_Information::TOUCH_FINGER_NUMBER));

        Serial.printf("Touch X:%d Y:%d\n",
                      (int32_t)FT3168->IIC_Read_Device_Value(FT3168->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X),
                      (int32_t)FT3168->IIC_Read_Device_Value(FT3168->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y));

        Serial.printf("\nTouch X1:%d Y1:%d\n",
                      (int32_t)FT3168->IIC_Read_Device_Value(FT3168->Arduino_IIC_Touch::Value_Information::TOUCH1_COORDINATE_X),
                      (int32_t)FT3168->IIC_Read_Device_Value(FT3168->Arduino_IIC_Touch::Value_Information::TOUCH1_COORDINATE_Y));
        Serial.printf("Touch X2:%d Y2:%d\n",
                      (int32_t)FT3168->IIC_Read_Device_Value(FT3168->Arduino_IIC_Touch::Value_Information::TOUCH2_COORDINATE_X),
                      (int32_t)FT3168->IIC_Read_Device_Value(FT3168->Arduino_IIC_Touch::Value_Information::TOUCH2_COORDINATE_Y));
    }

    delay(500);

    if (millis() > CycleTime)
    {
        CycleTime = millis() + 5000;
    }
}
