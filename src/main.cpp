#include <Arduino.h>
#include <TFT_eSPI.h>
#include "lv_demos.h"
#include <lvgl.h>
#define MY_DISP_HOR_RES   240
#define MY_DISP_VER_RES   240

/**********************
 *  STATIC VARIABLES
 **********************/

TFT_eSPI tft = TFT_eSPI();   
/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    // tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    // tft.endWrite();

    lv_disp_flush_ready(disp_drv);
}
void setup() {
  // put your setup code here, to run once:
  lv_init();

    /* Init ePaper Screen */

    /* Init Paint */
    tft.begin();
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(0);  

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/
    static lv_disp_draw_buf_t draw_buf_dsc_1;
    static lv_color_t buf_1[MY_DISP_HOR_RES * 10];  /*A buffer for 10 rows*/
    static lv_color_t buf_2[MY_DISP_HOR_RES * 10];  /*B buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, buf_2, MY_DISP_HOR_RES * 10);   /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/
    static lv_disp_drv_t disp_drv;                  /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_1;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);

    /*demo test*/
    lv_demo_music();
    // lv_demo_flex_layout();
}

void loop() {
  // put your main code here, to run repeatedly:
   lv_task_handler();
}