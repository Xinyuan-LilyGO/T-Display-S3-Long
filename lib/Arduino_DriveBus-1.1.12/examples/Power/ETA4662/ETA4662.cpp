/*
 * @Description(CN):
 *      基于Arduino_DriveBus库的ETA4662充电芯片例程
 *      以下是使用该款芯片需要注意的几个点：
 *      1. ETA4662默认状态下为关闭充电功能的状态，需要手动开启充电
 *      2. 在使用Arduino_DriveBus库时，初始化阶段会对芯片进行一次初始化的写入数据，具体
 * 初始化内容可以前往芯片文件下的XXX_Initialization_Operations[]这个数组查看
 *      3. Arduino_DriveBus库使用枚举的方式寄存操作芯片的命令和值，具体的可执行命令已经在
 * 芯片文件的@Description有说明
 *
 *  @Description(EN):
 *      ETA4662 charging chip routine based on Arduino_DriveBus library
 *      Here are a few points to note when using this chip:
 *      1. The ETA4662 is in a default state of disabling charging, which needs to be manually enabled.
 *      2. When using the Arduino_DriveBus library, the chip is initialized during the initialization phase
 *  by writing data to it. The specific initialization content can be viewed in the XXX_Initialization_Operations[]
 *  array under the chip file.
 *      3. The Arduino_DriveBus library uses enumerations to register commands and values for operating the chip.
 *  The specific executable commands are explained in the chip file under @Description.
 *
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2023-11-27 10:08:51
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-02-28 14:19:22
 * @License: GPL 3.0
 */
#include "Arduino_DriveBus_Library.h"

std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
    std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire);

std::unique_ptr<Arduino_IIC> ETA4662(new Arduino_ETA4662(IIC_Bus, ETA4662_DEVICE_ADDRESS,
                                                         DRIVEBUS_DEFAULT_VALUE, DRIVEBUS_DEFAULT_VALUE));

