/*
 * @Description:
            GFX_AXS15231B_Image
        Screen test and touch test, when the screen is touched after power-on, it displays touch
    information on the screen and outputs the information to the serial port simultaneously.
 * @Author: LILYGO_L
 * @Date: 2024-07-19 18:02:07
 * @LastEditTime: 2024-07-19 18:11:17
 * @License: GPL 3.0
 */
#include <Arduino.h>
#include "Arduino_GFX_Library.h"
#include "Arduino_DriveBus_Library.h"
#include "pins_config.h"
#include "Material_16Bit_180x640px.h"
#include <Wire.h>

#define LCD_CS TFT_QSPI_CS
#define LCD_SCLK TFT_QSPI_SCK
#define LCD_SDIO0 TFT_QSPI_D0
#define LCD_SDIO1 TFT_QSPI_D1
#define LCD_SDIO2 TFT_QSPI_D2
#define LCD_SDIO3 TFT_QSPI_D3
#define LCD_RST TFT_QSPI_RST

#define LCD_WIDTH 180
#define LCD_HEIGHT 640

static const uint8_t read_touchpad_cmd[] = {0xB5, 0xAB, 0xA5, 0x5A, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00};

volatile bool Touch_INT = false;
static uint8_t Image_Flag = 0;
static size_t CycleTime = 0;

Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    LCD_CS /* CS */, LCD_SCLK /* SCK */, LCD_SDIO0 /* SDIO0 */,
    LCD_SDIO1 /* SDIO1 */, LCD_SDIO2 /* SDIO2 */, LCD_SDIO3 /* SDIO3 */);

Arduino_GFX *gfx = new Arduino_AXS15231(bus, LCD_RST /* RST */, 0 /* rotation */,
                                        false /* IPS */, LCD_WIDTH, LCD_HEIGHT);

std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
    std::make_shared<Arduino_HWIIC>(TOUCH_IICSDA, TOUCH_IICSCL, &Wire);

void AXS15231_Touch(void)
{
    Touch_INT = true;
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Ciallo");

    pinMode(TFT_BL, OUTPUT);
    pinMode(TOUCH_RES, OUTPUT);
    pinMode(TOUCH_INT, INPUT_PULLUP);

    ledcAttachPin(TFT_BL, 1);
    ledcSetup(1, 2000, 8);
    ledcWrite(1, 0);

    attachInterrupt(TOUCH_INT, AXS15231_Touch, FALLING);

    digitalWrite(TOUCH_RES, HIGH);
    delay(2);
    digitalWrite(TOUCH_RES, LOW);
    delay(100);
    digitalWrite(TOUCH_RES, HIGH);
    delay(2);
    IIC_Bus->begin();

    gfx->begin();
    gfx->fillScreen(WHITE);

    for (int i = 0; i <= 255; i++)
    {
        ledcWrite(1, i);
        delay(3);
    }

    gfx->setCursor(40, 300);
    gfx->setTextSize(2);
    gfx->setTextColor(PURPLE);
    gfx->printf("Touch me");
}

void loop()
{
    // Serial.printf("System running time: %d\n\n", (uint32_t)millis() / 1000);

    if (Touch_INT == true)
    {
        Touch_INT = false;
        uint8_t temp_buf[8] = {0};

        if (millis() > CycleTime)
        {
            IIC_Bus->IIC_ReadCData_Data(0x3B, read_touchpad_cmd, sizeof(read_touchpad_cmd),
                                        temp_buf, sizeof(temp_buf));

            CycleTime = millis() + 20;
        }

        uint8_t fingers_number = temp_buf[1];
        uint8_t touch_event = temp_buf[2] >> 4;

        if ((fingers_number == 1) && (touch_event == 0x08)) // Touch inspection and judgment
        {
            uint16_t touch_x = ((uint16_t)(temp_buf[4] & 0B00001111) << 8) | (uint16_t)temp_buf[5];
            uint16_t touch_y = LCD_HEIGHT - (((uint16_t)(temp_buf[2] & 0B00001111) << 8) | (uint16_t)temp_buf[3]);

            switch (Image_Flag)
            {
            case 0:
                gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)gImage_1, LCD_WIDTH, LCD_HEIGHT); // RGB
                break;
            case 1:
                gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)gImage_2, LCD_WIDTH, LCD_HEIGHT); // RGB
                break;
            case 2:
                gfx->fillScreen(BLACK);
                break;
            case 3:
                gfx->fillScreen(WHITE);
                break;
            case 4:
                gfx->fillScreen(BLACK);
                break;
            case 5:
                gfx->fillScreen(WHITE);
                break;
            case 6:
                gfx->fillScreen(BLACK);
                break;
            case 7:
                gfx->fillScreen(WHITE);
                break;

            default:
                break;
            }

            Image_Flag++;

            if (Image_Flag > 7)
            {
                Image_Flag = 0;
            }

            Serial.printf("Fingers Number: %d\n", fingers_number);
            Serial.printf("Touch Event: %#X\n", touch_event);
            Serial.printf("Touch X: %d Y: %d\n", touch_x, touch_y);

            gfx->setTextSize(1);
            gfx->setTextColor(RED);
            gfx->setCursor(touch_x, touch_y);
            gfx->fillCircle(touch_x, touch_y, 2, RED);
            gfx->printf(" Fingers Number: %d\n", fingers_number);
            gfx->setCursor(touch_x, touch_y + 10);
            gfx->printf(" Touch Event: %#X\n", touch_event);
            gfx->setCursor(touch_x, touch_y + 20);
            gfx->printf(" Touch X: %d Y: %d\n", touch_x, touch_y);
        }
    }
}
