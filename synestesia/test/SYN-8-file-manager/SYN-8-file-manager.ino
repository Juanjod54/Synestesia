#include "file_manager.h"
#include <FS.h>

char * content;

void setup() {
  Serial.begin(9600);
  init_file_manager(false);
  
  while(! Serial);
  Serial.println("Start");

  content = read_from_file("/global.syn");
  Serial.write(content);
}

void loop() {
    
}
