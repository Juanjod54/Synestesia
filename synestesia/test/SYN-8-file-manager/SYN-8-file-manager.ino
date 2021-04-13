#include "file_manager.h"
#include <FS.h>

char * content;

void setup() {
  Serial.begin(9600);
  while(! Serial){delay(500);}
  
  init_file_manager();
  
  Serial.println("Configuration load has finished");

  Serial.println("Preparing to read global.syn configuration file:");
  content = read_from_file("/global.syn");
  Serial.write(content);
  Serial.println("");
  
  Serial.println("Preparing to overwrite global.syn configuration file:");
  write_to_file("/global.syn", "New configuration values ... ");

  Serial.println("Preparing to read global.syn configuration file after overwrite:");
  content = read_from_file("/global.syn");
  Serial.write(content);
  Serial.println("");
  
  end_file_manager();
}

void loop() {
    
}
