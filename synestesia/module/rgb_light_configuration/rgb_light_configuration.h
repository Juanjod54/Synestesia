/**
 *  Created on 04-18-2021 by Juan Jose Daza Linares
 *  Filename: rgb_light_configuration.h 
 **/

#ifndef RGB_LIGHT_CONFIGURATION_h
#define RGB_LIGHT_CONFIGURATION_h

#include "rgb.h"
#include "rgb_light.h"

#include "Arduino.h"


/*Module configuration object*/
typedef struct _RGBLightConfiguration RGBLightConfiguration;

/*
 * Loads configuration file into a new Configuration object. Loads file manager.
 * If all required fields were found and valid, it returns a new Configuration object with read configuration
 * If there is any missing field or there is any error it returns NULL
*/ 
void * load_rgb_light_configuration();

int save_rgb_light_configuration(void * pt_configuration);

/*
 * Frees allocated memory for configuration object. Finishes file manager.
 * @param configuration: The configuration object to free
*/ 
void free_rgb_light_configuration(void * pt_configuration);

RGB_LIGHT ** get_lights(RGBLightConfiguration * configuration);

RGB * get_color(RGBLightConfiguration * configuration, RGB_LIGHT * light, int * note);

char * marshall_rgb_light_configuration(void * pt_configuration);

void * unmarshall_rgb_light_configuration(char * configuration_text);

int get_note (float read_freq, int base_freq);

#endif