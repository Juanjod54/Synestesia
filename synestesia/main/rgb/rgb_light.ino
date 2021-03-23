/**
 *  Created on 03-23-2021 by Juan Jose Daza Linares
 *  Filename: rgb_light.h 
 **/

#include "rgb_light.h"

#define MAX_PWM 1023
//analogWriteRange(new_range);


struct _RGB_LIGHT {
    int red_con;
    int green_con;
    int blue_con;
};

int get_pwm_for_color(int color) {
    return (color / 255) * MAX_PWM;
}

RGB_LIGHT * create_rgb_light (int red_con, int green_con, int blue_con) {
    RGB_LIGHT *rgb_light;

    rgb_light = malloc(sizeof(rgb_light[0]));
    if (rgb_light == NULL) return NULL;

    rgb_light -> red_con = red_con;
    rgb_light -> green_con = green_con;
    rgb_light -> blue_con = blue_con;

    return rgb_light;
}

/**
 * Frees allocated memory for given RGB_LIGHT object.
**/
void delete_rgb_light(RGB_LIGHT * rgb_light) {
    if (rgb_light == NULL) return;
    free(rgb_light);
}

void set_rgb_light(RGB_LIGHT * rgb_light, RGB * color) {
    if (rgb_light == NULL || color == NULL) return;

    // Change rgb color on light. Gets pwm for each led-connection (color)
    analogWrite(rgb_light -> red_con, get_pwm_for_color(get_red_color(color)));
    analogWrite(rgb_light -> green_con, get_pwm_for_color(get_green_color(color)));
    analogWrite(rgb_light -> blue_con, get_pwm_for_color(get_blue_color(color)));
}

void turn_off_rgb_light(RGB_LIGHT * rgb_light) {
    if (rgb_light == NULL || color == NULL) return;

    // Turn off pwm on each led-connection
    analogWrite(rgb_light -> red_con, 0);
    analogWrite(rgb_light -> green_con, 0);
    analogWrite(rgb_light -> blue_con, 0);
}