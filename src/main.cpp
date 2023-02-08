#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "display.h"

Display screen;

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

static void lv_circle_show(void)
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
	static lv_style_t font_style;
    lv_style_set_text_color(&font_style, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_style_set_text_font(&font_style, LV_STATE_DEFAULT, &lv_font_montserrat_20);

	lv_obj_t *label = lv_label_create(arc,NULL);	
	lv_obj_set_size(label,100,100);
	lv_label_set_text(label,"hello, lvgl");
	lv_obj_add_style(label, LV_OBJ_PART_MAIN, &font_style);
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

    
    Serial.begin(9600);

	screen.init();

    lv_circle_show();
}

void loop() {

	screen.update();
}