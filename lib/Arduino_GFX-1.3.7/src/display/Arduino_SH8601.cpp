#include "Arduino_SH8601.h"

Arduino_SH8601::Arduino_SH8601(Arduino_DataBus *bus, int8_t rst, uint8_t r, bool ips, int16_t w, int16_t h)
    : Arduino_TFT(bus, rst, r, ips, w, h, 0, 0, 0, 0)
{
}

bool Arduino_SH8601::begin(int32_t speed)
{
    return Arduino_TFT::begin(speed);
}

void Arduino_SH8601::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    if ((x != _currentX) || (w != _currentW))
    {
        _currentX = x;
        _currentW = w;
        x += _xStart;
        _bus->writeC8D16D16(SH8601_W_CASET, x, x + w - 1);
    }

    if ((y != _currentY) || (h != _currentH))
    {
        _currentY = y;
        _currentH = h;
        y += _yStart;
        _bus->writeC8D16D16(SH8601_W_PASET, y, y + h - 1);
    }

    _bus->writeCommand(SH8601_W_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_SH8601::setRotation(uint8_t r)
{
    // Sh8601 does not support rotation
    switch (_rotation)
    {
    case 0:
        Arduino_TFT::setRotation(0);
        r = SH8601_MADCTL_COLOR_ORDER;
        break;
    case 1:
        Arduino_TFT::setRotation(0);
        r = SH8601_MADCTL_COLOR_ORDER | SH8601_MADCTL_X_AXIS_FLIP;
        break;
    case 2:
        Arduino_TFT::setRotation(0);
        r = SH8601_MADCTL_COLOR_ORDER | SH8601_MADCTL_Y_AXIS_FLIP;
        break;
    case 3:
        Arduino_TFT::setRotation(0);
        r = SH8601_MADCTL_COLOR_ORDER | SH8601_MADCTL_X_AXIS_FLIP | SH8601_MADCTL_Y_AXIS_FLIP; // flip horizontal and flip vertical
        break;

    default:
        break;
    }
    _bus->beginWrite();
    _bus->writeC8D8(SH8601_W_MADCTL, r);
    _bus->endWrite();
}

void Arduino_SH8601::invertDisplay(bool i)
{
    _bus->sendCommand((_ips ^ i) ? SH8601_C_INVON : SH8601_C_INVOFF);
}

void Arduino_SH8601::displayOn(void)
{
    _bus->sendCommand(SH8601_C_DISPON);
    delay(SH8601_SLPIN_DELAY);
    _bus->sendCommand(SH8601_C_SLPOUT);
    // _bus->writeC8D8(SH8601_W_DEEPSTMODE, 0x00);
    delay(SH8601_SLPOUT_DELAY);
}

void Arduino_SH8601::displayOff(void)
{
    _bus->sendCommand(SH8601_C_DISPOFF);
    delay(SH8601_SLPIN_DELAY);
    _bus->sendCommand(SH8601_C_SLPIN);
    // _bus->writeC8D8(SH8601_W_DEEPSTMODE, 0x01);
    delay(SH8601_SLPIN_DELAY);
}

void Arduino_SH8601::Display_Brightness(uint8_t brightness)
{
    _bus->beginWrite();
    _bus->writeC8D8(SH8601_W_WDBRIGHTNESSVALNOR, brightness);
    _bus->endWrite();
}

void Arduino_SH8601::SetContrast(uint8_t Contrast)
{
    switch (Contrast)
    {
    case SH8601_ContrastOff:
        _bus->beginWrite();
        _bus->writeC8D8(SH8601_W_WCE, 0x00);
        _bus->endWrite();
        break;
    case SH8601_LowContrast:
        _bus->beginWrite();
        _bus->writeC8D8(SH8601_W_WCE, 0x05);
        _bus->endWrite();
        break;
    case SH8601_MediumContrast:
        _bus->beginWrite();
        _bus->writeC8D8(SH8601_W_WCE, 0x06);
        _bus->endWrite();
        break;
    case SH8601_HighContrast:
        _bus->beginWrite();
        _bus->writeC8D8(SH8601_W_WCE, 0x07);
        _bus->endWrite();
        break;

    default:
        break;
    }
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_SH8601::tftInit()
{
    if (_rst != GFX_NOT_DEFINED)
    {
        pinMode(_rst, OUTPUT);
        digitalWrite(_rst, HIGH);
        delay(10);
        digitalWrite(_rst, LOW);
        delay(SH8601_RST_DELAY);
        digitalWrite(_rst, HIGH);
        delay(SH8601_RST_DELAY);
    }
    else
    {
        // Software Rest
        _bus->sendCommand(SH8601_C_SWRESET);
        delay(SH8601_RST_DELAY);
    }

    _bus->batchOperation(sh8601_init_operations, sizeof(sh8601_init_operations));

    invertDisplay(false);

    // _bus->beginWrite();

    // _bus->writeCommand(SH8601_C_SLPOUT);
    // delay(SH8601_SLPOUT_DELAY);

    // _bus->writeCommand(SH8601_W_SETTSL);
    // _bus->write(0x01);
    // _bus->write(0xD1);

    // _bus->writeC8D8(SH8601_WC_TEARON, 0x00);
    // _bus->writeC8D8(SH8601_W_PIXFMT, 0x55);
    // _bus->writeC8D8(SH8601_W_WCTRLD1, 0x20);
    // _bus->writeCommand(SH8601_C_DISPON);                // Display on
    // _bus->writeC8D8(SH8601_W_WDBRIGHTNESSVALNOR, 0x00); // Brightest brightness

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
