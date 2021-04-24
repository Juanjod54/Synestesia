#include "configuration.h"
#include "rgb_light_configuration.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(500);

  Configuration * main_conf = load_configuration_and_module(load_module_configuration, free_module_configuration);
  RGBLightConfiguration * rgb_conf = (RGBLightConfiguration *) get_module_configuration(main_conf);
  RGB_LIGHT ** lights = get_lights(rgb_conf);
  
  RGB * color_0 = get_color(rgb_conf, lights[0], 2);
  Serial.println(get_red_color(color_0));
  Serial.println(get_green_color(color_0));
  Serial.println(get_blue_color(color_0));

  RGB * color_1 = get_color(rgb_conf, lights[1], 2);
  Serial.println(get_red_color(color_1));
  Serial.println(get_green_color(color_1));
  Serial.println(get_blue_color(color_1));

  free_configuration(main_conf);
}

void loop() {
    
}
