#pragma once

#define UI_BG_COLOR    lv_color_black()
#define UI_FRAME_COLOR lv_color_hex(0x282828)
#define UI_FONT_COLOR  lv_color_white()
#define UI_PAGE_COUNT  3

#define MSG_NEW_HOUR   1
#define MSG_NEW_MIN    2
#define MSG_NEW_VOLT   3
#define MSG_NEW_TOUCH_POINT   4
#define MSG_NEW_SEC    5
#define MSG_NEW_USB    6
#define MSG_NEW_OTG_BTN   7

void ui_begin();
void ui_switch_page(void);