#include "configuration.h"
#include "rgb_light_configuration.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(500);

  Configuration * main_conf = load_configuration_and_module(load_module_configuration, free_module_configuration);
  RGBLightConfiguration * rgb_conf = (RGBLightConfiguration *) get_module_configuration(main_conf);
  RGB_LIGHT ** lights = get_lights(rgb_conf);
  RGB * color = get_color(rgb_conf, lights[0], 1);
  Serial.println(get_red_color(color));
  Serial.println(get_green_color(color));
  Serial.println(get_blue_color(color));

}

void loop() {
    
}
