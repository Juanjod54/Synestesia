#include "configuration.h"
#include "rgb_light_configuration.h"
#include "wireless.h"
#include "postUp.h"

void setup () {
  Serial.begin(9600);
  while(!Serial) delay(500);

  /*Loads configuration and module configuration*/
  Configuration * main_conf = load_configuration_and_module(load_rgb_light_configuration, 
                                                            save_rgb_light_configuration, 
                                                            marshall_rgb_light_configuration, 
                                                            unmarshall_rgb_light_configuration, 
                                                            free_rgb_light_configuration);

  /* Starts configuration server */
  start_server(main_conf);
  
  start_udp();
}

void loop() {
  delay(1000);
  send_package("test", IPAddress(192,168,1,255));
}
