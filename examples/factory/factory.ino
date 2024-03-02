#include "lvgl.h" /* https://github.com/lvgl/lvgl.git */
#include "AXS15231B.h"
#include "WiFi.h"
#include "factory_gui.h"
#include "sntp.h"
#include "time.h"
#include "zones.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <esp_wifi.h>
#include <XPowersLib.h>
#define TOUCH_MODULES_CST_SELF
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "freertos/semphr.h"
SemaphoreHandle_t xSemaphore = NULL;
PowersSY6970      PMU;

//================================
// If you turn on software rotation(disp_drv.sw_rotate = 1), Do not update or replace LVGL.
// disp_drv.full_refresh must be 1
//================================

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";

static lv_disp_draw_buf_t draw_buf;
static lv_color_t        *buf;
static lv_color_t        *buf1;

uint8_t ALS_ADDRESS = 0x3B;


#define AXS_TOUCH_ONE_POINT_LEN 6
#define AXS_TOUCH_BUF_HEAD_LEN  2

#define AXS_TOUCH_GESTURE_POS 0
#define AXS_TOUCH_POINT_NUM   1
#define AXS_TOUCH_EVENT_POS   2
#define AXS_TOUCH_X_H_POS     2
#define AXS_TOUCH_X_L_POS     3
#define AXS_TOUCH_ID_POS      4
#define AXS_TOUCH_Y_H_POS     4
#define AXS_TOUCH_Y_L_POS     5
#define AXS_TOUCH_WEIGHT_POS  6
#define AXS_TOUCH_AREA_POS    7

#define AXS_GET_POINT_NUM(buf)                buf[AXS_TOUCH_POINT_NUM]
#define AXS_GET_GESTURE_TYPE(buf)             buf[AXS_TOUCH_GESTURE_POS]
#define AXS_GET_POINT_X(buf, point_index)     (((uint16_t)(buf[AXS_TOUCH_ONE_POINT_LEN * point_index + AXS_TOUCH_X_H_POS] & 0x0F) << 8) + (uint16_t)buf[AXS_TOUCH_ONE_POINT_LEN * point_index + AXS_TOUCH_X_L_POS])
#define AXS_GET_POINT_Y(buf, point_index)     (((uint16_t)(buf[AXS_TOUCH_ONE_POINT_LEN * point_index + AXS_TOUCH_Y_H_POS] & 0x0F) << 8) + (uint16_t)buf[AXS_TOUCH_ONE_POINT_LEN * point_index + AXS_TOUCH_Y_L_POS])
#define AXS_GET_POINT_EVENT(buf, point_index) (buf[AXS_TOUCH_ONE_POINT_LEN * point_index + AXS_TOUCH_EVENT_POS] >> 6)


void wifi_test(void);
void SmartConfig();
void setTimezone();

static uint32_t last_tick;
struct tm       timeinfo;
uint32_t        cycleInterval = 0;

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area,
                   lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

#ifdef LCD_SPI_DMA
    char i = 0;
    while (get_lcd_spi_dma_write()) {
        i = i >> 1;
        lcd_PushColors(0, 0, 0, 0, NULL);
    }
#endif
    lcd_PushColors(area->x1, area->y1, w, h, (uint16_t *)&color_p->full);

#ifdef LCD_SPI_DMA

#else
    lv_disp_flush_ready(disp);
#endif
}

uint8_t read_touchpad_cmd[11] = {0xb5, 0xab, 0xa5, 0x5a, 0x0, 0x0, 0x0, 0x8};
void    my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    uint8_t buff[20] = {0};

    Wire.beginTransmission(0x3B);
    Wire.write(read_touchpad_cmd, 8);
    Wire.endTransmission();
    Wire.requestFrom(0x3B, 8);
    while (!Wire.available())
        ;
    Wire.readBytes(buff, 8);

    uint16_t pointX;
    uint16_t pointY;
    uint16_t type = 0;

    type   = AXS_GET_GESTURE_TYPE(buff);
    pointX = AXS_GET_POINT_X(buff, 0);
    pointY = AXS_GET_POINT_Y(buff, 0);

    if (!type && (pointX || pointY)) {
        pointX = (640 - pointX);
        if (pointX > 640)
            pointX = 640;
        if (pointY > 180)
            pointY = 180;
        data->state   = LV_INDEV_STATE_PR;
        data->point.x = pointY;
        data->point.y = pointX;

        String str_buf;
        str_buf += "x: " + String(pointY) + " y: " + String(pointX) + "\n";
        lv_msg_send(MSG_NEW_TOUCH_POINT, str_buf.c_str());
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
    xSemaphoreGive(xSemaphore);
}

