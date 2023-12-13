/**
 * @file      QWIIC_Sensor.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-12-13
 * @note      It only demonstrates how to use QWIIC.
 * * T-Display-Long itself does not have any sensors and needs to be connected to the external sensor through a QWICC cable.
 */
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>    //https://github.com/adafruit/Adafruit_Sensor
#include <Adafruit_BME280.h>    //https://github.com/adafruit/Adafruit_BME280_Library
#include <SparkFunTMP102.h>     //https://www.sparkfun.com/products/16304
#include "AXS15231B.h"

Adafruit_BME280     bme280;     // I2C
TMP102              tmp102;     // I2C

uint32_t delayTime;
bool     findTMP102 = false,
         findBME280 = false;

void setup()
{
    Serial.begin(115200);
    while (!Serial);   // time to get serial running

    pinMode(TOUCH_RES, OUTPUT);
    digitalWrite(TOUCH_RES, HIGH); delay(2);
    digitalWrite(TOUCH_RES, LOW); delay(10);
    digitalWrite(TOUCH_RES, HIGH); delay(2);

    // Touch use Wire Pin 15,10
    Wire.begin(TOUCH_IICSDA, TOUCH_IICSCL);
    axs15231_init();  //If you use touch, you must initialize the screen first

    // BME280 connected QWIIC Pin 10,15 connector
    if (!bme280.begin(0x76, &Wire)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme280.sensorID(), 16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
    } else {
        Serial.println("Connected to BME280!");
        findBME280 = true;
    }

    //  Sensor address can be changed with an external jumper to:
    //  ADD0 - Address
    //  VCC - 0x49
    //  SDA - 0x4A
    //  SCL - 0x4B
    // TMP102 connected QWIIC Pin 43,44 connector
    Wire1.begin(43/*sda*/, 44/*scl*/);
    if (!tmp102.begin(0x49, Wire1)) {
        Serial.println("Cannot connect to TMP102.");
        Serial.println("Is the board connected? Is the device ID correct?");
    } else {
        Serial.println("Connected to TMP102!");
        findTMP102 = true;
    }
    Serial.println();
}


void loop()
{
    if (millis() > delayTime) {
        delayTime = millis() + 2000;
        printBME280();
        printTMP102();
    }
    readTouch();
    delay(10);
}


void printTMP102()
{
    if (!findTMP102) {
        return;
    }

    float temperature;

    // Turn sensor on to start temperature measurement.
    // Current consumtion typically ~10uA.
    tmp102.wakeup();

    // read temperature data
    temperature = tmp102.readTempF();
    //temperature = tmp102.readTempC();

    // Place sensor in sleep mode to save power.
    // Current consumtion typically <0.5uA.
    tmp102.sleep();

    // Print temperature and alarm state
    Serial.print("Temperature: ");
    Serial.print(temperature);

}

void printBME280()
{
    if (!findBME280) {
        return;
    }
    Serial.print("Temperature = ");
    Serial.print(bme280.readTemperature());
    Serial.println(" °C");

    Serial.print("Pressure = ");

    Serial.print(bme280.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme280.readAltitude(1013.25));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme280.readHumidity());
    Serial.println(" %");

    Serial.println();
}

void readTouch()
{

#define AXS_GET_GESTURE_TYPE(buf)  buf[0]
#define AXS_GET_POINT_X(buf,point_index) (((uint16_t)(buf[6*point_index+2] & 0x0F) <<8) + (uint16_t)buf[6*point_index+3])
#define AXS_GET_POINT_Y(buf,point_index) (((uint16_t)(buf[6*point_index+4] & 0x0F) <<8) + (uint16_t)buf[6*point_index+5])

    uint8_t ALS_ADDRESS = 0x3B;
    const uint8_t read_touchpad_cmd[11] = {0xb5, 0xab, 0xa5, 0x5a, 0x0, 0x0, 0x0, 0x8};
    uint8_t buff[20] = {0};
    Wire.beginTransmission(ALS_ADDRESS);
    Wire.write(read_touchpad_cmd, 8);
    Wire.endTransmission();
    Wire.requestFrom(ALS_ADDRESS, 8);
    while (!Wire.available());
    Wire.readBytes(buff, 8);

    uint16_t pointX;
    uint16_t pointY;
    uint16_t type = 0;

    type =  AXS_GET_GESTURE_TYPE(buff);
    pointX = AXS_GET_POINT_X(buff, 0);
    pointY = AXS_GET_POINT_Y(buff, 0);

    if (!type && (pointX || pointY)) {
        pointX = (640 - pointX);
        if (pointX > 640) pointX = 640;
        if (pointY > 180) pointY = 180;

        Serial.printf("x = %d, y = %d\n", pointX, pointY);
    }

}