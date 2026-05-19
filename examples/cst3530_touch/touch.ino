#include <Arduino.h>
#include <Wire.h>
#include "hyn_core.h"
#include "pins_config.h"

struct hyn_ts_data *tp_data;

void setup()
{
    Serial.begin(115200);
    Serial.println("sta\n");

    touch_init(TP_I2C_SDA, TP_I2C_SCL, TP_IRQ, TP_RST);

    Serial.println("end\n");
}

void loop()
{
    tp_data = get_touch_data();
    if(tp_data->rp_buf.rep_num > 0)
    {
        Serial.printf("num:%d xy:(%d,%d)\n", tp_data->rp_buf.rep_num, tp_data->rp_buf.pos_info[0].pos_x, tp_data->rp_buf.pos_info[0].pos_y);
    }
    delay(50);
}
