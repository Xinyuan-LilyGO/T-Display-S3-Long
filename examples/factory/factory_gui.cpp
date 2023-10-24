#include "factory_gui.h"
#include "Arduino.h"
#include "SD_MMC.h"
#include "lvgl.h"

#define CLOCK_NORMAL 0
#define CLOCK_FLIP   1
#define CLOCK_DEMO CLOCK_FLIP

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

static lv_point_t line_points[] = {{-600, 0}, {600, 0}};

static void update_text_subscriber_cb(lv_event_t *e);
static void update_text_subscriber_cb_demo1(void *s, lv_msg_t *msg);
static void update_touch_point_subscriber_cb(lv_event_t *e);
static void otg_btn_subscriber_cb(lv_event_t* e);
static void otg_btn_cd(lv_event_t* event);
static lv_obj_t *dis;

static lv_obj_t *img1;
static lv_obj_t *img2;
static lv_obj_t *img3;
static lv_obj_t *img4;
static lv_obj_t *img5;
static lv_obj_t *img6;
// Transition animation
static const void * trans_ainm_buf[] = {
    &gif_01, &gif_12, &gif_23, &gif_34, &gif_45,
    &gif_56, &gif_67, &gif_78, &gif_89, &gif_90
};

#define set_anim_src(x) ((const void*)trans_ainm_buf[(x)])
#define buf_limit(idx) ((idx)%10)
#define timer_h(v, n, obj)                                                          \
        do{                                                                         \
            if(v != n / 10) {                                                       \
                v = buf_limit(n / 10);                                              \
                if(v != 0)                                                          \
                    lv_gif_set_src(obj, (const void *)trans_ainm_buf[v-1]);         \
                else                                                                \
                    lv_gif_set_src(obj, (const void *)trans_ainm_buf[buf_limit(9)]);\
            }                                                                       \
        }while (0);

#define timer_l(v, n, obj)                                                          \
        do{                                                                         \
            if(v != n % 10) {                                                       \
                v = buf_limit(n % 10);                                              \
                if(v != 0)                                                          \
                    lv_gif_set_src(obj, (const void *)trans_ainm_buf[v-1]);         \
                else                                                                \
                    lv_gif_set_src(obj, (const void *)trans_ainm_buf[buf_limit(9)]);\
            }                                                                       \
        }while (0);

void set_flip_time_anim(int hour, int minute, int second)
{
    static int sec_h,sec_l,min_h,min_l, hou_h,hou_l;

    timer_l(sec_l, second, img6);
    timer_h(sec_h, second, img5);
    timer_l(min_l, minute, img4);
    timer_h(min_h, minute, img3);
    timer_l(hou_l, hour, img2);
    timer_h(hou_h, hour, img1);

    // printf("clock=%02d:%02d:%02d\n", hour, minute, second);
}

void ui_switch_page(void) {
    static uint8_t n;
    n++;
    lv_obj_set_tile_id(dis, 0, n % UI_PAGE_COUNT, LV_ANIM_ON);
}

