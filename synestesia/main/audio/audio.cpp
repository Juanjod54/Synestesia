#include <math.h>
#include "audio.h"
#include "approxfft.h"

#ifndef SAMPLES
    #define SAMPLES 128
#endif

#ifndef MIC
    #define MIC 0
#endif

#ifndef SAMPLING_RATE
    #define SAMPLING_RATE 2000 
#endif

#define NOISE 500 

#define NOTES_PER_OCTAVE 7
#define BASE_FREQ 16

int base_freqs[] = {16, 18, 21, 22, 24, 28, 31};

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
    return Approx_FFT(vReal, SAMPLES, sampling_period_us);

}

int get_note (float read_freq, int octaves) {
    
    //Gets octave (from 1 to 9)
    int current_octave = floor(log2(read_freq / base_freqs[0]));
    return ((read_freq / (pow(2, current_octave))) - base_freqs[0]) / 2 + 1;
}