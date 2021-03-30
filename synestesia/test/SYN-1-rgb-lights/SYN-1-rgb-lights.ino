/***********************************
 * # Brief
 *    This program tests RGB and RGB_LIGHT libraries. 
 *    It uses RGB and RGB_LIGHT libraries to create a RGB color and a RGB Light that will fade colors in a loop.
 * # Created on 03-29-2021 by Juan Jose Daza Linares
***********************************/

#include <rgb.h>
#include <rgb_light.h>

#define DELIMITER 32

char input[15];

/***** RGB LIGHT N 1 *****/
  RGB * RGB_1 = NULL;
  RGB_LIGHT * LIGHT_1 = NULL;

   // Colors
  int LIGHT_1_RED;
  int LIGHT_1_GREEN;
  int LIGHT_1_BLUE;

  // Connections (Set OUTPUT pinout)
  int LIGHT_1_CON_RED = 6;
  int LIGHT_1_CON_GREEN = 3;
  int LIGHT_1_CON_BLUE = 5;
/*************************/


void setup() {
  //For debug purposes
  Serial.begin(9600);
  
  //Creates one light
  LIGHT_1 = create_rgb_light(LIGHT_1_CON_RED, LIGHT_1_CON_GREEN, LIGHT_1_CON_BLUE);
  RGB_1 = create_rgb(LIGHT_1_RED, LIGHT_1_GREEN, LIGHT_1_BLUE);
  
}

void loop() {

  for (LIGHT_1_RED = 0; LIGHT_1_RED < 255; LIGHT_1_RED ++) {
     set_color(RGB_1, LIGHT_1_RED, LIGHT_1_GREEN, LIGHT_1_BLUE);
     set_rgb_light(LIGHT_1, RGB_1);
     delay(5);
  }

  for (LIGHT_1_GREEN = 0; LIGHT_1_GREEN < 255; LIGHT_1_GREEN ++) {
     set_color(RGB_1, LIGHT_1_RED, LIGHT_1_GREEN, LIGHT_1_BLUE);
     set_rgb_light(LIGHT_1, RGB_1);
     delay(5);
  }

  for (LIGHT_1_BLUE = 0; LIGHT_1_BLUE < 255; LIGHT_1_BLUE ++) {
     set_color(RGB_1, LIGHT_1_RED, LIGHT_1_GREEN, LIGHT_1_BLUE);
     set_rgb_light(LIGHT_1, RGB_1);
     delay(5);
  }

  delay(500);

  for (LIGHT_1_RED; LIGHT_1_RED > 0; LIGHT_1_RED --) {
     set_color(RGB_1, LIGHT_1_RED, LIGHT_1_GREEN, LIGHT_1_BLUE);
     set_rgb_light(LIGHT_1, RGB_1);
     delay(5);
  }

  for (LIGHT_1_GREEN; LIGHT_1_GREEN > 0; LIGHT_1_GREEN --) {
     set_color(RGB_1, LIGHT_1_RED, LIGHT_1_GREEN, LIGHT_1_BLUE);
     set_rgb_light(LIGHT_1, RGB_1);
     delay(5);
  }

  for (LIGHT_1_BLUE; LIGHT_1_BLUE > 0; LIGHT_1_BLUE --) {
     set_color(RGB_1, LIGHT_1_RED, LIGHT_1_GREEN, LIGHT_1_BLUE);
     set_rgb_light(LIGHT_1, RGB_1);
     delay(5);
  }

}
