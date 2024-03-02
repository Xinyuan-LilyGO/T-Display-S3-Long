#include "factory_gui.h"
#include "Arduino.h"
#include "SD_MMC.h"
#include "lvgl.h"
#include <WiFi.h>
#include <SD.h>

#define CLOCK_NORMAL 0
#define CLOCK_FLIP   1
#define CLOCK_DEMO   CLOCK_FLIP

LV_FONT_DECLARE(font_Alibaba);
LV_IMG_DECLARE(lilygo1_gif);
LV_IMG_DECLARE(gif_01);
LV_IMG_DECLARE(gif_12);
LV_IMG_DECLARE(gif_23);
LV_IMG_DECLARE(gif_34);
LV_IMG_DECLARE(gif_45);
LV_IMG_DECLARE(gif_56);
LV_IMG_DECLARE(gif_67);
LV_IMG_DECLARE(gif_78);
LV_IMG_DECLARE(gif_89);
LV_IMG_DECLARE(gif_90);

#if (CLOCK_DEMO == CLOCK_NORMAL)
static lv_point_t line_points[] = {
    {-600, 0},
    { 600, 0}
};
static void      update_text_subscriber_cb(lv_event_t *e);
#endif
static void      update_text_subscriber_cb_demo1(void *s, lv_msg_t *msg);
static void      update_touch_point_subscriber_cb(lv_event_t *e);
static void      otg_btn_subscriber_cb(lv_event_t *e);
static void      otg_btn_cd(lv_event_t *event);
static void sleep_btn_cd(lv_event_t *event);
static lv_obj_t *dis;

static lv_obj_t *img1;
static lv_obj_t *img2;
static lv_obj_t *img3;
static lv_obj_t *img4;
static lv_obj_t *img5;
static lv_obj_t *img6;
// Transition animation
static const void *trans_ainm_buf[] = {
    &gif_01, &gif_12, &gif_23, &gif_34, &gif_45,
    &gif_56, &gif_67, &gif_78, &gif_89, &gif_90
};

#define set_anim_src(x) ((const void *)trans_ainm_buf[(x)])
#define buf_limit(idx)  ((idx) % 10)
#define timer_h(v, n, obj)                                                       \
    do {                                                                         \
        if (v != n / 10) {                                                       \
            v = buf_limit(n / 10);                                               \
            if (v != 0)                                                          \
                lv_gif_set_src(obj, (const void *)trans_ainm_buf[v - 1]);        \
            else                                                                 \
                lv_gif_set_src(obj, (const void *)trans_ainm_buf[buf_limit(9)]); \
        }                                                                        \
    } while (0);

#define timer_l(v, n, obj)                                                       \
    do {                                                                         \
        if (v != n % 10) {                                                       \
            v = buf_limit(n % 10);                                               \
            if (v != 0)                                                          \
                lv_gif_set_src(obj, (const void *)trans_ainm_buf[v - 1]);        \
            else                                                                 \
                lv_gif_set_src(obj, (const void *)trans_ainm_buf[buf_limit(9)]); \
        }                                                                        \
    } while (0);

void set_flip_time_anim(int hour, int minute, int second)
{
    static int sec_h, sec_l, min_h, min_l, hou_h, hou_l;

    timer_l(sec_l, second, img6);
    timer_h(sec_h, second, img5);
    timer_l(min_l, minute, img4);
    timer_h(min_h, minute, img3);
    timer_l(hou_l, hour, img2);
    timer_h(hou_h, hour, img1);

    // printf("clock=%02d:%02d:%02d\n", hour, minute, second);
}

void ui_switch_page(void)
{
    static uint8_t n;
    n++;
    lv_obj_set_tile_id(dis, 0, n % UI_PAGE_COUNT, LV_ANIM_ON);
}

static volatile bool smartConfigStart      = false;
static lv_timer_t   *wifi_timer            = NULL;
static uint32_t      wifi_timer_counter    = 0;
static uint32_t      wifi_connnect_timeout = 60;

