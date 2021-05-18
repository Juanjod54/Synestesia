#include <string.h>
#include <Wire.h>
#include "audio.h"

float read_freq;

void send_last_read_freq() { 
  Wire.write(String(read_freq).c_str());
}

void setup() {  
  Wire.begin(1); 
  Wire.onRequest(send_last_read_freq);
}

void loop() {
  read_freq = get_frequency();
}
