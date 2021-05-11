#include "configuration.h"
#include "rgb_light_configuration.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(500);

  Configuration * main_conf = load_configuration_and_module(load_rgb_light_configuration, 
                                                            save_rgb_light_configuration, 
                                                            marshall_rgb_light_configuration, 
                                                            unmarshall_rgb_light_configuration, 
                                                            free_rgb_light_configuration);
                                                            
  RGBLightConfiguration * rgb_conf = (RGBLightConfiguration *) get_module_configuration(main_conf);
  RGB_LIGHT ** lights = get_lights(rgb_conf);

  int note = 1;
  
  RGB * color = (RGB *) get_color(rgb_conf, lights[0], &note);
  Serial.print("Red: ");
  Serial.println(get_red_color(color));
  Serial.print("Green: ");
  Serial.println(get_green_color(color));
  Serial.print("Blue: ");
  Serial.println(get_blue_color(color));

  free_configuration(main_conf);
}

void loop() {
    
}
