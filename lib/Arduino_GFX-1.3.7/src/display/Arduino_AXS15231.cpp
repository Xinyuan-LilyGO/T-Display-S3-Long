#include "Arduino_AXS15231.h"

Arduino_AXS15231::Arduino_AXS15231(Arduino_DataBus *bus, int8_t rst, uint8_t r, bool ips, int16_t w, int16_t h)
    : Arduino_TFT(bus, rst, r, ips, w, h, 0, 0, 0, 0)
{
}

bool Arduino_AXS15231::begin(int32_t speed)
{
    return Arduino_TFT::begin(speed);
}

void Arduino_AXS15231::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    if ((x != _currentX) || (w != _currentW))
    {
        _currentX = x;
        _currentW = w;
        x += _xStart;
        _bus->writeC8D16D16(AXS15231_W_CASET, x, x + w - 1);
    }

    if ((y != _currentY) || (h != _currentH))
    {
        _currentY = y;
        _currentH = h;
        y += _yStart;
        _bus->writeC8D16D16(AXS15231_W_PASET, y, y + h - 1);
    }

    _bus->writeCommand(AXS15231_W_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_AXS15231::setRotation(uint8_t r)
{
    // AXS15231 does not support rotation
    switch (_rotation)
    {
    case 0:
        Arduino_TFT::setRotation(0);
        r = AXS15231_MADCTL_COLOR_ORDER;
        break;
    case 1:
        Arduino_TFT::setRotation(0);
        r = AXS15231_MADCTL_COLOR_ORDER | AXS15231_MADCTL_X_AXIS_FLIP;
        break;
    case 2:
        Arduino_TFT::setRotation(0);
        r = AXS15231_MADCTL_COLOR_ORDER | AXS15231_MADCTL_Y_AXIS_FLIP;
        break;
    case 3:
        Arduino_TFT::setRotation(0);
        r = AXS15231_MADCTL_COLOR_ORDER | AXS15231_MADCTL_X_AXIS_FLIP | AXS15231_MADCTL_Y_AXIS_FLIP; // flip horizontal and flip vertical
        break;

    default:
        break;
    }
    _bus->beginWrite();
    _bus->writeC8D8(AXS15231_W_MADCTL, r);
    _bus->endWrite();
}

void Arduino_AXS15231::invertDisplay(bool i)
{
    _bus->sendCommand((_ips ^ i) ? AXS15231_C_INVON : AXS15231_C_INVOFF);
}

void Arduino_AXS15231::displayOn(void)
{
    _bus->sendCommand(AXS15231_C_DISPON);
    delay(AXS15231_SLPIN_DELAY);
    _bus->sendCommand(AXS15231_C_SLPOUT);
    // _bus->writeC8D8(AXS15231_W_DEEPSTMODE, 0x00);
    delay(AXS15231_SLPOUT_DELAY);
}

void Arduino_AXS15231::displayOff(void)
{
    _bus->sendCommand(AXS15231_C_DISPOFF);
    delay(AXS15231_SLPIN_DELAY);
    _bus->sendCommand(AXS15231_C_SLPIN);
    // _bus->writeC8D8(AXS15231_W_DEEPSTMODE, 0x01);
    delay(AXS15231_SLPIN_DELAY);
}

void Arduino_AXS15231::Display_Brightness(uint8_t brightness)
{
    _bus->beginWrite();
    _bus->writeC8D8(AXS15231_W_WDBRIGHTNESSVALNOR, brightness);
    _bus->endWrite();
}

void Arduino_AXS15231::SetContrast(uint8_t Contrast)
{
    switch (Contrast)
    {
    case AXS15231_ContrastOff:
        _bus->beginWrite();
        _bus->writeC8D8(AXS15231_W_WCE, 0x00);
        _bus->endWrite();
        break;
    case AXS15231_LowContrast:
        _bus->beginWrite();
        _bus->writeC8D8(AXS15231_W_WCE, 0x05);
        _bus->endWrite();
        break;
    case AXS15231_MediumContrast:
        _bus->beginWrite();
        _bus->writeC8D8(AXS15231_W_WCE, 0x06);
        _bus->endWrite();
        break;
    case AXS15231_HighContrast:
        _bus->beginWrite();
        _bus->writeC8D8(AXS15231_W_WCE, 0x07);
        _bus->endWrite();
        break;

    default:
        break;
    }
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_AXS15231::tftInit()
{
    if (_rst != GFX_NOT_DEFINED)
    {
        pinMode(_rst, OUTPUT);
        digitalWrite(_rst, HIGH);
        delay(10);
        digitalWrite(_rst, LOW);
        delay(AXS15231_RST_DELAY);
        digitalWrite(_rst, HIGH);
        delay(AXS15231_RST_DELAY);
    }
    else
    {
        // Software Rest
        _bus->sendCommand(AXS15231_C_SWRESET);
        delay(AXS15231_RST_DELAY);
    }

    _bus->batchOperation(AXS15231_init_operations, sizeof(AXS15231_init_operations));

    invertDisplay(false);

    // _bus->beginWrite();

    // _bus->writeCommand(AXS15231_C_SLPOUT);
    // delay(AXS15231_SLPOUT_DELAY);

    // _bus->writeCommand(AXS15231_W_SETTSL);
    // _bus->write(0x01);
    // _bus->write(0xD1);

    // _bus->writeC8D8(AXS15231_WC_TEARON, 0x00);
    // _bus->writeC8D8(AXS15231_W_PIXFMT, 0x55);
    // _bus->writeC8D8(AXS15231_W_WCTRLD1, 0x20);
    // _bus->writeCommand(AXS15231_C_DISPON);                // Display on
    // _bus->writeC8D8(AXS15231_W_WDBRIGHTNESSVALNOR, 0x00); // Brightest brightness

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
