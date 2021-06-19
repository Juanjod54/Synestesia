/**
 *  Created on 03-23-2021 by Juan Jose Daza Linares
 *  Filename: rgb.cpp
 **/

#include "rgb.h"

static char hash_char[10] = {'\0'};

struct _RGB{
    int red;
    int green;
    int blue;
    long hash;
};

int is_invalid_rgb(int red, int green, int blue) {
    // Limit values
    return (red < 0 || green < 0 || blue < 0 || red > 255 || green > 255 || blue > 255);
}

long create_hash(int red, int green, int blue) {
    long hash;

    if (is_invalid_rgb(red, green, blue)) return -1;
    
    sprintf(hash_char, "%d%d%d", red, green, blue);
    hash = atoi(hash_char);
    
    return hash;
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

    rgb -> hash = create_hash(red, green, blue);
    
    return rgb; 
}

/**
 * Frees allocated memory for RGB and assigns given pointer to NULL
**/ 
void delete_rgb(void * rgb) {
    if (!rgb) return;
    free(rgb);
    rgb = NULL;
}

/**
 * Frees allocated memory for RGB in array. Does not free array
**/
void delete_rgbs(void ** pt_rgbs, int length) {
    int i;
    RGB ** rgbs = (RGB **) pt_rgbs;

    if (rgbs == NULL) return;
    
    for (i = 0; i < length; i++) {
        free(rgbs[i]); 
        rgbs[i] = NULL;
    }
}

/**
 * Creates 'times' copies of given rgb
**/
RGB** copy_rgb(void * pt_rgb, int times) {
    int i;
    int red, green, blue;

    RGB *rgb_copy;
    RGB **rgbs;
    
    RGB * rgb = (RGB *) pt_rgb;

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
            delete_rgbs((void **) rgbs, i);
            // Exits
            return NULL;
        }

        rgb_copy -> hash = rgb -> hash;

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

    rgb -> hash = create_hash(red, green, blue);

    return rgb;
}

int equals(RGB * rgb1, RGB * rgb2) {
    if (rgb1 == NULL || rgb2 == NULL) return 0;
    if (rgb1 -> hash == rgb2 -> hash == NULL) return 1;
    return 0;
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

long get_rgb_hash(void * pt_rgb) {
    RGB * rgb = (RGB *) pt_rgb;
    return (rgb == NULL) ? -1 : rgb -> hash;
}

long simulate_rgb_hash(int red, int green, int blue) {
    return create_hash(red, green, blue);
}