#include <rgb.h>
#include <rgb_light.h>

void setup() {
  RGB * rgb = create_rgb(255,255,255);
  RGB_LIGHT * light = create_rgb_light(13,14,15);
  set_rgb_light(light, rgb);

}

void loop() {
  // put your main code here, to run repeatedly:

}
