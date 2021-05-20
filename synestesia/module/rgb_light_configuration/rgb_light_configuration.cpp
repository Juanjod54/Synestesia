/**
 *  Created on 04-18-2021 by Juan Jose Daza Linares
 *  Filename: rgb_light_configuration.cpp 
 **/

#include <math.h>
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

#define LIGHT_marshal_STRUCT "%s%s:%s"
#define CONN_marshal_STRUCT "%s%s:%d,%d,%d%s"
#define COLOR_marshal_STRUCT "%s%d:%d,%d,%d%s"
#define LAST_COLOR_marshal_STRUCT "%s%d:%d,%d,%d"


#define MAX_LIGHTS 2
#define MAX_COLORS 14
#define MAX_COLORS_PER_LIGHT 14

#define DELIMITER_CHARACTER '$'

#define BUFFER_LENGTH 512

#define BASE_FREQ 16.35

static char buffer[BUFFER_LENGTH] = {'\0'};

/*Module configuration object*/
struct _RGBLightConfiguration {
    LittleHashMap * lights_map; //LittleHashMaps a light with a LittleHashMap of colors per note
};

//colors_map stores hash -> color, so its hash function will be the hash itself
long colors_map_hash(void * hash) {
    if (hash == NULL) return -1;
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
    
    key_ptr = (int *) malloc(sizeof(key_ptr[0]));
    * key_ptr = key;

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
    char * position = NULL;
    char * red, * green, * blue;
    
    RGB * color;

    red = get_first_without_tabs_nor_spaces( strtok_r(line, ",", &position) );
    green = get_first_without_tabs_nor_spaces( strtok_r(NULL, ",", &position) );
    blue = get_first_without_tabs_nor_spaces( strtok_r(NULL, ",", &position) );

    r = atoi(red);
    g = atoi(green);
    b = atoi(blue);

    return create_rgb(r, g, b);
}

RGB_LIGHT * parse_light(char * line) {
    char * position = NULL;
    char * red_conn, * green_conn, * blue_conn;

    red_conn = get_first_without_tabs_nor_spaces( strtok_r(line, ",", &position) );
    green_conn = get_first_without_tabs_nor_spaces( strtok_r(NULL, ",", &position) );
    blue_conn = get_first_without_tabs_nor_spaces( strtok_r(NULL, ",", &position) );

    return create_rgb_light(atoi(red_conn), atoi(green_conn), atoi(blue_conn));
}


RGBLightConfiguration * create_rgb_light_configuration() {
    LittleHashMap * lights_map;

    RGBLightConfiguration * configuration;

    //Allocates struct 
    configuration = (RGBLightConfiguration *) malloc(sizeof(configuration[0]));
    if (configuration == NULL) {
        logger("(create_rgb_light_configuration) Could not allocate rgb_configuration\n");
        return NULL;
    }

    configuration -> lights_map = NULL;

    //Light colors map: light -> map(note -> color)
    lights_map = create_map(lights_map_hash, delete_rgb_light, free_map, MAX_LIGHTS);
    if (lights_map == NULL) {
        logger("(create_rgb_light_configuration) Could not allocate lights_map");
        free_rgb_light_configuration(configuration);
        return NULL;
    }

    configuration -> lights_map = lights_map;

    return configuration;
}

