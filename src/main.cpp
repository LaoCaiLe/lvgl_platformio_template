#include <Arduino.h>
#include <TFT_eSPI.h>
#include "lv_demos.h"
#include <lvgl.h>
#include "display.h"

Display screen;

void setup() 
{
	screen.init();
	
	lv_demo_music();

}

void loop() 
{
	screen.update();
}