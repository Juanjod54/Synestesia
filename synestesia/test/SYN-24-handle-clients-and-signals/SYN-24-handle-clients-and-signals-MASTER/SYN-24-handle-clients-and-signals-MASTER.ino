
#include "Thread.h"
#include <Wire.h>

#include "Synestesia.h"
#include "configuration.h"
#include "rgb_light_configuration.h"
#include "wireless.h"
#include "audio.h"

float wire_value;
size_t bytes = sizeof(int);

RGB * color;
RGB_LIGHT ** lights;
RGBLightConfiguration * module;

Thread freq_thread = Thread();

void setup() {

  Wire.begin(D1, D2);
  
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
  Synestesia * synestesia = initialize(&moduleFunctions, MASTER);
  Configuration * main_conf = get_configuration(synestesia);
  
  /* Gets module */
  module = (RGBLightConfiguration *) get_module_configuration(main_conf);
  /* Gets lights stored at module */
  lights = (RGB_LIGHT **) get_lights(module);

  /* Configure lightning thread */
  freq_thread.onRun(handle_signals);
   
}

void handle_signals() {
    
    Wire.requestFrom(1, 10);
    wire_value = Wire.parseFloat();
    if (wire_value > 0) {
      int note = get_note(wire_value, 2);
      Serial.println(wire_value);
      Serial.println(note);
      color = get_color(module, lights[0], &note);
      set_rgb_light(lights[0], color);
    }
    else {
      fade_out_rgb_light(lights[0]);
    }
}

void loop() {
    handle_client();
    if (freq_thread.shouldRun()) { freq_thread.run(); }
}