char * configuration_to_text(RGBLightConfiguration * configuration, char * delimiter) {
    int ** notes;
    char * empty = "";
    char * configuration_text;
    
    RGB * color;
    RGB_LIGHT ** lights;

    LittleHashMap * notes_map;

    int i;
    int length;
    int notes_size;
    int lights_size;
    int notes_index;
    int lights_index;

    if (configuration == NULL) { 
        logger("(configuration_to_text) Configuration is NULL\n");
        return NULL;
    }
    
    lights_size = map_size(configuration -> lights_map);
    
    if (lights_size < 0) { 
        logger("(configuration_to_text) There was an error while fetching lights\n");
        return empty; 
    }
    
    lights = (RGB_LIGHT **) map_keys(configuration -> lights_map);
    if (lights == NULL) {
        logger("(configuration_to_text) Could not fetch lights\n");
        return NULL;
    }

    //Iterate though lights
    for (lights_index = 0; lights_index < lights_size; lights_index++) {
        //Gets notes for light at index

        sprintf(buffer, LIGHT_marshal_STRUCT, buffer, LIGHT_KEYWORD, delimiter);
        sprintf(buffer, CONN_marshal_STRUCT, 
                buffer, CONNECTION_KEYWORD, 
                get_red_connection(lights[lights_index]), 
                get_green_connection(lights[lights_index]), 
                get_blue_connection(lights[lights_index]), 
                delimiter);
        
        notes_map = (LittleHashMap *) map_get(configuration -> lights_map, lights[lights_index]);
        notes_size = map_size(notes_map);

        if (notes_size < 0) {
            logger("(configuration_to_text) There was a problem while fetching notes\n");
            continue; 
        }

        notes = (int **) map_keys(notes_map);
        if (notes == NULL) {
            logger("(configuration_to_text) Could not fetch notes\n");
            free(lights); //Frees pointer to lights array only, not its content
            return NULL;
        }

        for (notes_index = 0; notes_index < notes_size; notes_index++) {

            color = (RGB *) map_get(notes_map, notes[notes_index]);

            if (color != NULL) {

                if (((notes_index + 1) == notes_size) && ((lights_index + 1) == lights_size)) {
                    sprintf(buffer, LAST_COLOR_marshal_STRUCT, 
                            buffer, notes_index + 1, //Notes start at 1 
                            get_red_color(color), 
                            get_green_color(color), 
                            get_blue_color(color), 
                            delimiter);
                }
                else {
                    sprintf(buffer, COLOR_marshal_STRUCT, 
                            buffer, notes_index + 1, //Notes start at 1 
                            get_red_color(color), 
                            get_green_color(color), 
                            get_blue_color(color));
                }
                
            }
            else {
                continue;
            }
        }
        
        //Frees pointer **, not content
        free(notes);

    }

    //Frees pointer **, not content
    free(lights);

    length = strlen(buffer);
    configuration_text = (char *) malloc(sizeof(configuration_text[0]) * length + 1);
    if (configuration_text == NULL) {
        logger("(configuration_to_text) Could not allocate memory for configuration text\n");
        return NULL;
    }

    strcpy(configuration_text, buffer);

    //Clears buffer
    for (i = 0; i < BUFFER_LENGTH; i++) buffer[i] = '\0';

    return configuration_text;
    
}

RGBLightConfiguration * parse_rgb_light_configuration(char * configuration_text, char * delimiter) {
    char * line;
    char * lines;
    char * value;
    char * keyword;

    RGB * color = NULL;
    RGB_LIGHT * light = NULL;
    RGBLightConfiguration * conf = NULL; 

    int found_light = 0;

    if (configuration_text == NULL || delimiter == NULL)  {
        logger("(parse_configuration) Found NULL on input params\n");
        return NULL;
    }

    logger("(parse_configuration) Configuration text:\n%s\n", configuration_text);

    conf = create_rgb_light_configuration();
    
    if (conf != NULL) {

        //Reads line by line
        line = strtok_r(configuration_text, delimiter, &lines);

        //Fill configuration values until we finish files or until are fields are completed
        while (line != NULL) {
        
            //Gets value and sets to which field refers
            value = parse_line(line, &keyword);

            if (keyword != NULL) {

                if (is_light_keyword(keyword) && found_light == 0) {
                    light = NULL;
                    found_light = 1;
                }
                else if (value != NULL) {
                    
                    if (is_connection_keyword(keyword) && found_light == 1 && light == NULL) {
                        found_light = 0;
                        light = parse_light(value);
                        add_rgb_light(conf, light);
                    }
                    else if (atoi(keyword) > 0 && light != NULL) {
                        color = parse_color(conf, value);
                        add_rgb_color(conf, light, atoi(keyword), color);
                    }
                    else {
                        logger("(parse_rgb_light_configuration) Configuration file error: Found %s, found_light flag = %d, light: %s\n", line, found_light, (light == NULL) ? "NULL" : "NOT NULL");

                        free_rgb_light_configuration(conf);
                        break;
                    }
                }
                
            } 

            line = strtok_r(NULL, delimiter, &lines);
        }
    }
    
    return conf;
}

