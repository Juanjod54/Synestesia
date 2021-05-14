#ifndef AUDIO_h
#define AUDIO_h

#include <Arduino.h>

float get_frequency();

int get_note (float read_freq, int base_freq);

#endif