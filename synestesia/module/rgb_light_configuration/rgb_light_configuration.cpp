/**
 *  Created on 04-18-2021 by Juan Jose Daza Linares
 *  Filename: rgb_light_configuration.cpp 
 **/

#include <string.h>

#include "map.h"
#include "file_manager.h"

#include "rgb.h"
#include "rgb_light.h"
#include "rgb_light_configuration.h"

/** Logger **/
#ifdef DEBUG_ESP_PORT
#define logger(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define logger(...)
#endif

#define LIGHT_KEYWORD "LIGHT"

/* MODULE configuration file */
#define MODULE_CONFIGURATION_FILE "/module.syn"

/*Module configuration object*/
struct _RGBLightConfiguration {
    RGB ** colors; //Colors array
    RGB_LIGHT ** lights; //Lights array
    Map * colors_map; // Maps a color per note
    Map * light_colors_map; //Maps a light with a Map of colors per note
};

RGBLightConfiguration * create_rgb_configuration() {
    RGBLightConfiguration * configuration = (RGBLightConfiguration *) malloc(sizeof(configuration[0]));
    if (configuration == NULL) return NULL;

    return configuration;
}

long color_hash(void * value) {
    const char * note = (char *) value;
    if (note == NULL) return -1;
    if (strcmp(note,"DO") == 0) return 0;
    if (strcmp(note,"RE") == 0) return 1;
    if (strcmp(note,"MI") == 0) return 2;
    if (strcmp(note,"FA") == 0) return 3;
    if (strcmp(note,"SOL") == 0) return 4;
    if (strcmp(note,"LA") == 0) return 5;
    if (strcmp(note,"SI") == 0) return 6;
    
    return -1;
}

long light_hash(void * light) {
    if (light == NULL) return -1;
    return 0; //VERY DUMMY
}

/** DUMMY **/
RGBLightConfiguration * load_rgb_light_configuration() {
    RGB ** colors;
    RGB_LIGHT ** lights;

    Map * colors_map;
    Map * light_colors_map;

    RGBLightConfiguration * conf; 

    //7 colores
    colors = (RGB **) malloc(sizeof(colors[0]) * 7);
    if (colors == NULL) { return NULL; }

    colors[0] = create_rgb(255, 255, 255);
    colors[1] = create_rgb(0, 0, 0); 
    colors[2] = create_rgb(0, 255, 0);
    colors[3] = create_rgb(0, 0, 255);
    colors[4] = create_rgb(255, 0, 0);
    colors[5] = create_rgb(255, 255, 0);
    colors[6] = create_rgb(0, 255, 255);


    //1 luz
    lights = (RGB_LIGHT **) malloc(sizeof(lights[0]));
    if (lights == NULL) { return NULL;}
    lights[0] = create_rgb_light(13, 12, 14);

    char * names[7] = {"DO", "RE", "MI", "FA", "SOL", "LA", "SI"};

    //Mapa colores
    colors_map = create_map(color_hash, free, delete_rgb);
    map_put(colors_map, names[0], colors[0]);
    map_put(colors_map, names[1], colors[1]);
    map_put(colors_map, names[2], colors[3]);
    map_put(colors_map, names[3], colors[2]);
    map_put(colors_map, names[4], colors[4]);
    map_put(colors_map, names[5], colors[5]);
    map_put(colors_map, names[6], colors[6]);

    light_colors_map = create_map(light_hash, delete_rgb_light, free_map);
    map_put(light_colors_map, lights[0], colors_map);

    conf = create_rgb_configuration();
    
    conf -> colors = colors;
    conf -> lights = lights;
    conf -> colors_map = colors_map;
    conf -> light_colors_map = light_colors_map;

    return conf;

}

RGB_LIGHT ** get_lights(RGBLightConfiguration * configuration) {
    if (configuration == NULL) { return NULL; }
    return configuration -> lights;
}

RGB * get_color(RGBLightConfiguration * configuration, RGB_LIGHT * light, char * note) {
    if (configuration == NULL || light == NULL) { return NULL; }
    Map * colors_per_light = (Map *) map_get(configuration -> light_colors_map, light);
    return (RGB *) map_get(colors_per_light, note);
}

/*
 * Frees allocated memory for configuration object. Finishes file manager.
 * @param configuration: The configuration object to free
*/ 
void free_regb_light_configuration(RGBLightConfiguration * configuration);