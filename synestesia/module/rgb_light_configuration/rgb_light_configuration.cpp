/**
 *  Created on 04-18-2021 by Juan Jose Daza Linares
 *  Filename: rgb_light_configuration.cpp 
 **/

#include <string.h>

#include "little_hash_map.h"
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
#define CONNECTION_KEYWORD "CONN"

/* MODULE configuration file */
#define MODULE_CONFIGURATION_FILE "/module.syn"

#define MAX_LIGHTS 2
#define MAX_COLORS 14
#define MAX_COLORS_PER_LIGHT 14

/*Module configuration object*/
struct _RGBLightConfiguration {
    int n_lights;
    LittleHashMap * colors_map; // Avoids duplicated RGB instances
    LittleHashMap * lights_map; //LittleHashMaps a light with a LittleHashMap of colors per note
};

//colors_map stores hash -> color, so its hash function will be the hash itself
long colors_map_hash(void * hash) {
    return * ((long *) hash);
}

long lights_map_hash(void * light) {
    return get_rgb_light_hash(light);
}



int is_light_keyword(char * keyword) {
    if (keyword == NULL) return 0;
    if (strcmp(keyword, LIGHT_KEYWORD) == 0) return 1;
    return 0;
}

int is_connection_keyword(char * keyword) {
    if (keyword == NULL) return 0;
    if (strcmp(keyword, CONNECTION_KEYWORD) == 0) return 1;
    return 0;
}

void add_rgb_color(RGBLightConfiguration * configuration, RGB_LIGHT * light, int key, RGB * value) {
    int * key_ptr;
    long  * color_hash_ptr;

    if (configuration == NULL || light == NULL || value == NULL) return;
    
    logger("(add_rgb_color) Adding new color\n");

    //color_hash = (long *) malloc(sizeof(color_hash[0]));

    //Gets color's hash 
    color_hash_ptr = (long *) malloc(sizeof(color_hash_ptr));
    * color_hash_ptr = get_rgb_hash(value);

    key_ptr = (int *) malloc(sizeof(key_ptr));
    * key_ptr = key;

    //If colors_map does not have this color, it must be added to colors map and colors array
    if (map_get(configuration -> colors_map, color_hash_ptr) == NULL) {
        //Adds to colors map
        map_put(configuration -> colors_map, color_hash_ptr, value);
    }

    //Get colors map for light
    LittleHashMap * colors_per_light = (LittleHashMap *) map_get(configuration -> lights_map, light);
    
    if (colors_per_light != NULL) {
        //Adds color to light's colors map
        if (! map_put(colors_per_light, key_ptr, value)) logger("(add_rgb_color) Could not add color to light color's map");
    }
    else logger("(add_rgb_color) Could not find light in lights map");
}

void add_rgb_light(RGBLightConfiguration * configuration, RGB_LIGHT * light) {
    LittleHashMap * colors_map;
    if (configuration == NULL || light == NULL) return;

    logger("(add_rgb_light) Adding new light\n");

    colors_map = create_map(colors_map_hash, free, delete_rgb, MAX_COLORS_PER_LIGHT);
    if (colors_map == NULL) {
        logger("(add_rgb_light) Could not create map for light's colors");
        return;
    }

    if (! map_put(configuration -> lights_map, light, colors_map)) logger("(add_rgb_light) Could not add light to lights map\n");
}

RGB * parse_color(RGBLightConfiguration * configuration, char * line) {
    long hash;
    int r, g, b;
    char * position;
    char * red, * green, * blue;
    
    RGB * color;

    red = get_first_without_tabs_nor_spaces( strtok_r(line, ",", &position) );
    green = get_first_without_tabs_nor_spaces( strtok_r(NULL, ",", &position) );
    blue = get_first_without_tabs_nor_spaces( strtok_r(NULL, ",", &position) );

    logger("(parse_color) Read color: R:%s, G:%s, B:%s\n", red, green, blue);

    r = atoi(red);
    g = atoi(green);
    b = atoi(blue);

    //If color already exists it does not create a new one
    hash = simulate_rgb_hash(r, g, b);
    color = (RGB *) map_get(configuration -> colors_map, &hash);
    
    return (color == NULL) ? create_rgb(r, g, b) : color;
}

RGB_LIGHT * parse_light(char * line) {
    char * position;
    char * red_conn, * green_conn, * blue_conn;

    red_conn = get_first_without_tabs_nor_spaces( strtok_r(line, ",", &position) );
    green_conn = get_first_without_tabs_nor_spaces( strtok_r(NULL, ",", &position) );
    blue_conn = get_first_without_tabs_nor_spaces( strtok_r(NULL, ",", &position) );

    logger("(parse_light) Read connections for new light: R:%s, G:%s, B:%s\n", red_conn, green_conn, blue_conn);

    return create_rgb_light(atoi(red_conn), atoi(green_conn), atoi(blue_conn));
}


