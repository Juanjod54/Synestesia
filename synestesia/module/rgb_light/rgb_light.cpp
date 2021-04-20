/**
 *  Created on 03-23-2021 by Juan Jose Daza Linares
 *  Filename: rgb_light.c 
 **/
#include "Arduino.h"

#include "rgb_light.h"

/** Logger **/
#ifdef DEBUG_ESP_PORT
#define logger(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define logger(...)
#endif

#define MAX_COLOR 255.0

struct _RGB_LIGHT {
    int red_con;
    int green_con;
    int blue_con;
};

RGB_LIGHT * create_rgb_light (int red_con, int green_con, int blue_con) {
    RGB_LIGHT *rgb_light;

    rgb_light = (RGB_LIGHT*) malloc(sizeof(rgb_light[0]));
    if (rgb_light == NULL) { 
        logger("(create_rgb_light) Could not allocate RGB_LIGHT object\n");
        return NULL;
    }

    rgb_light -> red_con = red_con;
    rgb_light -> green_con = green_con;
    rgb_light -> blue_con = blue_con;

    pinMode(rgb_light -> red_con, OUTPUT);
    pinMode(rgb_light -> green_con, OUTPUT);
    pinMode(rgb_light -> blue_con, OUTPUT);

    return rgb_light;
}

/**
 * Frees allocated memory for given RGB_LIGHT object.
**/
void delete_rgb_light(void * rgb_light) {
    if (rgb_light == NULL) return;
    free(rgb_light);
}

void set_rgb_light(RGB_LIGHT * rgb_light, RGB * color) {
    if (rgb_light == NULL || color == NULL) return;

    // Change rgb color on light.
    analogWrite(rgb_light -> red_con, get_red_color(color));
    analogWrite(rgb_light -> green_con, get_green_color(color));
    analogWrite(rgb_light -> blue_con, get_blue_color(color));
}

void turn_off_rgb_light(RGB_LIGHT * rgb_light) {
    if (rgb_light == NULL) return;

    // Turn off pwm on each led-connection
    analogWrite(rgb_light -> red_con, 0);
    analogWrite(rgb_light -> green_con, 0);
    analogWrite(rgb_light -> blue_con, 0);
}