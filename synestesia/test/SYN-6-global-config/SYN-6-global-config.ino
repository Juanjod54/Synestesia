#include "configuration.h"

void setup() {
  Serial.begin(9600);
  delay(5000);
  Configuration * configuration = load_configuration();
  Serial.println("Configuration loaded OK");
 
  free_configuration(configuration);
  
}

void loop() {
    
}
