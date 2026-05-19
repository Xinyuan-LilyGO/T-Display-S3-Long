#include "lvgl.h" /* https://github.com/lvgl/lvgl.git */
#include "AXS15231B.h"
#include <Arduino.h>
#include <Wire.h>
#include "hyn_core.h"

//================================
// If you turn on software rotation(disp_drv.sw_rotate = 1), Do not update or replace LVGL.
// disp_drv.full_refresh must be 1
//================================

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf;
static lv_color_t *buf1;
struct hyn_ts_data *ts_data;

lv_obj_t *ui_cartext = NULL;

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area,
                   lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

#ifdef LCD_SPI_DMA
  char i = 0;
  while (get_lcd_spi_dma_write())
  {
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

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  ts_data = get_touch_data();

  if (ts_data->rp_buf.rep_num > 0)
  {
    data->state = LV_INDEV_STATE_PR;
    data->point.x =  ts_data->rp_buf.pos_info[0].pos_x;
    data->point.y =  ts_data->rp_buf.pos_info[0].pos_y;

    char buf[20] = {0};
    sprintf(buf, "(%d, %d)", data->point.x, data->point.y);
    if (ui_cartext != NULL)
      lv_label_set_text(ui_cartext, buf);
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
}

LV_IMG_DECLARE(test_img);
LV_IMG_DECLARE(test1_180640);
LV_IMG_DECLARE(test3_180640);
LV_IMG_DECLARE(test4_180640);
static const lv_img_dsc_t *imgs[4] = {&test1_180640, &test3_180640,
                                      &test4_180640, &test_img};

void demo_init(lv_obj_t *parent, lv_img_dsc_t **dsc, uint8_t num,
               uint32_t duration, lv_coord_t x, lv_coord_t y, lv_coord_t w,
               lv_coord_t h)
{
  lv_obj_t *animimg0 = lv_animimg_create(parent);
  lv_obj_center(animimg0);
  lv_animimg_set_src(animimg0, dsc, num);

  lv_animimg_set_duration(animimg0, duration);
  lv_animimg_set_repeat_count(animimg0, 0xffff);

  lv_obj_set_style_pad_all(animimg0, 0, 0);

  lv_obj_set_size(animimg0, w, h);
  lv_animimg_start(animimg0);
}

void demo_animation(void)
{
  lv_obj_t *obj = lv_btn_create(lv_scr_act());
  lv_obj_set_style_pad_all(obj, 0, 0);
  lv_obj_set_pos(obj, 0, 0);
  lv_obj_set_size(obj, 180, 640);
  lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN);
  demo_init(obj, (lv_img_dsc_t **)imgs, 4, 12000, 0, 0, 180, 640);
}

bool result = false;
void setup()
{
  Serial.begin(115200);
  Serial.println("sta\n");


  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  axs15231_init();
  touch_init(TP_I2C_SDA, TP_I2C_SCL, TP_IRQ, TP_RST);

  lv_init();
  size_t buffer_size =
      sizeof(lv_color_t) * EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES;
  buf = (lv_color_t *)ps_malloc(buffer_size);
  if (buf == NULL)
  {
    while (1)
    {
      Serial.println("buf NULL");
      delay(500);
    }
  }

  buf1 = (lv_color_t *)ps_malloc(buffer_size);
  if (buf1 == NULL)
  {
    while (1)
    {
      Serial.println("buf NULL");
      delay(500);
    }
  }

  lv_disp_draw_buf_init(&draw_buf, buf, buf1, buffer_size);
  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = EXAMPLE_LCD_H_RES;
  disp_drv.ver_res = EXAMPLE_LCD_V_RES;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  // disp_drv.sw_rotate = 1;             //If you turn on software rotation, Do not update or replace LVGL
  // disp_drv.rotated = LV_DISP_ROT_90;
  disp_drv.full_refresh = 1; // full_refresh must be 1
  lv_disp_drv_register(&disp_drv);

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  ui_cartext = lv_label_create(lv_layer_top());
  lv_obj_align(ui_cartext, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_text_color(ui_cartext, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text(ui_cartext, "(0, 0)");

  demo_animation();

  Serial.println("end\n");
}

extern uint32_t transfer_num;
extern size_t lcd_PushColors_len;
void loop()
{
  delay(1);
  if (transfer_num <= 0 && lcd_PushColors_len <= 0)
    lv_timer_handler();

  if (transfer_num <= 1 && lcd_PushColors_len > 0)
  {
    lcd_PushColors(0, 0, 0, 0, NULL);
  }
}
