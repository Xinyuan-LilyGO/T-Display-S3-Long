/*
 * @Description(CN):
 *      Arduino_DriveBus_Library是使用Arduino_DriveBus库时需要引用的
 *  其中包含了所有需要引用的库 使用的时候只需要调用这个头文件就行了
 *
 * @Description(EN):
 *      Arduino_DriveBus_Library is the header file to be referenced when using
 * the Arduino_DriveBus library, which contains all the necessary libraries to be referenced.
 * Just call this header file when using it.
 *
 * @version: V1.1.5
 * @Author: Xk_w
 * @Date: 2023-11-16 15:49:20
 * @LastEditors: Xk_w
 * @LastEditTime: 2024-05-15 16:47:07
 * @License: GPL 3.0
 */
#pragma once

#include "Arduino_DriveBus.h"

#include "./hardware/Arduino_HWIIC.h"
#include "./hardware/Arduino_HWIIS.h"

#include "Arduino_IIC.h"
#include "Arduino_IIS.h"

#include "./power_chip/Arduino_ETA4662.h"
#include "./power_chip/Arduino_SY6970.h"

#include "./touch_chip/Arduino_CST816x.h"
#include "./touch_chip/Arduino_CST2xxSE.h"
#include "./touch_chip/Arduino_FT3x68.h"

#include "./iis_chip/Arduino_MEMS.h"
#include "./iis_chip/Arduino_Amplifier.h"

#include "./imu_chip/Arduino_LSM6DSL.h"

#include "./rtc_chip/Arduino_PCF8563.h"
#include "./rtc_chip/Arduino_PCF85063.h"

#include "./haptic_chip/Arduino_AW8624.h"
#include "./haptic_chip/Haptic_Chip_Waveform_library.h"