bool result = false;
void setup()
{
    xSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xSemaphore);

    Serial.begin(115200);
    Serial.println("sta\n");

    pinMode(PIN_BAT_VOLT, ANALOG);

    pinMode(TOUCH_RES, OUTPUT);
    digitalWrite(TOUCH_RES, HIGH);
    delay(2);
    digitalWrite(TOUCH_RES, LOW);
    delay(10);
    digitalWrite(TOUCH_RES, HIGH);
    delay(2);

    Wire.begin(TOUCH_IICSDA, TOUCH_IICSCL);
    result =  PMU.init(Wire, TOUCH_IICSDA, TOUCH_IICSCL, SY6970_SLAVE_ADDRESS);
    if (result == false) {
        Serial.println("PMU is not online...");
        delay(50);
    } else {
        PMU.enableADCMeasure();
        PMU.disableStatLed();
        PMU.disableOTG();
    }

    // Only the SD card version has an SD card slot
    SPI.begin(SPI_SD_SCLK, SPI_SD_MISO, SPI_SD_MOSI);
    if (!SD.begin(SPI_SD_CS)) {
        Serial.println("Failed to detect SD Card!");
    }
    if (SD.cardType() != CARD_NONE) {
        Serial.printf("SD Card Size:");
        Serial.print(SD.cardSize() / (1024 * 1024));
        Serial.println(" MB");
    }


    configTime(GMT_OFFSET_SEC, DAY_LIGHT_OFFSET_SEC, NTP_SERVER1, NTP_SERVER2);

    axs15231_init();

    lv_init();
    size_t buffer_size =
        sizeof(lv_color_t) * EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES;
    buf = (lv_color_t *)ps_malloc(buffer_size);
    if (buf == NULL) {
        while (1) {
            Serial.println("buf NULL");
            delay(500);
        }
    }

    buf1 = (lv_color_t *)ps_malloc(buffer_size);
    if (buf1 == NULL) {
        while (1) {
            Serial.println("buf NULL");
            delay(500);
        }
    }

    lv_disp_draw_buf_init(&draw_buf, buf, buf1, buffer_size);
    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res      = EXAMPLE_LCD_H_RES;
    disp_drv.ver_res      = EXAMPLE_LCD_V_RES;
    disp_drv.flush_cb     = my_disp_flush;
    disp_drv.draw_buf     = &draw_buf;
    disp_drv.sw_rotate    = 1;          // If you turn on software rotation, Do not update or replace LVGL
    disp_drv.rotated      = LV_DISP_ROT_90;
    disp_drv.full_refresh = 1;          // full_refresh must be 1
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type    = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    setenv("TZ", "CST-8", 1);

    Serial.println("end\n");
}

extern uint32_t transfer_num;
extern size_t   lcd_PushColors_len;
void            loop()
{
    delay(1);
    if (transfer_num <= 0 && lcd_PushColors_len <= 0)
        lv_timer_handler();

    if (transfer_num <= 1 && lcd_PushColors_len > 0) {
        lcd_PushColors(0, 0, 0, 0, NULL);
    }

    if (millis() - last_tick > 100) {
        if (getLocalTime(&timeinfo, 2)) {
            lv_msg_send(MSG_NEW_HOUR, &timeinfo.tm_hour);
            lv_msg_send(MSG_NEW_MIN, &timeinfo.tm_min);
            lv_msg_send(MSG_NEW_SEC, &timeinfo.tm_sec);
        }
        last_tick = millis();
    }

    if (millis() > cycleInterval) {
        if (result != false) {
            xSemaphoreTake(xSemaphore, portMAX_DELAY);
            char buf_Batt[41] = {0};
            snprintf(buf_Batt, 40, "USB:%s, OTG:%s\nBattVol:%dmV\n",
                     (PMU.isVbusIn() ? "YES" : "NO"), (PMU.isEnableOTG() ? "YES" : "NO"), PMU.getBattVoltage());
            lv_msg_send(MSG_NEW_USB, buf_Batt);
            xSemaphoreGive(xSemaphore);
        }
        cycleInterval = millis() + 500;
    }

    static int           flag_bl = 0;
    static unsigned long cnt     = 0;

    cnt++;
    if (cnt >= 100) {
        if (flag_bl == 0) {
            pinMode(TFT_BL, OUTPUT);
            digitalWrite(TFT_BL, HIGH);
            flag_bl = 1;
            wifi_test();
            ui_begin();
        }
    }
}

LV_IMG_DECLARE(lilygo2_gif);
void lv_delay_ms(int x)
{
    do {
        uint32_t t = x;
        while (t--) {
            delay(1);
            if (transfer_num <= 0 && lcd_PushColors_len <= 0)
                lv_timer_handler();

            if (transfer_num <= 1 && lcd_PushColors_len > 0) {
                lcd_PushColors(0, 0, 0, 0, NULL);
            }
        }
    } while (0);
}

