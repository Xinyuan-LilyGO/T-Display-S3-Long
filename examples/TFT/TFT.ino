#include "AXS15231B.h"
#include <Arduino.h>
#include "hyn_core.h"

extern uint8_t test1_180640_map[];
extern uint8_t test2_180640_map[];
extern uint8_t test3_180640_map[];
extern uint8_t test4_180640_map[];
struct hyn_ts_data *ts_data;

bool result = false;
void setup()
{
    Serial.begin(115200);
    Serial.println("sta\n");

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    axs15231_init();
    touch_init(TP_I2C_SDA, TP_I2C_SCL, TP_IRQ, TP_RST);

    lcd_PushColors(0, 0, 180, 640, (uint16_t *)test1_180640_map);

    Serial.println("end\n");
}

extern uint32_t transfer_num;
extern size_t lcd_PushColors_len;
int cont = 0;
void loop()
{
    delay(1);
    cont++;
    ts_data = get_touch_data();
    if(ts_data->rp_buf.rep_num > 0)
    {
        Serial.printf("num:%d xy:(%d,%d)\n", ts_data->rp_buf.rep_num, ts_data->rp_buf.pos_info[0].pos_x, ts_data->rp_buf.pos_info[0].pos_y);
    }

    if (cont >= 12000)
        cont = 0;

    if (transfer_num <= 0 && lcd_PushColors_len <= 0)
    {
#ifdef LCD_SPI_DMA
        char i = 0;
        while (get_lcd_spi_dma_write())
        {
            i = i >> 1;
            lcd_PushColors(0, 0, 0, 0, NULL);
        }
#endif

        if (cont == 0)
            lcd_PushColors(0, 0, 180, 640, (uint16_t *)test1_180640_map);
        else if (cont == 4000)
            lcd_PushColors(0, 0, 180, 640, (uint16_t *)test2_180640_map);
        else if (cont == 8000)
            lcd_PushColors(0, 0, 180, 640, (uint16_t *)test3_180640_map);
    }

    if (transfer_num <= 1 && lcd_PushColors_len > 0)
    {
        lcd_PushColors(0, 0, 0, 0, NULL);
    }
}
