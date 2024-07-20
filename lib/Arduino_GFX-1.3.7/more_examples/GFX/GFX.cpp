/*
 * @Description: None
 * @version: V1.0.0
 * @Author: LILYGO_L
 * @Date: 2023-09-06 10:58:19
 * @LastEditors: LILYGO_L
 * @LastEditTime: 2023-09-19 11:28:49
 * @License: GPL 3.0
 */
#include <Arduino.h>
#include "Arduino_GFX_Library.h"
#include "pin_config.h"

// NFP099B-01A
// Arduino_DataBus *bus = new Arduino_SWSPI(
//     LCD_DC /* DC */, LCD_CS /* CS */, LCD_SCLK /* SCK */, LCD_MOSI /* MOSI */, -1 /* MISO */);

// Arduino_GFX *gfx = new Arduino_GC9107(
//     bus, LCD_RST /* RST */, 0 /* rotation */, true /* IPS */,
//     LCD_WIDTH /* width */, LCD_HEIGHT /* height */,
//     0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col_offset2 */, 0 /* row_offset2 */);

// DO0143FAT01
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    LCD_CS /* CS */, LCD_SCLK /* SCK */, LCD_SDIO0 /* SDIO0 */, LCD_SDIO1 /* SDIO1 */, LCD_SDIO2 /* SDIO2 */, LCD_SDIO3 /* SDIO3 */);

Arduino_GFX *gfx = new Arduino_SH8601(bus, LCD_RST /* RST */, 0 /* rotation */, false /* IPS */, LCD_WIDTH, LCD_HEIGHT);

void setup()
{
    Serial.begin(115200);
    Serial.println("Ciallo");

    // NFP099B-01A
    // ledcSetup(0, 20000, 8);
    // ledcAttachPin(LCD_BL, 0);
    // ledcWrite(0, 255);

    gfx->begin();
    gfx->fillScreen(PINK);

    // DO0143FAT01
    for (uint8_t i = 0; i < 255; i++)
    {
        gfx->Display_Brightness(i);
        delay(3);
    }

    for (uint8_t i = 0; i < 4; i++)
    {
        gfx->SetContrast(i);
        delay(2000);
    }
    gfx->SetContrast(SH8601_ContrastOff);

    gfx->setCursor(20, 250);
    gfx->setTextColor(YELLOW);
    gfx->println("Ciallo1~(L *##*L)^**");
}

void loop()
{
    // gfx->fillScreen(PINK);
    delay(1000);
}
