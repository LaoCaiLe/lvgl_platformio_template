#include <Arduino.h>
#include <Wire.h>
#include <lvgl.h>
#include <TFT_eSPI.h>


#define led 2

// AS_5600 ams5600;
TFT_eSPI tft = TFT_eSPI();   

static lv_disp_buf_t draw_buf_dsc_1;
static lv_color_t draw_buf_1[LV_HOR_RES_MAX * 10];                          /*A buffer for 10 rows*/

void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}


/**
 * Start animation on an event
 */

void lv_symbol_show()
{
	lv_obj_t * my_label = lv_label_create(lv_scr_act(), NULL);
	
	lv_label_set_text(my_label, LV_SYMBOL_OK "ok");
	// lv_obj_set_size(my_label,160,160);	
	lv_obj_align(my_label,NULL, LV_ALIGN_CENTER, 0 ,0);//、
	// lv_obj_set_opa_scale_enable(my_label,ENABLE);   //开启透明度
	// lv_obj_set_opa_scale(my_label,75);              //设置透明度50%

	lv_obj_t * my_label2 = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(my_label2, LV_SYMBOL_MUTE "Mute");
	lv_obj_align(my_label2,my_label,LV_ALIGN_IN_BOTTOM_MID,0,30);

}
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

static void ArcTaskCb(lv_task_t *t)
{
	static int16_t a = 0;

	//1. 每20ms任务调度时，此a+3；
	a += 3;
	if(a > 360)a = 360;

	//2. 给user_data这个传递过来的圆弧对象重绘制0度->a度的弧度	
  	lv_arc_set_bg_angles((lv_obj_t*)t->user_data,0,a);
	lv_arc_set_angles((lv_obj_t*)t->user_data, 0 ,a);
	if(a == 360)   
	{
		a=0;		
		//删除任务
		//lv_task_del(t);
	}
}

void lv_circle_show(void)
{	
	/*前景style*/
	static lv_style_t style_fg;
	lv_style_reset(&style_fg);
	lv_style_init(&style_fg);
    lv_style_set_size(&style_fg, LV_STATE_DEFAULT, 240);
    lv_style_set_line_color(&style_fg, LV_STATE_DEFAULT, LV_COLOR_RED);	//设置前景线条颜色为橙色
	lv_style_set_line_width(&style_fg, LV_STATE_DEFAULT, 9);				//设置前景线条宽度为9
	lv_style_set_line_rounded(&style_fg, LV_STATE_DEFAULT,0);

	/*背景style*/
	static lv_style_t style_bg;
	lv_style_reset(&style_bg);
	lv_style_init(&style_bg);
	lv_style_set_bg_color(&style_bg, LV_STATE_DEFAULT, LV_COLOR_BLACK);		//设置背景透明
	lv_style_set_border_opa(&style_bg, LV_STATE_DEFAULT, LV_OPA_TRANSP);		//设置背景边界线透明
	lv_style_set_line_opa(&style_bg, LV_STATE_DEFAULT, LV_OPA_TRANSP);		//设置背景线条为透明
	lv_style_set_line_width(&style_bg, LV_STATE_DEFAULT, 5);				//设置背景线条宽度为9
	
	/*画圆初始化*/
	lv_obj_t *arc = lv_arc_create(lv_scr_act(),NULL);
	lv_obj_set_style_local_border_opa(arc,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,LV_OPA_TRANSP);
	lv_obj_set_style_local_bg_color(arc,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_BLACK);
	/*将style添加到arc中*/
    lv_obj_set_size(arc,240,240);
	lv_obj_add_style(arc,LV_ARC_PART_BG, &style_bg);
	lv_obj_add_style(arc,LV_ARC_PART_INDIC, &style_fg); 
	lv_arc_set_rotation(arc, 270); //旋转90度

	/*数字显示*/
	lv_obj_t *label = lv_label_create(arc,NULL);	
	lv_obj_set_size(label,100,100);
	lv_label_set_text(label,"hello,circle");
	lv_obj_set_style_local_text_color(label,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_RED);	
	lv_obj_align(label, arc, LV_ALIGN_CENTER, 0, 0);

	#if 0
	lv_arc_set_range(arc,0,100);//设置范围为0-200度
	lv_arc_set_value(arc,80);//前景色设置就访问为100 即50%
	#else
	lv_arc_set_bg_angles(arc,90,360);
	lv_arc_set_angles(arc,45,181);

	#endif
	
    lv_obj_align(arc, NULL, LV_ALIGN_CENTER, 0, 0);	
	lv_task_create(ArcTaskCb, 10, LV_TASK_PRIO_LOWEST, arc);//创建了一个20ms，优先级为最低的ArcTaskCb重绘任务，并向任务的user_data参数传递arc指针
	
}
void setup() {
  // put your setup code here, to run once:
    pinMode(led, OUTPUT);

    Serial.begin(9600);

    lv_init();

    tft.begin();

    lv_disp_buf_init(&draw_buf_dsc_1, draw_buf_1, NULL, LV_HOR_RES_MAX * 10);   /*Initialize the display buffer*/

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.buffer = &draw_buf_dsc_1;
    lv_disp_drv_register(&disp_drv);

    lv_circle_show();
}



void loop() {

    lv_task_handler();

}