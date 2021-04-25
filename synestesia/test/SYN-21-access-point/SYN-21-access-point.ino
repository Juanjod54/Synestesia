#include "configuration.h"
#include "wireless.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(500);

  Configuration * main_conf = load_configuration();
  create_access_point(main_conf);
  Serial.println("Acces point ON. 2 minutes left before unload");
  delay(120000);
  end_acces_point();
  free_configuration(main_conf);
}

void loop() {
    
}