static void wifi_config_event_handler(lv_event_t *e)
{
    lv_event_code_t code  = lv_event_get_code(e);
    lv_obj_t       *btn   = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t       *label = (lv_obj_t *)lv_event_get_user_data(e);
    if (code == LV_EVENT_CLICKED) {
        if (smartConfigStart) {
            lv_label_set_text(label, "Config Start");
            if (wifi_timer) {
                lv_timer_del(wifi_timer);
                wifi_timer = NULL;
            }
            WiFi.stopSmartConfig();
            lv_obj_clear_state(btn, LV_STATE_CHECKED);
            Serial.println("return smart Config has Start;");
            smartConfigStart = false;
            return;
        }
        WiFi.disconnect();
        smartConfigStart = true;
        WiFi.beginSmartConfig();
        lv_label_set_text(label, "Config Stop");
        lv_obj_add_state(btn, LV_STATE_CHECKED);

        wifi_timer = lv_timer_create([](lv_timer_t *t) {
            lv_obj_t *btn     = (lv_obj_t *)t->user_data;
            lv_obj_t *label   = lv_obj_get_child(btn, 0);
            bool      destory = false;
            wifi_timer_counter++;
            if (wifi_timer_counter > wifi_connnect_timeout && !WiFi.isConnected()) {
                Serial.println("Connect timeout!");
                destory = true;
                lv_label_set_text(label, "Time Out");
            }
            if (WiFi.isConnected()) {
                Serial.println("WiFi has connected!");
                Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
                Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
                destory   = true;
                String IP = WiFi.localIP().toString();
                lv_label_set_text(label, IP.c_str());
            }
            if (destory) {
                WiFi.stopSmartConfig();
                smartConfigStart = false;
                lv_timer_del(wifi_timer);
                wifi_timer         = NULL;
                wifi_timer_counter = 0;
                lv_obj_clear_state(btn, LV_STATE_CHECKED);
            }
            // Every seconds check conected
        },
        1000, btn);
    }
}

