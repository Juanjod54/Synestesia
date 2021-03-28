#include <rgb.h>
#include <rgb_light.h>

RGB * rgb;
RGB_LIGHT * light;

void setup() {
  Serial.begin(9600);
  
  rgb = create_rgb(255,255,255);
  light = create_rgb_light(13,14,15);
}

void loop() {
  set_rgb_light(light, rgb);
  delay(1000);
  turn_off_rgb_light(light);
  delay(1000);
}