void wifi_test(void)
{
    String    text;
    lv_obj_t *boj = lv_obj_create(lv_scr_act());
    lv_obj_set_style_pad_all(boj, 0, 0);
    lv_obj_set_width(boj, LV_PCT(100));
    lv_obj_set_height(boj, LV_PCT(100));
    lv_obj_set_align(boj, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(boj, lv_color_hex(0x1874f0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_t *logo_img = lv_gif_create(boj);
    lv_obj_center(logo_img);
    lv_gif_set_src(logo_img, &lilygo2_gif);
    lv_delay_ms(400);
    lv_obj_del(boj);

    lv_obj_t *log_label = lv_label_create(lv_scr_act());
    lv_obj_align(log_label, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_width(log_label, LV_PCT(100));
    lv_label_set_long_mode(log_label, LV_LABEL_LONG_SCROLL);
    lv_label_set_recolor(log_label, true);

    lv_label_set_text(log_label, "Scan WiFi");
    lv_delay_ms(1);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    lv_delay_ms(100);
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        text = "no networks found";
    } else {
        text = n;
        text += " networks found\n";
        for (int i = 0; i < n; ++i) {
            text += (i + 1);
            text += ": ";
            text += WiFi.SSID(i);
            text += " (";
            text += WiFi.RSSI(i);
            text += ")";
            text += (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " \n" : "*\n";
            lv_delay_ms(10);
        }
    }

    wifi_config_t current_conf = {0};
    esp_wifi_get_config(WIFI_IF_STA, &current_conf);
    if (strlen((const char *)current_conf.sta.ssid) == 0) {
        Serial.println("Use default WiFi SSID & PASSWORD!!");
        memcpy((char *)(current_conf.sta.ssid), (const char *)WIFI_SSID, strlen(WIFI_SSID) + 1);
        memcpy((char *)(current_conf.sta.password), (const char *)WIFI_PASSWORD, strlen(WIFI_PASSWORD) + 1);
        WiFi.begin((char *)(current_conf.sta.ssid), (char *)(current_conf.sta.password));
    } else {
        Serial.println("Begin WiFi");
        WiFi.begin();
    }

    lv_label_set_text(log_label, text.c_str());
    Serial.println(text);
    lv_delay_ms(2000);
    text = "Connecting to ";
    Serial.print("Connecting to ");
    text += (char *)(current_conf.sta.ssid);
    text += "\n";
    Serial.print((char *)(current_conf.sta.ssid));

    uint32_t last_tick              = millis();
    bool     is_smartconfig_connect = false;
    lv_label_set_long_mode(log_label, LV_LABEL_LONG_WRAP);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        text += ".";
        lv_label_set_text(log_label, text.c_str());
        lv_delay_ms(100);
        if (millis() - last_tick > WIFI_CONNECT_WAIT_MAX) { /* Automatically start smartconfig when connection times out */
            text += "\nConnection timed out, start smartconfig";
            lv_label_set_text(log_label, text.c_str());
            lv_delay_ms(100);
            is_smartconfig_connect = true;
            WiFi.mode(WIFI_AP_STA);
            Serial.println("\r\n wait for smartconfig....");
            text += "\r\n wait for smartconfig....";
            text += "\nPlease use #ff0000 EspTouch# Apps to connect to the distribution network";
            lv_label_set_text(log_label, text.c_str());
            WiFi.beginSmartConfig();
            while (1) {
                lv_delay_ms(100);
                if (WiFi.smartConfigDone()) {
                    Serial.println("\r\nSmartConfig Success\r\n");
                    Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
                    Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
                    text += "\nSmartConfig Success";
                    text += "\nSSID:";
                    text += WiFi.SSID().c_str();
                    text += "\nPSW:";
                    text += WiFi.psk().c_str();
                    lv_label_set_text(log_label, text.c_str());
                    lv_delay_ms(1000);
                    last_tick = millis();
                    break;
                }
            }
        }
    }
    if (!is_smartconfig_connect) {
        text += "\nCONNECTED \nTakes ";
        Serial.print("\n CONNECTED \nTakes ");
        text += millis() - last_tick;
        Serial.print(millis() - last_tick);
        text += " ms\n";
        Serial.println(" millseconds");
        lv_label_set_text(log_label, text.c_str());
    }
    lv_delay_ms(500);
}


void boardSleep()
{
    // Sleep display
    lcd_sleep();

    Wire.end();

    pinMode(TOUCH_IICSCL, OPEN_DRAIN);
    pinMode(TOUCH_IICSDA, OPEN_DRAIN);
    pinMode(TOUCH_INT, OPEN_DRAIN);

    // Disbale Touch chip
    pinMode(TOUCH_RES, OUTPUT);
    digitalWrite(TOUCH_RES, LOW);
    gpio_hold_en(GPIO_NUM_16);
    gpio_deep_sleep_hold_en();

    esp_sleep_enable_ext1_wakeup(_BV(0), ESP_EXT1_WAKEUP_ALL_LOW);

    esp_deep_sleep_start();
}