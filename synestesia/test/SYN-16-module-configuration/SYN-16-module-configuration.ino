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

  for (int note = 1; note < 10; note ++) {
    Serial.println(note);
    RGB * color = (RGB *) get_color(rgb_conf, lights[0], &note);
    if (color == NULL) Serial.println("No color at " + note);
    else {
      Serial.print("Red: ");
      Serial.println(get_red_color(color));
      Serial.print("Green: ");
      Serial.println(get_green_color(color));
      Serial.print("Blue: ");
      Serial.println(get_blue_color(color));
    }
  }

  free_configuration(main_conf);
}

void loop() {
    
}