void setup()
{
    Serial.begin(115200);
    Serial.println("Ciallo");

    while (ETA4662->begin() == false)
    {
        Serial.println("ETA4662 initialization fail");
        delay(2000);
    }
    Serial.println("ETA4662 initialization successfully");

    ETA4662->IIC_Write_Device_State(ETA4662->Arduino_IIC_Power::Device::POWER_DEVICE_CHARGING_MODE,
                                    ETA4662->Arduino_IIC_Power::Device_State::POWER_DEVICE_ON); // 充电
    // ETA4662->IIC_Write_Device_State(ETA4662->Arduino_IIC_Power::Device::POWER_DEVICE_WATCHDOG_MODE,
    //                                 ETA4662->Arduino_IIC_Power::Device_State::POWER_DEVICE_ON);                 // 看门狗（当启动ETA4662的看门狗时，看门狗的定时器到达指定值后将断开电源重新连接，与ETA4662通信的MCU将重启）
    // ETA4662->IIC_Write_Device_Value(ETA4662->Arduino_IIC_Power::Device_Value::POWER_DEVICE_WATCHDOG_TIMER, 80); // 看门狗定时器值

    // 热调节阈值设置为60度
    ETA4662->IIC_Write_Device_Value(ETA4662->Arduino_IIC_Power::Device_Value::POWER_DEVICE_THERMAL_REGULATION_THRESHOLD, 60);
    // 最小输入电压设置为4760mV
    ETA4662->IIC_Write_Device_Value(ETA4662->Arduino_IIC_Power::Device_Value::POWER_DEVICE_MINIMUM_INPUT_VOLTAGE_LIMIT, 4760);
    // 充电目标电压电压设置为4215mV
    ETA4662->IIC_Write_Device_Value(ETA4662->Arduino_IIC_Power::Device_Value::POWER_DEVICE_CHARGING_TARGET_VOLTAGE_LIMIT, 4215);
    // 系统电压设置为4200mV（输出电压，该值不能设置过高）
    ETA4662->IIC_Write_Device_Value(ETA4662->Arduino_IIC_Power::Device_Value::POWER_DEVICE_SYSTEM_VOLTAGE_LIMIT, 4200);
    // 输入电流限制设置为500mA
    ETA4662->IIC_Write_Device_Value(ETA4662->Arduino_IIC_Power::Device_Value::POWER_DEVICE_INPUT_CURRENT_LIMIT, 500);
    // 快速充电电流限制设置为456mA
    ETA4662->IIC_Write_Device_Value(ETA4662->Arduino_IIC_Power::Device_Value::POWER_DEVICE_FAST_CHARGING_CURRENT_LIMIT, 456);
    // 终端充电和预充电电流限制设置为5mA
    ETA4662->IIC_Write_Device_Value(ETA4662->Arduino_IIC_Power::Device_Value::POWER_DEVICE_TERMINATION_PRECHARGE_CHARGING_CURRENT_LIMIT, 5);
    // BAT到SYS的放电电流限制设置为2200mA
    ETA4662->IIC_Write_Device_Value(ETA4662->Arduino_IIC_Power::Device_Value::POWER_DEVICE_BAT_TO_SYS_DISCHARGE_CURRENT_LIMIT, 2200);
}
void loop()
{
    Serial.printf("--------------------ETA4662--------------------\n");
    Serial.printf("System running time: %d\n\n", (uint32_t)millis() / 1000);
    Serial.printf("IIC_Bus.use_count(): %d\n\n", (int32_t)IIC_Bus.use_count());

    Serial.printf("ID: %#X \n", (int32_t)ETA4662->IIC_Device_ID());

    Serial.printf("\nCharging Status: %s \n",
                  (ETA4662->IIC_Read_Device_State(ETA4662->Arduino_IIC_Power::Status_Information::POWER_CHARGING_STATUS)).c_str());
    Serial.printf("Input Source Status: %s \n",
                  (ETA4662->IIC_Read_Device_State(ETA4662->Arduino_IIC_Power::Status_Information::POWER_INPUT_SOURCE_STATUS)).c_str());
    Serial.printf("System Voltage Status: %s \n",
                  (ETA4662->IIC_Read_Device_State(ETA4662->Arduino_IIC_Power::Status_Information::POWER_SYSTEM_VOLTAGE_STATUS)).c_str());
    Serial.printf("Thermal Regulation Status: %s \n",
                  (ETA4662->IIC_Read_Device_State(ETA4662->Arduino_IIC_Power::Status_Information::POWER_THERMAL_REGULATION_STATUS)).c_str());

    Serial.printf("\nWatchdog Fault Status: %s \n",
                  (ETA4662->IIC_Read_Device_State(ETA4662->Arduino_IIC_Power::Status_Information::POWER_WATCHDOG_FAULT_STATUS)).c_str());
    Serial.printf("Input Fault Status: %s \n",
                  (ETA4662->IIC_Read_Device_State(ETA4662->Arduino_IIC_Power::Status_Information::POWER_INPUT_FAULT_STATUS)).c_str());
    Serial.printf("Thermal Shutdown Fault Status: %s \n",
                  (ETA4662->IIC_Read_Device_State(ETA4662->Arduino_IIC_Power::Status_Information::POWER_THERMAL_SHUTDOWN_FAULT_STATUS)).c_str());
    Serial.printf("Battery Fault Status: %s \n",
                  (ETA4662->IIC_Read_Device_State(ETA4662->Arduino_IIC_Power::Status_Information::POWER_BATTERY_FAULT_STATUS)).c_str());
    Serial.printf("Safety Timer Fault Status: %s \n",
                  (ETA4662->IIC_Read_Device_State(ETA4662->Arduino_IIC_Power::Status_Information::POWER_SAFETY_TIMER_STATUS_FAULT_STATUS)).c_str());
    Serial.printf("NTC Fault Status: %s \n",
                  (ETA4662->IIC_Read_Device_State(ETA4662->Arduino_IIC_Power::Status_Information::POWER_NTC_FAULT_STATUS)).c_str());

    Serial.printf("\nThermal Regulation Threshold: %d ^C \n",
                  ETA4662->IIC_Read_Device_Value(ETA4662->Arduino_IIC_Power::Value_Information::POWER_THERMAL_REGULATION_THRESHOLD));

    Serial.printf("\nInput Minimum Voltage Limit: %d mV \n",
                  ETA4662->IIC_Read_Device_Value(ETA4662->Arduino_IIC_Power::Value_Information::POWER_MINIMUM_INPUT_VOLTAGE_LIMIT));
    Serial.printf("Charging Voltage Limit: %d mV \n",
                  ETA4662->IIC_Read_Device_Value(ETA4662->Arduino_IIC_Power::Value_Information::POWER_CHARGING_TARGET_VOLTAGE_LIMIT));
    Serial.printf("System Voltage Limit: %d mV \n",
                  ETA4662->IIC_Read_Device_Value(ETA4662->Arduino_IIC_Power::Value_Information::POWER_SYSTEM_VOLTAGE_LIMIT));
    Serial.printf("Input Current Limit: %d mA \n",
                  ETA4662->IIC_Read_Device_Value(ETA4662->Arduino_IIC_Power::Value_Information::POWER_INPUT_CURRENT_LIMIT));
    Serial.printf("Fast Charge Current Limit: %d mA \n",
                  ETA4662->IIC_Read_Device_Value(ETA4662->Arduino_IIC_Power::Value_Information::POWER_FAST_CHARGING_CURRENT_LIMIT));
    Serial.printf("Termination And Precondition Charge Current Limit: %d mA \n",
                  ETA4662->IIC_Read_Device_Value(ETA4662->Arduino_IIC_Power::Value_Information::POWER_TERMINATION_PRECHARGE_CHARGING_CURRENT_LIMIT));
    Serial.printf("BAT To SYS Discharge Current Limit: %d mA \n",
                  ETA4662->IIC_Read_Device_Value(ETA4662->Arduino_IIC_Power::Value_Information::POWER_BAT_TO_SYS_DISCHARGE_CURRENT_LIMIT));

    Serial.printf("--------------------ETA4662--------------------\n");

    delay(1000);
}