/****  PUBLIC METHODS ****/

/**
 * Loads module configuration by reading it from module configuration file
 * If there is any error during load, it returns NULL
 * Otherwise it returns loaded configuration
 */
void * load_rgb_light_configuration() {
    char * delimiter = "\n";
    char * configuration_text;
    RGBLightConfiguration * conf = NULL; 

    configuration_text = read_from_file(MODULE_CONFIGURATION_FILE);
    if (configuration_text == NULL) {
        logger("(load_rgb_light_configuration) Could not read RGBLightConfiguration file\n");
        return NULL;
    }

    conf = parse_rgb_light_configuration(configuration_text, delimiter); 
    
    free(configuration_text);

    return conf;

}

/*
 * Frees allocated memory for configuration object.
 * @param configuration: The configuration object to free
*/ 
int free_rgb_light_configuration(void * pt_configuration) {
    RGBLightConfiguration * configuration = (RGBLightConfiguration *) pt_configuration;

    if (!configuration) return 0;
    
    if (configuration -> lights_map != NULL) {
        free_map(configuration -> lights_map);
        configuration -> lights_map = NULL;
    }
    
    free(configuration);
    configuration = NULL;

    return 1;
}

int save_rgb_light_configuration(void * pt_configuration) {
    RGBLightConfiguration * configuration = (RGBLightConfiguration *) pt_configuration;
    
    char * conf_text = configuration_to_text(configuration, "\n");

    if (conf_text != NULL) {
        if (write_to_file(MODULE_CONFIGURATION_FILE, conf_text) > 0) {
            free(conf_text);
            return 1;
        }
    }

    return 0;
}

/*
 * Returns a list of the lights stored in configuration
*/
RGB_LIGHT ** get_lights(RGBLightConfiguration * configuration) {
    if (configuration == NULL) return NULL;
    return (RGB_LIGHT **) map_keys(configuration -> lights_map);
}

/*
 * Returns a color for given light and note
 * If no color was found, NULL is returned
*/
RGB * get_color(RGBLightConfiguration * configuration, RGB_LIGHT * light, int * note) {
    if (configuration == NULL || light == NULL || note == NULL) { return NULL; }
    LittleHashMap * colors_per_light = (LittleHashMap *) map_get(configuration -> lights_map, light);
    return (RGB *) map_get(colors_per_light, note);
}

char * marshal_rgb_light_configuration(void * pt_configuration) {
    char delimiter = DELIMITER_CHARACTER;
    RGBLightConfiguration * conf = (RGBLightConfiguration *) pt_configuration;
    return configuration_to_text(conf, &delimiter);
}

void * unmarshal_rgb_light_configuration(char * configuration_text) {
    char delimiter = DELIMITER_CHARACTER;
    return parse_rgb_light_configuration(configuration_text, &delimiter);
}

int get_note (float read_freq, int octaves) {
    //Gets octave (from 0 to 8)
    int current_octave = log2(read_freq / BASE_FREQ); 
    //Gets distance to the base freq (set to Do/C by default)
    int distance_to_base = ((read_freq / (pow(2, current_octave))) - BASE_FREQ) / 2;
    
    return (distance_to_base < 0) ? 0 : ((current_octave % octaves) + 1) * distance_to_base + 1;
}

size_t rgb_light_configuration_size() { return sizeof(RGBLightConfiguration); }