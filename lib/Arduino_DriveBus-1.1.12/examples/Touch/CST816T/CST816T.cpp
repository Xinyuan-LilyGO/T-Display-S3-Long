/*
 * @Description(CN):
 *      基于Arduino_DriveBus库的CST816T芯片例程
 *      以下是使用该款芯片需要注意的几个点：
 *      1. 有些CST816T芯片的上下手势可能会颠倒，根据厂家固件决定
 *      2. CST816T暂时只能进入休眠模式而不能退出
 *      3. 在使用Arduino_DriveBus库时，初始化阶段会对芯片进行一次初始化的写入数据，具体
 * 初始化内容可以前往芯片文件下的XXX_Initialization_Operations[]这个数组查看
 *      4. Arduino_DriveBus库使用枚举的方式寄存操作芯片的命令和值，具体的可执行命令已经在
 * 芯片文件的@Description有说明
 *
 *  * @Description(EN):
 *    CST816T chip routine based on Arduino_DriveBus library
 *    Here are a few points to note when using this chip:
 *    1. The up and down gestures of some CST816T chips may be reversed, decided by the manufacturer's firmware.
 *    2. CST816T can only enter sleep mode temporarily and cannot exit it.
 *    3. When using the Arduino_DriveBus library, the chip is initialized with data during the initialization phase.
 *  The specific initialization content can be viewed in the XXX_Initialization_Operations[] array under the chip file.
 *    4. The Arduino_DriveBus library uses enumerations to register commands and values for chip operations.
 *  The specific executable commands are explained in the chip file under @Description.
 *
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2023-08-25 17:09:20
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-02-28 14:24:48
 * @License: GPL 3.0
 */
#include "Arduino_DriveBus_Library.h"

std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
    std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire);

void Arduino_IIC_Touch_Interrupt(void);

std::unique_ptr<Arduino_IIC> CST816T(new Arduino_CST816x(IIC_Bus, CST816T_DEVICE_ADDRESS,
                                                         TP_RST, TP_INT, Arduino_IIC_Touch_Interrupt));

void Arduino_IIC_Touch_Interrupt(void)
{
    CST816T->IIC_Interrupt_Flag = true;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Ciallo");

    while (CST816T->begin() == false)
    {
        Serial.println("CST816T initialization fail");
        delay(2000);
    }
    Serial.println("CST816T initialization successfully");

    // 中断模式为检测到触摸时，发出低脉冲
    //  CST816T->IIC_Write_Device_State(CST816T->Arduino_IIC_Touch::Device::TOUCH_DEVICE_INTERRUPT_MODE,
    //                                  CST816T->Arduino_IIC_Touch::Device_Mode::TOUCH_DEVICE_INTERRUPT_PERIODIC);

    // 目前休眠功能只能进入不能退出 所有不建议开启休眠
    // CST816T->IIC_Write_Device_State(CST816T->Arduino_IIC_Touch::Device::TOUCH_DEVICE_SLEEP_MODE,
    //                                 CST816T->Arduino_IIC_Touch::Device_State::TOUCH_DEVICE_ON);

    Serial.printf("ID: %#X \n\n", (int32_t)CST816T->IIC_Device_ID());
    delay(1000);
}

void loop()
{
    Serial.printf("System running time: %d\n\n", (uint32_t)millis() / 1000);

    if (CST816T->IIC_Interrupt_Flag == true)
    {
        CST816T->IIC_Interrupt_Flag = false;

        Serial.printf("\nGesture:%s\n",
                      (CST816T->IIC_Read_Device_State(CST816T->Arduino_IIC_Touch::Status_Information::TOUCH_GESTURE_ID)).c_str());
        Serial.printf("Fingers Number:%d\n",
                      CST816T->IIC_Read_Device_Value(CST816T->Arduino_IIC_Touch::Value_Information::TOUCH_FINGER_NUMBER));
        Serial.printf("Touch X:%d Y:%d\n\n",
                      CST816T->IIC_Read_Device_Value(CST816T->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X),
                      CST816T->IIC_Read_Device_Value(CST816T->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y));
    }

    delay(1000);
}
