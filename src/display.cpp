/**
 * @file display.cpp
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "display.h"
#include <stdbool.h>

/*********************
 *      DEFINES
 *********************/

#define MY_DISP_HOR_RES   240
#define MY_DISP_VER_RES   240

TFT_eSPI tft = TFT_eSPI();   

static lv_disp_buf_t draw_buf_dsc_1;
static lv_color_t draw_buf_1[LV_HOR_RES_MAX * 10];                          /*A buffer for 10 rows*/
static lv_color_t draw_buf_2[LV_HOR_RES_MAX * 10];                          /*A buffer for 10 rows*/


/**********************
 *  STATIC PROTOTYPES
 **********************/

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

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp_drv);
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void Display::init(void)
{
    /* Init lvgl */
	lv_init();

    /* Init ePaper Screen */

    /* Init Paint */
    tft.begin();
    tft.setRotation(0);  

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/
 

    lv_disp_buf_init(&draw_buf_dsc_1, draw_buf_1, draw_buf_2, LV_HOR_RES_MAX * 10);   /*Initialize the display buffer*/

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.buffer = &draw_buf_dsc_1;
    lv_disp_drv_register(&disp_drv);

}

void Display::update(void)
{
    lv_task_handler();
}


/**
 * @brief flip the black and white color of epaper
 * 
 */
