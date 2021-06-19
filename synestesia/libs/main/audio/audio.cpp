#include <math.h>
#include "audio.h"
#include "approxfft.h"

#ifndef SAMPLES
    #define SAMPLES 128
#endif

#ifndef MIC
    #define MIC 1
#endif

#ifndef SAMPLING_RATE
    #define SAMPLING_RATE 2000 
#endif

#define NOISE 500 

#define NOTES_PER_OCTAVE 7

//Takes period in ms
long sampling_period_us = round(1000000 * (1.0 / SAMPLING_RATE));

float get_frequency () {
    int vReal[SAMPLES];
    unsigned long new_time;

    // Sample the audio
    for (int i = 0; i < SAMPLES; i++) {
        new_time = micros();
        vReal[i] = analogRead(MIC) - NOISE;
        while ((micros() - new_time) < sampling_period_us);
    }

    //Get frequency of the highest wave, with an amplitude threshold 
    return Approx_FFT(vReal, SAMPLES, SAMPLING_RATE);

}