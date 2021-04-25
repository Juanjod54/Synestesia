
/**
 *  Created on 03-23-2021 by Juan Jose Daza Linares
 *  Filename: rgb.h 
 **/

#ifndef RGB_h
#define RGB_h

#include "Arduino.h"

typedef struct _RGB RGB;

/**
 * Allocates memory for RGB structure and sets its rgb values to the given ones
**/
RGB * create_rgb (int red, int green, int blue);

/**
 * Frees allocated memory for RGB and assigns given pointer to NULL
**/ 
void delete_rgb(RGB * rgb);

/**
 * Frees allocated memory for RGB array and assigns given pointer to NULL
**/ 
void delete_rgbs(RGB ** rgbs, int length);

/**
 * Creates 'times' copies of given rgb
**/
RGB** copy_rgb(RGB * rgb, int times);

/**
 * Changes color of given rgb
**/
RGB * set_color(RGB *rgb, int red, int green, int blue);

/**
 * Returns red color value (0 - 255)
**/
int get_red_color(RGB * rgb);

/**
 * Returns green color value (0 - 255)
**/
int get_green_color(RGB * rgb);

/**
 * Returns blue color value (0 - 255)
**/
int get_blue_color(RGB * rgb);

#endif