/*
 * Example of SYNESTESIA SLAVE.
 * THIS EXAMPLE IS MEANT TO BE USED WITH MASTER EXAMPLE
 */

#include "Synestesia.h"

int err = 0;
Synestesia * synestesia;

void setup() {

  /* Initiates Synestesia object */
  synestesia = initialize_by_type(SLAVE);
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
