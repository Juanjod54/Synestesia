/**
 *  Created on 03-23-2021 by Juan Jose Daza Linares
 *  Filename: rgb_light.h 
 **/

#ifndef RGB_LIGHT_h
#define RGB_LIGHT_h

#include "rgb.h"

typedef struct _RGB_LIGHT RGB_LIGHT;

RGB_LIGHT * create_rgb_light (int red_con, int green_con, int blue_con);

/**
 * Frees allocated memory for given RGB_LIGHT object.
**/
void delete_rgb_light(RGB_LIGHT * rgb_light);

void set_rgb_light(RGB_LIGHT * rgb_light, RGB * color);

void turn_off_rgb_light(RGB_LIGHT * rgb_light);

#endif