
#include "configuration.h"
#include "rgb_light_configuration.h"
#include "wireless.h"
#include "audio.h"

int tries = 0;
RGB * color;
RGB_LIGHT ** lights;
RGBLightConfiguration * module;



void setup() {
  
  Serial.begin(9600);
  while(!Serial) delay(500);
  Configuration * main_conf = load_configuration_and_module(load_rgb_light_configuration, 
                                                            save_rgb_light_configuration, 
                                                            marshall_rgb_light_configuration, 
                                                            unmarshall_rgb_light_configuration, 
                                                            free_rgb_light_configuration);
  
  module = (RGBLightConfiguration *) get_module_configuration(main_conf);
  lights = (RGB_LIGHT **) get_lights(module);
  start_server(main_conf);
  
}

void loop() {
  
  
    float read_freq = get_frequency();
    if (read_freq > 0) {
      Serial.println(read_freq);
      int note = get_note(read_freq, 2);
      Serial.println(note);
      color = get_color(module, lights[0], &note);
      set_rgb_light(lights[0], color);
    }
    else if (tries ++ > 25) {
      fade_out_rgb_light(lights[0], color);
    }
  
}
