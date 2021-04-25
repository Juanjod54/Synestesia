/**
 *  Created on 03-23-2021 by Juan Jose Daza Linares
 *  Filename: rgb.ino
 **/

#include "rgb.h"

struct _RGB{
    int red;
    int green;
    int blue;
};

int is_invalid_rgb(int red, int green, int blue) {
    // Limit values
    return (red < 0 || green < 0 || blue < 0 || red > 255 || green > 255 || blue > 255);
}

/**
 * Allocates memory for RGB structure and sets its rgb values to the given ones
**/
RGB * create_rgb (int red, int green, int blue) {
    RGB * rgb;
    
    // Limit values
    if (is_invalid_rgb(red, green, blue)) return NULL;

    rgb = (RGB*) malloc(sizeof(rgb[0]));
    if (rgb == NULL) return NULL;

    rgb -> red = red;
    rgb -> green = green;
    rgb -> blue = blue;
    
    return rgb; 
}

/**
 * Frees allocated memory for RGB and assigns given pointer to NULL
**/ 
void delete_rgb(RGB * rgb) {
    free(rgb);
    rgb = NULL;
}

/**
 * Frees allocated memory for RGB array and assigns given pointer to NULL
**/ 
void delete_rgbs(RGB ** rgbs, int length) {
    int i;
    
    if (rgbs == NULL) return;
    for (i = 0; i < length; i++)
        free(rgbs[i]); 
    
    free(rgbs);
    rgbs = NULL;
}

/**
 * Creates 'times' copies of given rgb
**/
RGB** copy_rgb(RGB * rgb, int times) {
    int i;
    int red, green, blue;

    RGB *rgb_copy;
    RGB **rgbs;

    if (rgb == NULL || times < 1) return NULL;

    rgbs = (RGB**) malloc(times * sizeof(rgbs[0]));
    if (rgbs == NULL) return NULL;

    //Avoids memory accessing;
    red = rgb -> red;
    green = rgb -> green;
    blue = rgb -> blue;

    for (i = 0; i < times; i++) {
        rgb_copy = create_rgb(red, green, blue);
        
        if (rgb_copy == NULL) { 
            // Clears ONLY created RGBs
            delete_rgbs(rgbs, i);
            // Exits
            return NULL;
        }

        rgbs[i] = rgb_copy;
    }

    return rgbs;
}

RGB * set_color(RGB *rgb, int red, int green, int blue) {
    if (rgb == NULL) return NULL;
    
    // Limit values
    if (is_invalid_rgb(red, green, blue)) return NULL;

    rgb -> red = red;
    rgb -> green = green;
    rgb -> blue = blue;

    return rgb;
}

int get_red_color(RGB * rgb) {
    return (rgb == NULL) ? 0 : rgb -> red;
}

int get_green_color(RGB * rgb) {
    return (rgb == NULL) ? 0 : rgb -> green;
}

int get_blue_color(RGB * rgb) {
    return (rgb == NULL) ? 0 : rgb -> blue;
}