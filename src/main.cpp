#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
// #include "lv_demos.h"
TFT_eSPI tft = TFT_eSPI();   
#define led 2


static const uint16_t screenWidth  = 240;
static const uint16_t screenHeight = 240;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];
#define CHART_POINTS_NUM 256
extern void lv_demo_music(void);




void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

void setup() {
  // put your setup code here, to run once:
    // pinMode(led, OUTPUT);
    Serial.begin(9600);
    // Wire.begin();
    lv_init();
    tft.begin();
    tft.fillScreen(TFT_RED);
    tft.setRotation(1);  
   

    /*Initialize the display*/
    lv_disp_draw_buf_t draw_buf_dsc_1;
    lv_color_t buf_1[240 * 10];  /*A buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, 240 * 10);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 240;
    disp_drv.ver_res = 240;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf_dsc_1;
    lv_disp_drv_register(&disp_drv);

// lv_obj_t *label = lv_label_create( lv_scr_act() );
//     lv_label_set_text( label, "hello lvgl" );
//     lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
    // lv_demo_music();
}

void loop() {
  // put your main code here, to run repeatedly:
   lv_task_handler(); /* let the GUI do its work */
  //  delay(5);
}