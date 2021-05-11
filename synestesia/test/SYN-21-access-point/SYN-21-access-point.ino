#include "configuration.h"
#include "wireless.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(500);

  Configuration * main_conf = load_configuration();
  start_server(main_conf);
  Serial.println("Acces point ON. 5 seconds left before unload");
  delay(500);
  end_server();
  free_configuration(main_conf);
}

void loop() {
    
}
