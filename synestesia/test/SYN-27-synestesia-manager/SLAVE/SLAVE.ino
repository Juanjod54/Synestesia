/*
 * Example of SYNESTESIA SLAVE.
 * THIS EXAMPLE IS MEANT TO BE USED WITH MASTER EXAMPLE
 */

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
      run_core(synestesia);
  }
}
