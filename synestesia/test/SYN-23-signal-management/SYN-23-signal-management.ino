
#include "Synestesia.h"

int err = 0;
Synestesia * synestesia;

void setup() {
  Serial.begin(9600);
  /* Initiates Synestesia object */
  synestesia = initialize_slave();
  if (synestesia == NULL) { 
    Serial.println("ERROR creating synestesia");
    err = 1;
    return;
  }
 
}

void loop() {
  if (! err) {
      /* Runs synestesia */
      float freq = run_core(synestesia);
      Serial.println(freq);
  }
}
