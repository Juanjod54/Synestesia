#include "configuration.h"
#include "rgb_light_configuration.h"
#include "wireless.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(500);

  Configuration * main_conf = load_configuration_and_module(load_rgb_light_configuration, 
                                                            save_rgb_light_configuration, 
                                                            marshall_rgb_light_configuration, 
                                                            unmarshall_rgb_light_configuration, 
                                                            free_rgb_light_configuration);
  start_server(main_conf);
  
}

void loop() {
    handle_client();  
}
