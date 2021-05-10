<<<<<<< HEAD:synestesia/main/rgb_light/rgb_light.h
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

=======
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
void delete_rgb_light(void * rgb_light);

int get_red_connection(RGB_LIGHT * rgb_light);
int get_green_connection(RGB_LIGHT * rgb_light);
int get_blue_connection(RGB_LIGHT * rgb_light);

/**
 * Frees allocated memory for RGB_LIGHT in array. Does not free array
**/
void delete_rgb_lights(void ** pt_rgb_lights, int length);

void set_rgb_light(RGB_LIGHT * rgb_light, RGB * color);

void turn_off_rgb_light(RGB_LIGHT * rgb_light);

long get_rgb_light_hash(void * pt_rgb_light);

>>>>>>> develop:synestesia/module/rgb_light/rgb_light.h
#endif