#include "configuration.h"
#include "Synestesia.h"
#include "rgb_light_configuration.h"
#include "postUp.h"

void setup () {
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
}

void loop() {
  delay(1000);
  send_package("test", IPAddress(192,168,1,255));
}
