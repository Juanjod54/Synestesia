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

static char hash_char[7] = {'\0'};

struct _RGB_LIGHT {
    int red_conn;
    int green_conn;
    int blue_conn;
    int red_value;
    int green_value;
    int blue_value;
    long hash;
};

void update_values(RGB_LIGHT * rgb_light, RGB * color) {
    if (rgb_light == NULL || color == NULL) return;

    rgb_light -> red_value = get_red_color(color);
    rgb_light -> green_value = get_green_color(color);
    rgb_light -> blue_value = get_blue_color(color);
}

void write_values(RGB_LIGHT * rgb_light) {
    if (rgb_light == NULL) return;

    analogWrite(rgb_light -> red_conn, rgb_light -> red_value);
    analogWrite(rgb_light -> green_conn, rgb_light -> green_value);
    analogWrite(rgb_light -> blue_conn, rgb_light -> blue_value);
}

long create_rgb_light_hash(int red_conn, int green_conn, int blue_conn) {
    long hash;

    sprintf(hash_char, "%d%d%d", red_conn, green_conn, blue_conn);
    hash = atoi(hash_char);

    return hash;
}

RGB_LIGHT * create_rgb_light (int red_conn, int green_conn, int blue_conn) {
    RGB_LIGHT *rgb_light;

    rgb_light = (RGB_LIGHT*) malloc(sizeof(rgb_light[0]));
    if (rgb_light == NULL) { 
        logger("(create_rgb_light) Could not allocate RGB_LIGHT object\n");
        return NULL;
    }

    rgb_light -> red_conn = red_conn;
    rgb_light -> green_conn = green_conn;
    rgb_light -> blue_conn = blue_conn;

    rgb_light -> hash = create_rgb_light_hash(red_conn, green_conn, blue_conn);

    pinMode(rgb_light -> red_conn, OUTPUT);
    pinMode(rgb_light -> green_conn, OUTPUT);
    pinMode(rgb_light -> blue_conn, OUTPUT);

    return rgb_light;
}

/**
 * Frees allocated memory for given RGB_LIGHT object.
**/
void delete_rgb_light(void * rgb_light) {
    if (rgb_light == NULL) return;
    free(rgb_light);
    rgb_light = NULL;
}

/**
 * Frees allocated memory for RGB_LIGHT in array. Does not free array
**/
void delete_rgb_lights(void ** pt_rgb_lights, int length) {
    int i;
    RGB_LIGHT ** rgb_lights = (RGB_LIGHT **) pt_rgb_lights;

    if (rgb_lights == NULL) return;
    
    for (i = 0; i < length; i++) {
        free(rgb_lights[i]); 
        rgb_lights[i] = NULL;
    }
}

int get_red_connection(RGB_LIGHT * rgb_light) {
    return (rgb_light == NULL) ? -1 : rgb_light -> red_conn;
}

int get_green_connection(RGB_LIGHT * rgb_light) {
    return (rgb_light == NULL) ? -1 : rgb_light -> green_conn;
}

int get_blue_connection(RGB_LIGHT * rgb_light) {
    return (rgb_light == NULL) ? -1 : rgb_light -> blue_conn;
}

void set_rgb_light(RGB_LIGHT * rgb_light, RGB * color) {
    if (rgb_light == NULL || color == NULL) return;

    update_values(rgb_light, color);

    // Change rgb color on light.
    write_values(rgb_light);
}

void fade_out_rgb_light(RGB_LIGHT * rgb_light, RGB * color) {
    if (rgb_light == NULL || color == NULL) return;
    
    int red = rgb_light -> red_value - 10;
    int green = rgb_light -> green_value - 10;
    int blue = rgb_light -> blue_value - 10;

    rgb_light -> red_value = (red > 0) ? red : 0;
    rgb_light -> green_value = (green > 0) ? green : 0;
    rgb_light -> blue_value = (blue > 0) ? blue : 0;

    // Change rgb color on light.
    write_values(rgb_light);
}

void turn_off_rgb_light(RGB_LIGHT * rgb_light) {
    if (rgb_light == NULL) return;

    rgb_light -> red_value = 0;
    rgb_light -> green_value = 0;
    rgb_light -> blue_value = 0;

    // Turn off pwm on each led-connection
    write_values(rgb_light);
}

long get_rgb_light_hash(void * pt_rgb_light) {
    RGB_LIGHT * rgb_light = (RGB_LIGHT *) pt_rgb_light;
    return (rgb_light == NULL) ? -1 : rgb_light -> hash;
}