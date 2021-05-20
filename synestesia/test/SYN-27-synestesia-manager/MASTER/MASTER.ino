/*
 * Example of SYNESTESIA MASTER.
 * THIS EXAMPLE IS MEAN TO BE USED WITH RGB_LIGHTS_MODULE AND ONE LIGHT CONFIGURED
 */

#include "Synestesia.h"
#include "rgb_light_configuration.h"

int err = 0;
Synestesia * synestesia;

RGB * color;
RGB_LIGHT ** lights;
RGBLightConfiguration * module;

void setup() {

  Serial.begin(9600);
  while(!Serial) delay(500);

  /* Creates ModuleFunctions object */
  ModuleFunctions moduleFunctions;
  moduleFunctions.load_fn = load_rgb_light_configuration;
  moduleFunctions.free_fn = free_rgb_light_configuration;
  moduleFunctions.save_fn = save_rgb_light_configuration;
  moduleFunctions.marshal_fn = marshal_rgb_light_configuration;
  moduleFunctions.unmarshal_fn = unmarshal_rgb_light_configuration;

  /* Initiates Synestesia object */
  synestesia = initialize(&moduleFunctions);
  if (! synestesia) { 
    Serial.println("ERROR creating synestesia");
    err = 1;
    return;
  }

  /* Gets global configuration */
  Configuration * main_conf = get_configuration(synestesia);

  /* Gets module */
  module = (RGBLightConfiguration *) get_module_configuration(main_conf);
  
  /* Gets lights stored at module */
  lights = (RGB_LIGHT **) get_lights(module);
  if (lights == NULL) {
    Serial.println("ERROR GETTING MODULE");
    err = 1;
    return;
  }
 
}

void loop() {
  if (! err) {
      /* Runs synestesia */
      float freq = run_core(synestesia);
      
      if (freq > 0) {
        int note = get_note(freq, 2);
        color = get_color(module, lights[0], &note);
        set_rgb_light(lights[0], color);
      }
      else {
        fade_out_rgb_light(lights[0]);
      }
  }
}