RGBLightConfiguration * create_rgb_light_configuration() {
    LittleHashMap * colors_map;
    LittleHashMap * lights_map;

    RGBLightConfiguration * configuration;

    //Allocates struct 
    configuration = (RGBLightConfiguration *) malloc(sizeof(configuration[0]));
    if (configuration == NULL) {
        logger("(create_rgb_light_configuration) Could not allocate rgb_configuration\n");
        return NULL;
    }

    //Colors map: color's hash -> color (Avoid duplicating colors)
    colors_map = create_map(colors_map_hash, free, delete_rgb, MAX_COLORS);
    if (colors_map == NULL) {
        logger("(create_rgb_light_configuration) Could not allocate colors_map");
        free_module_configuration(configuration);
        return NULL;
    }

    //Light colors map: light -> map(note -> color)
    lights_map = create_map(lights_map_hash, delete_rgb_light, free_map, MAX_LIGHTS);
    if (lights_map == NULL) {
        logger("(create_rgb_light_configuration) Could not allocate lights_map");
        free_module_configuration(configuration);
        return NULL;
    }

    configuration -> colors_map = colors_map;
    configuration -> lights_map = lights_map;

    return configuration;
}

/*
 * Frees allocated memory for configuration object.
 * @param configuration: The configuration object to free
*/ 
void free_module_configuration(void * pt_configuration) {
    RGBLightConfiguration * configuration = (RGBLightConfiguration *) pt_configuration;

    logger("FREEING MODULE\n");

    if (configuration == NULL) return;
    
    if (configuration -> colors_map != NULL) {
        free_map(configuration -> colors_map);
        configuration -> colors_map = NULL;
    } 

    if (configuration -> lights_map != NULL) {
        free_map(configuration -> lights_map);
        configuration -> lights_map = NULL;
    }
    
    free(configuration);
    configuration = NULL;
}


void * load_module_configuration() {
    char * line;
    char * lines;
    char * value;
    char * keyword;
    char * configuration_text;

    RGB * color = NULL;
    RGB_LIGHT * light = NULL;
    RGBLightConfiguration * conf = NULL; 

    int found_light = 0;

    configuration_text = read_from_file(MODULE_CONFIGURATION_FILE);
    if (configuration_text == NULL) {
        logger("(load_rgb_light_configuration) Could not read RGBLightConfiguration file\n");
        return NULL;
    }

    logger("(load_rgb_light_configuration) RGBLightConfiguration text:\n%s\n", configuration_text);

    conf = create_rgb_light_configuration();

    //Reads line by line
    line = strtok_r(configuration_text, "\n", &lines);

    //Fill configuration values until we finish files or until are fields are completed
    while (line != NULL) {
        
        //Gets value and sets to which field refers
        value = parse_line(line, &keyword);

        if (keyword != NULL && value != NULL) {

            if (is_light_keyword(keyword) && found_light == 0) {
                logger("(load_rgb_light_configuration) New LIGHT keyword found\n");

                light = NULL;
                found_light = 1;
            }
            else if (is_connection_keyword(keyword) && found_light == 1 && light == NULL) {
                logger("(load_rgb_light_configuration) Setting light connection\n");

                found_light = 0;
                light = parse_light(value);
                add_rgb_light(conf, light);
            }
            else if (atoi(keyword) > 0 && light != NULL) {
                logger("(load_rgb_light_configuration) Adding colors\n");

                color = parse_color(conf, value);
                add_rgb_color(conf, light, atoi(keyword), color);
            }
            else {
                logger("(load_rgb_light_configuration) Configuration file error: Found %s, found_light flag = %d, light: %s\n", line, found_light, (light == NULL) ? "NULL" : "NOT NULL");

                free_module_configuration(conf);
                return NULL;
            }
        } 

        line = strtok_r(NULL, "\n", &lines);
    }

    free(lines);
    free(configuration_text);

    return conf;

}

RGB_LIGHT ** get_lights(RGBLightConfiguration * configuration) {
   return (RGB_LIGHT **) map_keys(configuration -> lights_map);
}

RGB * get_color(RGBLightConfiguration * configuration, RGB_LIGHT * light, int note) {
    if (configuration == NULL || light == NULL) { return NULL; }
    LittleHashMap * colors_per_light = (LittleHashMap *) map_get(configuration -> lights_map, light);
    return (RGB *) map_get(colors_per_light, &note);
}