#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "display.h"
#include "lv_demos.h"

Display screen;

extern void lv_demo_music(void);


void setup() {

    Serial.begin(9600);

    screen.init();
 
    lv_demo_music();

}

void loop() {
    screen.update();
}