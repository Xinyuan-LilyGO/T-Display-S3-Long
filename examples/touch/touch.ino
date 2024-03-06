#include <Arduino.h>
#include <Wire.h>
#include "AXS15231B.h"

uint8_t ALS_ADDRESS = 0x3B;
#define TOUCH_IICSCL 10
#define TOUCH_IICSDA 15
#define TOUCH_INT 11
#define TOUCH_RES 16

#define AXS_TOUCH_TWO_POINT_LEN             14  // (AXS_TOUCH_ONE_POINT_LEN *  2) +  2  Bytes
#define AXS_TOUCH_ONE_POINT_LEN             6
#define AXS_TOUCH_BUF_HEAD_LEN              2

#define AXS_TOUCH_GESTURE_POS               0
#define AXS_TOUCH_POINT_NUM                 1
#define AXS_TOUCH_EVENT_POS                 2
#define AXS_TOUCH_X_H_POS                   2
#define AXS_TOUCH_X_L_POS                   3
#define AXS_TOUCH_ID_POS                    4
#define AXS_TOUCH_Y_H_POS                   4
#define AXS_TOUCH_Y_L_POS                   5
#define AXS_TOUCH_WEIGHT_POS                6
#define AXS_TOUCH_AREA_POS                  7

#define AXS_GET_POINT_NUM(buf) buf[AXS_TOUCH_POINT_NUM]
#define AXS_GET_GESTURE_TYPE(buf)  buf[AXS_TOUCH_GESTURE_POS]
#define AXS_GET_POINT_X(buf,point_index) (((uint16_t)(buf[AXS_TOUCH_ONE_POINT_LEN*point_index+AXS_TOUCH_X_H_POS] & 0x0F) <<8) + (uint16_t)buf[AXS_TOUCH_ONE_POINT_LEN*point_index+AXS_TOUCH_X_L_POS])
#define AXS_GET_POINT_Y(buf,point_index) (((uint16_t)(buf[AXS_TOUCH_ONE_POINT_LEN*point_index+AXS_TOUCH_Y_H_POS] & 0x0F) <<8) + (uint16_t)buf[AXS_TOUCH_ONE_POINT_LEN*point_index+AXS_TOUCH_Y_L_POS])
#define AXS_GET_POINT_EVENT(buf,point_index) (buf[AXS_TOUCH_ONE_POINT_LEN*point_index+AXS_TOUCH_EVENT_POS] >> 6)


void setup()
{
    Serial.begin(115200);
    Serial.println("sta\n");

    pinMode(TOUCH_RES, OUTPUT);
    digitalWrite(TOUCH_RES, HIGH); delay(2);
    digitalWrite(TOUCH_RES, LOW); delay(10);
    digitalWrite(TOUCH_RES, HIGH); delay(2);

    Wire.begin(TOUCH_IICSDA, TOUCH_IICSCL);

    axs15231_init();  //If you use touch, you must initialize the screen first

    Serial.println("end\n");
}

uint8_t read_touchpad_cmd[11] = {0xb5, 0xab, 0xa5, 0x5a, 0x0, 0x0, 0x0, 0x8};
void loop()
{
    uint8_t buff[20] = {0};
    Wire.beginTransmission(ALS_ADDRESS);
    Wire.write(read_touchpad_cmd, 8);
    Wire.endTransmission();
    Wire.requestFrom(ALS_ADDRESS, AXS_TOUCH_TWO_POINT_LEN);
    while (!Wire.available());
    Wire.readBytes(buff, AXS_TOUCH_TWO_POINT_LEN);

    uint8_t num = AXS_GET_POINT_NUM(buff);
    uint16_t type = AXS_GET_GESTURE_TYPE(buff);

    if (num && !type) {
        Serial.printf("num:%d \n", num);
        for (int i = 0; i < num; ++i) {
            Serial.print("x"); Serial.print(i); Serial.print(":"); Serial.print(AXS_GET_POINT_X(buff, i));
            Serial.print("y"); Serial.print(i); Serial.print(":"); Serial.print(AXS_GET_POINT_Y(buff, i));
        }
        Serial.println();

    }
    delay(50);
}