void ui_begin() {

    dis = lv_tileview_create(lv_scr_act());
    lv_obj_align(dis, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_size(dis, LV_PCT(100), LV_PCT(100));
    //lv_obj_remove_style(dis, 0, LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_color(dis, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* tv1 = lv_tileview_add_tile(dis, 0, 0, LV_DIR_VER);
    lv_obj_t* tv2 = lv_tileview_add_tile(dis, 0, 1, LV_DIR_VER);
    lv_obj_t* tv3 = lv_tileview_add_tile(dis, 0, 2, LV_DIR_VER);

#if (CLOCK_DEMO ==  CLOCK_FLIP)  // demo1
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
    lv_msg_subsribe(MSG_NEW_MIN,  update_text_subscriber_cb_demo1, NULL);
    lv_msg_subsribe(MSG_NEW_SEC,  update_text_subscriber_cb_demo1, NULL);
#else
    /* page 1 */
    lv_obj_t* main_cout = lv_obj_create(tv1);
    lv_obj_set_style_radius(main_cout, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_size(main_cout, LV_PCT(100), LV_PCT(100));
    lv_obj_clear_flag(main_cout, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(main_cout, 0, 0);
    lv_obj_set_style_bg_color(main_cout, UI_BG_COLOR, 0);

    lv_obj_t* hour_cout = lv_obj_create(main_cout);
    lv_obj_set_size(hour_cout, 140, 140);
    lv_obj_align(hour_cout, LV_ALIGN_LEFT_MID, 40, 0);
    lv_obj_set_style_bg_color(hour_cout, UI_FRAME_COLOR, 0);
    lv_obj_clear_flag(hour_cout, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* min_cout = lv_obj_create(main_cout);
    lv_obj_set_size(min_cout, 140, 140);
    lv_obj_align_to(min_cout, hour_cout, LV_ALIGN_OUT_RIGHT_MID, 50, 0);
    lv_obj_set_style_bg_color(min_cout, UI_FRAME_COLOR, 0);
    lv_obj_clear_flag(min_cout, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* sec_cout = lv_obj_create(main_cout);
    lv_obj_set_size(sec_cout, 140, 140);
    //lv_obj_align(sec_cout, LV_ALIGN_CENTER, 85, 0);
    lv_obj_align_to(sec_cout, min_cout, LV_ALIGN_OUT_RIGHT_MID, 50, 0);
    lv_obj_set_style_bg_color(sec_cout, UI_FRAME_COLOR, 0);
    lv_obj_clear_flag(sec_cout, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t* seg_text = lv_label_create(main_cout);
    lv_obj_align(seg_text, LV_ALIGN_CENTER, -100, -12);
    lv_obj_set_style_text_font(seg_text, &font_Alibaba, 0);
    lv_label_set_text(seg_text, ":");
    lv_obj_set_style_text_color(seg_text, UI_FONT_COLOR, 0);

    lv_obj_t* seg_text1 = lv_label_create(main_cout);
    lv_obj_align(seg_text1, LV_ALIGN_CENTER, 90, -12);
    lv_obj_set_style_text_font(seg_text1, &font_Alibaba, 0);
    lv_label_set_text(seg_text1, ":");
    lv_obj_set_style_text_color(seg_text1, UI_FONT_COLOR, 0);

    lv_obj_t* hour_text = lv_label_create(hour_cout);
    lv_obj_center(hour_text);
    lv_obj_set_style_text_font(hour_text, &font_Alibaba, 0);
    lv_label_set_text(hour_text, "12");
    lv_obj_set_style_text_color(hour_text, UI_FONT_COLOR, 0);
    lv_obj_add_event_cb(hour_text, update_text_subscriber_cb, LV_EVENT_MSG_RECEIVED, NULL);
    lv_msg_subsribe_obj(MSG_NEW_HOUR, hour_text, (void*)"%02d");

    lv_obj_t* min_text = lv_label_create(min_cout);
    lv_obj_center(min_text);
    lv_obj_set_style_text_font(min_text, &font_Alibaba, 0);
    lv_label_set_text(min_text, "34");
    lv_obj_set_style_text_color(min_text, UI_FONT_COLOR, 0);
    lv_obj_add_event_cb(min_text, update_text_subscriber_cb, LV_EVENT_MSG_RECEIVED, NULL);
    lv_msg_subsribe_obj(MSG_NEW_MIN, min_text, (void*)"%02d");

    lv_obj_t* sec_text = lv_label_create(sec_cout);
    lv_obj_center(sec_text);
    lv_obj_set_style_text_font(sec_text, &font_Alibaba, 0);
    lv_label_set_text(sec_text, "56");
    lv_obj_set_style_text_color(sec_text, UI_FONT_COLOR, 0);
    lv_obj_add_event_cb(sec_text, update_text_subscriber_cb, LV_EVENT_MSG_RECEIVED, NULL);
    lv_msg_subsribe_obj(MSG_NEW_SEC, sec_text, (void*)"%02d");

    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 4);
    lv_style_set_line_color(&style_line, UI_BG_COLOR);
    lv_style_set_line_rounded(&style_line, true);

    lv_obj_t* line;
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
    lv_obj_t *debug_label = lv_label_create(tv3);
    String text;
    esp_chip_info_t t;
    esp_chip_info(&t);
    text = "chip : ";
    text += ESP.getChipModel();
    text += "\n";
    text += "psram size : ";
    text += ESP.getPsramSize() / 1024;
    text += " KB\n";
    text += "flash size : ";
    text += ESP.getFlashChipSize() / 1024;
    text += " KB\n";

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

    lv_obj_t* otg_btn = lv_btn_create(tv3);
    lv_obj_add_event_cb(otg_btn, otg_btn_cd, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_set_size(otg_btn, 100, 50);
    lv_obj_align(otg_btn, LV_ALIGN_CENTER, -190, 0);
    lv_obj_set_style_bg_color(otg_btn, UI_FRAME_COLOR, 0);
    lv_obj_set_style_pad_all(otg_btn, 0, 0);
    lv_obj_t* otg_btn_label = lv_label_create(otg_btn);
    lv_obj_align(otg_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_color(otg_btn_label, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(otg_btn_label, otg_btn_subscriber_cb, LV_EVENT_MSG_RECEIVED, NULL);
    lv_label_set_text_fmt(otg_btn_label, "OTG Open");
    lv_msg_subsribe_obj(MSG_NEW_OTG_BTN, otg_btn_label, (void*)"%s");
}

#include <XPowersLib.h>
extern PowersSY6970 PMU;
static void otg_btn_cd(lv_event_t* event)
{
    static int btn_state = 1;

    if (btn_state)
    {
        btn_state = 0;
        lv_msg_send(MSG_NEW_OTG_BTN, "OTG Close");
        //digitalWrite(45, 1);
        PMU.enableOTG();
    }
    else
    {
        btn_state = 1;
        PMU.disableOTG();
        //digitalWrite(45, 0);
        lv_msg_send(MSG_NEW_OTG_BTN, "OTG Open");
    }
}

#if (CLOCK_DEMO ==  CLOCK_FLIP)
static void update_text_subscriber_cb_demo1(void *s, lv_msg_t *msg)
{
    static int hour = 0;
    static int minute = 0;
    static int second = 0;
    static uint32_t start_tick = lv_tick_get();
    const int32_t *v = (const int32_t *)lv_msg_get_payload(msg);

    switch (lv_msg_get_id(msg))
    {
        case MSG_NEW_HOUR: hour = *v; break;
        case MSG_NEW_MIN:  minute = *v; break;
        case MSG_NEW_SEC:  second = *v; break;
        default:
            break;
    }

    if(lv_tick_elaps(start_tick) >= 1000){
        start_tick = lv_tick_get();
        set_flip_time_anim(hour, minute, second);
    }
}
#elif
static void update_text_subscriber_cb(lv_event_t *e) {
    lv_obj_t *label = lv_event_get_target(e);
    lv_msg_t *m = lv_event_get_msg(e);

    const char *fmt = (const char *)lv_msg_get_user_data(m);
    const int32_t *v = (const int32_t *)lv_msg_get_payload(m);

    lv_label_set_text_fmt(label, fmt, *v);

}
#endif

static void update_touch_point_subscriber_cb(lv_event_t *e) {
    lv_obj_t *label = lv_event_get_target(e);
    lv_msg_t *m = lv_event_get_msg(e);

    const char *fmt = (const char *)lv_msg_get_user_data(m);
    const char *t = (const char *)lv_msg_get_payload(m);

    lv_label_set_text_fmt(label, fmt, t);
}

static void otg_btn_subscriber_cb(lv_event_t* e) {
    lv_obj_t* label = lv_event_get_target(e);
    lv_msg_t* m = lv_event_get_msg(e);

    const char* fmt = (const char*)lv_msg_get_user_data(m);
    const char* t = (const char*)lv_msg_get_payload(m);

    lv_label_set_text_fmt(label, fmt, t);
}


