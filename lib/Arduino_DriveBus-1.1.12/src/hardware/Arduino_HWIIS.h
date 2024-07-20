/*
 * @Description(CN):
 *      这是一个使用硬件IIS的IIS底层驱动文件
 *
 * @Description(EN):
 *      This is a bottom-layer IIS driver file using hardware IIS.
 *
 * @version: V1.0.0
 * @Author: Xk_w
 * @Date: 2023-12-20 15:45:17
 * @LastEditors: Xk_w
 * @LastEditTime: 2023-12-22 14:04:29
 * @License: GPL 3.0
 */
#pragma once

#include <driver/i2s.h>
#include "../Arduino_DriveBus.h"

class Arduino_HWIIS : public Arduino_IIS_DriveBus
{
public:
    Arduino_HWIIS(i2s_port_t iis_num = I2S_NUM_0, int8_t bclk = DRIVEBUS_DEFAULT_VALUE,
                  int8_t lrck = DRIVEBUS_DEFAULT_VALUE, int8_t data = DRIVEBUS_DEFAULT_VALUE);

    bool begin(int8_t device_state = DRIVEBUS_DEFAULT_VALUE,
               int32_t sample_rate = 44100UL, int32_t bits_per_sample = 16) override;

    size_t Read(char *data, size_t len) override;
    size_t Write(char *data, size_t len) override;

private:
    i2s_port_t _iis_num;
    int8_t _bclk, _lrck, _data;
};