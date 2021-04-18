#include "configuration.h"

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(500);
  
  Configuration * configuration = load_configuration();
  
  char * ssid = get_ssid(configuration);
  char * password = get_password(configuration);
  
  Serial.println("Configuration loaded OK");
  
  Serial.print("SSID: ");
  Serial.write(ssid);
  Serial.println("");

  Serial.print("Password: ");
  Serial.write((password == NULL) ? "Password is NULL, WiFi would be open" : password);
  Serial.println("");
  
  free_configuration(configuration);
  
}

void loop() {
    
}