void ui_begin()
{
    dis = lv_tileview_create(lv_scr_act());
    lv_obj_align(dis, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_size(dis, LV_PCT(100), LV_PCT(100));
    // lv_obj_remove_style(dis, 0, LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_color(dis, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *tv1 = lv_tileview_add_tile(dis, 0, 0, LV_DIR_VER);
    lv_obj_t *tv2 = lv_tileview_add_tile(dis, 0, 1, LV_DIR_VER);
    lv_obj_t *tv3 = lv_tileview_add_tile(dis, 0, 2, LV_DIR_VER);
    lv_obj_t *tv4 = lv_tileview_add_tile(dis, 0, 3, LV_DIR_VER);

#if (CLOCK_DEMO == CLOCK_FLIP)          // demo1
    img1 = lv_gif_create(tv1);
    img2 = lv_gif_create(tv1);
    img3 = lv_gif_create(tv1);
    img4 = lv_gif_create(tv1);
    img5 = lv_gif_create(tv1);
    img6 = lv_gif_create(tv1);

    lv_gif_set_src(img1, set_anim_src(9));
    lv_gif_set_src(img2, set_anim_src(9));
    lv_gif_set_src(img3, set_anim_src(9));
    lv_gif_set_src(img4, set_anim_src(9));
    lv_gif_set_src(img5, set_anim_src(9));
    lv_gif_set_src(img6, set_anim_src(9));

    lv_obj_align(img1, LV_ALIGN_LEFT_MID, 20, 0);
    lv_obj_align_to(img2, img1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_align(img3, LV_ALIGN_LEFT_MID, 225, 0);
    lv_obj_align_to(img4, img3, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
    lv_obj_align(img5, LV_ALIGN_LEFT_MID, 430, 0);
    lv_obj_align_to(img6, img5, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    lv_msg_subsribe(MSG_NEW_HOUR, update_text_subscriber_cb_demo1, NULL);
    lv_msg_subsribe(MSG_NEW_MIN, update_text_subscriber_cb_demo1, NULL);
    lv_msg_subsribe(MSG_NEW_SEC, update_text_subscriber_cb_demo1, NULL);
#else
    /* page 1 */
    lv_obj_t *main_cout = lv_obj_create(tv1);
    lv_obj_set_style_radius(main_cout, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_size(main_cout, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(main_cout, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(main_cout, 0, 0);
    lv_obj_set_style_bg_color(main_cout, UI_BG_COLOR, 0);

    lv_obj_t *hour_cout = lv_obj_create(main_cout);
    lv_obj_set_size(hour_cout, 140, 140);
    lv_obj_align(hour_cout, LV_ALIGN_LEFT_MID, 40, 0);
    lv_obj_set_style_bg_color(hour_cout, UI_FRAME_COLOR, 0);
    lv_obj_clear_flag(hour_cout, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *min_cout = lv_obj_create(main_cout);
    lv_obj_set_size(min_cout, 140, 140);
    lv_obj_align_to(min_cout, hour_cout, LV_ALIGN_OUT_RIGHT_MID, 50, 0);
    lv_obj_set_style_bg_color(min_cout, UI_FRAME_COLOR, 0);
    lv_obj_clear_flag(min_cout, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *sec_cout = lv_obj_create(main_cout);
    lv_obj_set_size(sec_cout, 140, 140);
    // lv_obj_align(sec_cout, LV_ALIGN_CENTER, 85, 0);
    lv_obj_align_to(sec_cout, min_cout, LV_ALIGN_OUT_RIGHT_MID, 50, 0);
    lv_obj_set_style_bg_color(sec_cout, UI_FRAME_COLOR, 0);
    lv_obj_clear_flag(sec_cout, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *seg_text = lv_label_create(main_cout);
    lv_obj_align(seg_text, LV_ALIGN_CENTER, -100, -12);
    lv_obj_set_style_text_font(seg_text, &font_Alibaba, 0);
    lv_label_set_text(seg_text, ":");
    lv_obj_set_style_text_color(seg_text, UI_FONT_COLOR, 0);

    lv_obj_t *seg_text1 = lv_label_create(main_cout);
    lv_obj_align(seg_text1, LV_ALIGN_CENTER, 90, -12);
    lv_obj_set_style_text_font(seg_text1, &font_Alibaba, 0);
    lv_label_set_text(seg_text1, ":");
    lv_obj_set_style_text_color(seg_text1, UI_FONT_COLOR, 0);

    lv_obj_t *hour_text = lv_label_create(hour_cout);
    lv_obj_center(hour_text);
    lv_obj_set_style_text_font(hour_text, &font_Alibaba, 0);
    lv_label_set_text(hour_text, "12");
    lv_obj_set_style_text_color(hour_text, UI_FONT_COLOR, 0);
    lv_obj_add_event_cb(hour_text, update_text_subscriber_cb, LV_EVENT_MSG_RECEIVED, NULL);
    lv_msg_subsribe_obj(MSG_NEW_HOUR, hour_text, (void *)"%02d");

    lv_obj_t *min_text = lv_label_create(min_cout);
    lv_obj_center(min_text);
    lv_obj_set_style_text_font(min_text, &font_Alibaba, 0);
    lv_label_set_text(min_text, "34");
    lv_obj_set_style_text_color(min_text, UI_FONT_COLOR, 0);
    lv_obj_add_event_cb(min_text, update_text_subscriber_cb, LV_EVENT_MSG_RECEIVED, NULL);
    lv_msg_subsribe_obj(MSG_NEW_MIN, min_text, (void *)"%02d");

    lv_obj_t *sec_text = lv_label_create(sec_cout);
    lv_obj_center(sec_text);
    lv_obj_set_style_text_font(sec_text, &font_Alibaba, 0);
    lv_label_set_text(sec_text, "56");
    lv_obj_set_style_text_color(sec_text, UI_FONT_COLOR, 0);
    lv_obj_add_event_cb(sec_text, update_text_subscriber_cb, LV_EVENT_MSG_RECEIVED, NULL);
    lv_msg_subsribe_obj(MSG_NEW_SEC, sec_text, (void *)"%02d");

    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 4);
    lv_style_set_line_color(&style_line, UI_BG_COLOR);
    lv_style_set_line_rounded(&style_line, true);

    lv_obj_t *line;
    line = lv_line_create(main_cout);
    lv_line_set_points(line, line_points, 2);
    lv_obj_add_style(line, &style_line, 0);
    lv_obj_center(line);
#endif

    /* page 2 */
    lv_obj_t *log_gif = lv_gif_create(tv2);
    lv_gif_set_src(log_gif, &lilygo1_gif);
    lv_obj_center(log_gif);

    /* page 3 */
    lv_obj_t       *debug_label = lv_label_create(tv3);
    String          text;
    esp_chip_info_t t;
    esp_chip_info(&t);
    text = "Chip : ";
    text += ESP.getChipModel();
    text += "\n";
    text += "PSRAM Size : ";
    text += ESP.getPsramSize() / 1024;
    text += " KB\n";
    text += "Flash Size : ";
    text += ESP.getFlashChipSize() / 1024;
    text += " KB\n";
    text += "SD Size:";
    text += String((float)(SD.cardSize() / 1024.0 / 1024.0 / 1024.0)).c_str();
    text += " GB";

    lv_label_set_text(debug_label, text.c_str());
    lv_obj_align(debug_label, LV_ALIGN_CENTER, 0, -35);

    lv_obj_t *getBattVoltage_text = lv_label_create(tv3);
    lv_obj_align_to(getBattVoltage_text, debug_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_set_style_text_color(getBattVoltage_text, lv_color_hex(0xFFFFFF),
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(getBattVoltage_text, update_touch_point_subscriber_cb, LV_EVENT_MSG_RECEIVED, NULL);
    lv_label_set_text(getBattVoltage_text, "USB:ON, OTG:ON\nBattVol:0mV\n");
    lv_msg_subsribe_obj(MSG_NEW_USB, getBattVoltage_text, (void *)"%s");

    lv_obj_t *touch_label = lv_label_create(tv3);
    lv_obj_align_to(touch_label, getBattVoltage_text, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_add_event_cb(touch_label, update_touch_point_subscriber_cb, LV_EVENT_MSG_RECEIVED, NULL);
    lv_label_set_text_fmt(touch_label, " ");
    lv_msg_subsribe_obj(MSG_NEW_TOUCH_POINT, touch_label, (void *)"%s");

    lv_obj_t *otg_btn = lv_btn_create(tv3);
    lv_obj_add_event_cb(otg_btn, otg_btn_cd, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_set_size(otg_btn, 150, 50);
    lv_obj_align(otg_btn, LV_ALIGN_CENTER, -190, -20);
    lv_obj_set_style_bg_color(otg_btn, UI_FRAME_COLOR, 0);
    lv_obj_t *otg_btn_label = lv_label_create(otg_btn);
    lv_obj_align(otg_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_color(otg_btn_label, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(otg_btn_label, otg_btn_subscriber_cb, LV_EVENT_MSG_RECEIVED, NULL);
    lv_label_set_text_fmt(otg_btn_label, "OTG Open");
    lv_msg_subsribe_obj(MSG_NEW_OTG_BTN, otg_btn_label, (void *)"%s");


    lv_obj_t *sleep_btn = lv_btn_create(tv3);
    lv_obj_add_event_cb(sleep_btn, sleep_btn_cd, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_set_size(sleep_btn, 150, 50);
    lv_obj_align_to(sleep_btn, otg_btn, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_bg_color(sleep_btn, UI_FRAME_COLOR, 0);

    lv_obj_t *sleep_btn_label = lv_label_create(sleep_btn);
    lv_obj_align(sleep_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_color(sleep_btn_label, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text_fmt(sleep_btn_label, "Sleep");

    /* page 4 */
    String txt = "Use ESP Touch App Configure your network\n";
    txt += "1. Install ESPTouch App\n";
    txt += "2. Turn on ESPTouch -> Click EspTouch\n";
    txt += "3. Enter Your WiFi Password,\n\tOther setting use default\n";
    txt += "4. Confirm\n";
    txt += "5. Click Config WiFi Button\n";
    txt += "6. Wait config done\n";

    lv_obj_t *label, *tips_label;
    tips_label = lv_label_create(tv4);
    lv_obj_set_width(tips_label, LV_PCT(100));
    lv_label_set_long_mode(tips_label, LV_LABEL_LONG_SCROLL);
    lv_obj_set_style_text_color(tips_label, lv_color_white(), LV_PART_MAIN);
    lv_label_set_text(tips_label, txt.c_str());
    lv_obj_align(tips_label, LV_ALIGN_TOP_MID, 0, 20);

    const char *android_url = "https://github.com/EspressifApp/EsptouchForAndroid/releases/tag/v2.0.0/esptouch-v2.0.0.apk";
    const char *ios_url     = "https://apps.apple.com/cn/app/espressif-esptouch/id1071176700";

    lv_color_t bg_color = lv_palette_lighten(LV_PALETTE_GREEN, 5);
    lv_color_t fg_color = lv_palette_darken(LV_PALETTE_NONE, 4);

    lv_coord_t size            = 120;
    lv_obj_t  *android_rq_code = lv_qrcode_create(tv4, size, fg_color, bg_color);
    lv_qrcode_update(android_rq_code, android_url, strlen(android_url));
    lv_obj_set_pos(android_rq_code, 340, 10);
    // lv_obj_align_to(android_rq_code, tips_label, LV_ALIGN_OUT_BOTTOM_LEFT, 30, 5);
    /*Add a border with bg_color*/
    lv_obj_set_style_border_color(android_rq_code, bg_color, 0);
    lv_obj_set_style_border_width(android_rq_code, 5, 0);
    label = lv_label_create(tv4);
    lv_label_set_text(label, "Android");
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_align_to(label, android_rq_code, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t *ios_rq_code = lv_qrcode_create(tv4, size, fg_color, bg_color);
    lv_qrcode_update(ios_rq_code, ios_url, strlen(ios_url));
    lv_obj_align_to(ios_rq_code, android_rq_code, LV_ALIGN_OUT_RIGHT_MID, 20, 0);

    /*Add a border with bg_color*/
    lv_obj_set_style_border_color(ios_rq_code, bg_color, 0);
    lv_obj_set_style_border_width(ios_rq_code, 5, 0);
    label = lv_label_create(tv4);
    lv_label_set_text(label, "IOS");
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_align_to(label, ios_rq_code, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    // BTN
    lv_obj_t *btn = lv_btn_create(tv4);
    label         = lv_label_create(btn);
    lv_label_set_text(label, "Config WiFi");
    lv_obj_set_width(btn, 120);
    // lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_obj_align_to(label, btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(btn, wifi_config_event_handler, LV_EVENT_CLICKED, label);
    lv_obj_set_pos(btn, 205, 120);
}

#include <XPowersLib.h>
extern PowersSY6970 PMU;
static void otg_btn_cd(lv_event_t *event)
{
    static int btn_state = 1;

    if (btn_state) {
        btn_state = 0;

        if (PMU.enableOTG()) {
            lv_msg_send(MSG_NEW_OTG_BTN, "OTG Close");
        } else {
            lv_msg_send(MSG_NEW_OTG_BTN, "USB is not removed");
        }
    } else {
        btn_state = 1;
        PMU.disableOTG();
        lv_msg_send(MSG_NEW_OTG_BTN, "OTG Open");
    }
}
extern void boardSleep();
static void sleep_btn_cd(lv_event_t *event)
{
    boardSleep();
}



#if (CLOCK_DEMO == CLOCK_FLIP)
static void update_text_subscriber_cb_demo1(void *s, lv_msg_t *msg)
{
    static int      hour       = 0;
    static int      minute     = 0;
    static int      second     = 0;
    static uint32_t start_tick = lv_tick_get();
    const int32_t  *v          = (const int32_t *)lv_msg_get_payload(msg);

    /* clang-format off */
    switch (lv_msg_get_id(msg)) {
    case MSG_NEW_HOUR: hour = *v;   break;
    case MSG_NEW_MIN:  minute = *v; break;
    case MSG_NEW_SEC:  second = *v; break;
    default:
        break;
    } /* clang-format off */

    if (lv_tick_elaps(start_tick) >= 1000) {
        start_tick = lv_tick_get();
        set_flip_time_anim(hour, minute, second);
    }
}
#elif
static void update_text_subscriber_cb(lv_event_t *e)
{
    lv_obj_t *label = lv_event_get_target(e);
    lv_msg_t *m = lv_event_get_msg(e);

    const char *fmt = (const char *)lv_msg_get_user_data(m);
    const int32_t *v = (const int32_t *)lv_msg_get_payload(m);

    lv_label_set_text_fmt(label, fmt, *v);
}
#endif

static void update_touch_point_subscriber_cb(lv_event_t *e)
{
    lv_obj_t *label = lv_event_get_target(e);
    lv_msg_t *m     = lv_event_get_msg(e);

    const char *fmt = (const char *)lv_msg_get_user_data(m);
    const char *t   = (const char *)lv_msg_get_payload(m);

    lv_label_set_text_fmt(label, fmt, t);
}

static void otg_btn_subscriber_cb(lv_event_t *e)
{
    lv_obj_t *label = lv_event_get_target(e);
    lv_msg_t *m     = lv_event_get_msg(e);

    const char *fmt = (const char *)lv_msg_get_user_data(m);
    const char *t   = (const char *)lv_msg_get_payload(m);

    lv_label_set_text_fmt(label, fmt, t);
}
