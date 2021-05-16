#include <string.h>
#include <Wire.h>
#include "audio.h"

float read_freq;
unsigned int last_freq = 0;

void send_last_read_freq() { Wire.write(last_freq); }

void setup() { 
  Wire.begin(1); 
  Wire.onRequest(send_last_read_freq);
}

void loop() {
  read_freq = get_frequency();
  last_freq = read_freq * 100;
}
