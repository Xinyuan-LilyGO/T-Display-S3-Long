/*
 * @Description: None
 * @version: V1.0.0
 * @Author: None
 * @Date: 2024-01-26 16:58:25
 * @LastEditors: LILYGO_L
 * @LastEditTime: 2024-01-30 10:24:54
 * @License: GPL 3.0
 */
#include "Arduino_CO5300.h"

Arduino_CO5300::Arduino_CO5300(Arduino_DataBus *bus, int8_t rst,
                               uint8_t r, bool ips, int16_t w, int16_t h,
                               uint8_t col_offset1, uint8_t row_offset1,
                               uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_TFT(bus, rst, r, ips, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
    {
    }

    bool Arduino_CO5300::begin(int32_t speed)
    {
        return Arduino_TFT::begin(speed);
    }

    void Arduino_CO5300::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
    {
        if ((x != _currentX) || (w != _currentW) || (y != _currentY) || (h != _currentH))
        {
            _bus->writeC8D16D16(CO5300_W_CASET, x + _xStart, x + w - 1 + _xStart);
            _bus->writeC8D16D16(CO5300_W_PASET, y + _yStart, y + h - 1 + _yStart);

            _currentX = x;
            _currentY = y;
            _currentW = w;
            _currentH = h;
        }

        _bus->writeCommand(CO5300_W_RAMWR); // write to RAM
    }

    /**************************************************************************/
    /*!
        @brief   Set origin of (0,0) and orientation of TFT display
        @param   m  The index for rotation, from 0-3 inclusive
    */
    /**************************************************************************/
    void Arduino_CO5300::setRotation(uint8_t r)
    {
        // CO5300 does not support rotation
        switch (_rotation)
        {
        case 0:
            Arduino_TFT::setRotation(0);
            r = CO5300_MADCTL_COLOR_ORDER;
            break;
        case 1:
            Arduino_TFT::setRotation(0);
            r = CO5300_MADCTL_COLOR_ORDER | CO5300_MADCTL_X_AXIS_FLIP;
            break;
        case 2:
            Arduino_TFT::setRotation(0);
            r = CO5300_MADCTL_COLOR_ORDER | CO5300_MADCTL_Y_AXIS_FLIP;
            break;
        case 3:
            Arduino_TFT::setRotation(0);
            r = CO5300_MADCTL_COLOR_ORDER | CO5300_MADCTL_X_AXIS_FLIP | CO5300_MADCTL_Y_AXIS_FLIP; // flip horizontal and flip vertical
            break;

        default:
            break;
        }
        _bus->beginWrite();
        _bus->writeC8D8(CO5300_W_MADCTL, r);
        _bus->endWrite();
    }

    void Arduino_CO5300::invertDisplay(bool i)
    {
        _bus->sendCommand((_ips ^ i) ? CO5300_C_INVON : CO5300_C_INVOFF);
    }

    void Arduino_CO5300::displayOn(void)
    {
        _bus->sendCommand(CO5300_C_DISPON);
        delay(CO5300_SLPIN_DELAY);
        _bus->sendCommand(CO5300_C_SLPOUT);
        // _bus->writeC8D8(CO5300_W_DEEPSTMODE, 0x00);
        delay(CO5300_SLPOUT_DELAY);
    }

    void Arduino_CO5300::displayOff(void)
    {
        _bus->sendCommand(CO5300_C_DISPOFF);
        delay(CO5300_SLPIN_DELAY);
        _bus->sendCommand(CO5300_C_SLPIN);
        // _bus->writeC8D8(CO5300_W_DEEPSTMODE, 0x01);
        delay(CO5300_SLPIN_DELAY);
    }

    void Arduino_CO5300::Display_Brightness(uint8_t brightness)
    {
        _bus->beginWrite();
        _bus->writeC8D8(CO5300_W_WDBRIGHTNESSVALNOR, brightness);
        _bus->endWrite();
    }

    void Arduino_CO5300::SetContrast(uint8_t Contrast)
    {
        switch (Contrast)
        {
        case CO5300_ContrastOff:
            _bus->beginWrite();
            _bus->writeC8D8(CO5300_W_WCE, 0x00);
            _bus->endWrite();
            break;
        case CO5300_LowContrast:
            _bus->beginWrite();
            _bus->writeC8D8(CO5300_W_WCE, 0x05);
            _bus->endWrite();
            break;
        case CO5300_MediumContrast:
            _bus->beginWrite();
            _bus->writeC8D8(CO5300_W_WCE, 0x06);
            _bus->endWrite();
            break;
        case CO5300_HighContrast:
            _bus->beginWrite();
            _bus->writeC8D8(CO5300_W_WCE, 0x07);
            _bus->endWrite();
            break;

        default:
            break;
        }
    }

    // Companion code to the above tables.  Reads and issues
    // a series of LCD commands stored in PROGMEM byte array.
    void Arduino_CO5300::tftInit()
    {
        if (_rst != GFX_NOT_DEFINED)
        {
            pinMode(_rst, OUTPUT);
            digitalWrite(_rst, HIGH);
            delay(10);
            digitalWrite(_rst, LOW);
            delay(CO5300_RST_DELAY);
            digitalWrite(_rst, HIGH);
            delay(CO5300_RST_DELAY);
        }
        else
        {
            // Software Rest
            _bus->sendCommand(CO5300_C_SWRESET);
            delay(CO5300_RST_DELAY);
        }

        _bus->batchOperation(co5300_init_operations, sizeof(co5300_init_operations));

        invertDisplay(false);

        // _bus->beginWrite();

        // _bus->writeCommand(CO5300_C_SLPOUT);
        // delay(CO5300_SLPOUT_DELAY);

        // _bus->writeCommand(CO5300_W_SETTSL);
        // _bus->write(0x01);
        // _bus->write(0xD1);

        // _bus->writeC8D8(CO5300_WC_TEARON, 0x00);
        // _bus->writeC8D8(CO5300_W_PIXFMT, 0x55);
        // _bus->writeC8D8(CO5300_W_WCTRLD1, 0x20);
        // _bus->writeCommand(CO5300_C_DISPON);                // Display on
        // _bus->writeC8D8(CO5300_W_WDBRIGHTNESSVALNOR, 0x00); // Brightest brightness

        // _bus->endWrite();

        // delay(1000);
        // displayOff();
        // delay(1000);
        // displayOn();
        // delay(1000);
        // displayOff();
        // delay(1000);
        // displayOn();
    }
