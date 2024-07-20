/*
 * @Description(CN):
 *      基于Arduino_DriveBus库的CST226SE芯片例程
 *      以下是使用该款芯片需要注意的几个点：
 *      1. CST226SE暂时只能进入休眠模式而不能退出，如需退出需要重置
 *      2. 在使用Arduino_DriveBus库时，初始化阶段会对芯片进行一次初始化的写入数据，具体
 * 初始化内容可以前往芯片文件下的XXX_Initialization_Operations[]这个数组查看
 *      3. Arduino_DriveBus库使用枚举的方式寄存操作芯片的命令和值，具体的可执行命令已经在
 * 芯片文件的@Description有说明
 *
 *  * @Description(EN):
 *    CST226SE chip routine based on Arduino_DriveBus library
 *    Here are a few points to note when using this chip:
 *    1. CST226SE can only enter sleep mode temporarily and cannot exit. If you need to exit, you need to reset it.
 *    2. When using the Arduino_DriveBus library, the chip is initialized with data during the initialization phase.
 *  The specific initialization content can be viewed in the XXX_Initialization_Operations[] array under the chip file.
 *    3. The Arduino_DriveBus library uses enumerations to register commands and values for chip operations.
 *  The specific executable commands are explained in the chip file under @Description.
 *
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2023-08-25 17:09:20
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-02-28 14:19:22
 * @License: GPL 3.0
 */
#include "Arduino_DriveBus_Library.h"

static size_t CycleTime = 0;

std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
    std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire);

void Arduino_IIC_Touch_Interrupt(void);

std::unique_ptr<Arduino_IIC> CST226SE(new Arduino_CST2xxSE(IIC_Bus, CST226SE_DEVICE_ADDRESS,
                                                           TP_RST, TP_INT, Arduino_IIC_Touch_Interrupt));

void Arduino_IIC_Touch_Interrupt(void)
{
    CST226SE->IIC_Interrupt_Flag = true;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Ciallo");

    while (CST226SE->begin() == false)
    {
        Serial.println("CST226SE initialization fail");
        delay(2000);
    }
    Serial.println("CST226SE initialization successfully");

    // 中断模式为检测到手势时，发出低脉冲
    //  CST226SE->IIC_Write_Device_State(CST226SE->Arduino_IIC_Touch::Device::TOUCH_DEVICE_INTERRUPT_MODE,
    //                                  CST226SE->Arduino_IIC_Touch::Device_Mode::TOUCH_INTERRUPT_MOTION);

    // 目前休眠功能只能进入不能退出 要退出只能系统重置
    // CST226SE->IIC_Write_Device_State(CST226SE->Arduino_IIC_Touch::Device::TOUCH_DEVICE_SLEEP_MODE,
    //                                 CST226SE->Arduino_IIC_Touch::Device_State::TOUCH_DEVICE_ON);

    Serial.printf("ID: %#X \n\n", (int32_t)CST226SE->IIC_Device_ID());
    delay(1000);
}

void loop()
{
    Serial.printf("System running time: %d\n\n", (uint32_t)millis() / 1000);

    if (CST226SE->IIC_Interrupt_Flag == true)
    {
        CST226SE->IIC_Interrupt_Flag = false;

        Serial.printf("ID: %#X \n\n", (int32_t)CST226SE->IIC_Device_ID());

        Serial.printf("Fingers Number:%d\n",
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_FINGER_NUMBER));
        Serial.printf("Touch X:%d Y:%d\n",
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X),
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y));

        Serial.printf("\nTouch X1:%d Y1:%d\n",
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH1_COORDINATE_X),
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH1_COORDINATE_Y));
        Serial.printf("Touch X2:%d Y2:%d\n",
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH2_COORDINATE_X),
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH2_COORDINATE_Y));
        Serial.printf("Touch X3:%d Y3:%d\n",
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH3_COORDINATE_X),
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH3_COORDINATE_Y));
        Serial.printf("Touch X4:%d Y4:%d\n",
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH4_COORDINATE_X),
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH4_COORDINATE_Y));
        Serial.printf("Touch X5:%d Y5:%d\n",
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH5_COORDINATE_X),
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH5_COORDINATE_Y));

        Serial.printf("\nTouch1 Pressure Value:%d\n",
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH1_PRESSURE_VALUE));
        Serial.printf("Touch2 Pressure Value:%d\n",
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH2_PRESSURE_VALUE));
        Serial.printf("Touch3 Pressure Value:%d\n",
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH3_PRESSURE_VALUE));
        Serial.printf("Touch4 Pressure Value:%d\n",
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH4_PRESSURE_VALUE));
        Serial.printf("Touch5 Pressure Value:%d\n",
                      (int32_t)CST226SE->IIC_Read_Device_Value(CST226SE->Arduino_IIC_Touch::Value_Information::TOUCH5_PRESSURE_VALUE));
    }

    delay(500);

    if (millis() > CycleTime)
    {
        CycleTime = millis() + 5000;
    }
}
