/**
 *  Created on 04-14-2021 by Juan Jose Daza Linares
 *  Filename: configuration.cpp 
 **/

#include <string.h>

#include "configuration.h"
#include "file_manager.h"

/** Logger **/
#ifdef DEBUG_ESP_PORT
#define logger(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define logger(...)
#endif

/* Configuration file */
#define CONFIGURATION_FILE "/global.syn"

/* Configuration keywords */
#define SSID_KEYWORD "SSID";
#define PASSWORD_KEYWORD "PSWD";

/* Configuration delimiters */
#define LINE_DELIMITER '\n'
#define VALUE_DELIMITER ':'
#define COMMENT '#'

/* Number of configuration fields */
#define CONFIGURATION_FIELDS 2


struct _Configuration {
    char * ssid;
    char * password;
};

/****  PRIVATE METHODS ****/

/*
 * TODO: REMOVE SPACES 


 * Parses a configuration file line in "keyword : value" format
 * If read line starts with comment symbol (#) it returns NULL
 * If read line is null it returns NULL
 * Otherwise it returns read value and sets keyword param to found keyword 
 * 
 * @param line: Read line
 * @param keyword: Pointer in which found keyword will be set
*/
char * parse_line(char * line, char* keyword) {
    char * value;
    
    //Point to NULL to avoid dirty memory
    keyword = NULL;

    if (line == NULL) { return NULL; }

    keyword = strtok(line, VALUE_DELIMITER);
    value = strtok(line, VALUE_DELIMITER);

    //If could not split line or it is a comment we'll return
    if (keyword == NULL || strcmp(key[0], COMMENT) == 0) { 
        keyword = NULL; 
        return NULL; 
    }
    
    return value;
}

/*
 * Checks if keyword is known as a configuration keyword
 * If so, non-zero value is returned.
 * Otherwise it returns 0
 * 
 * @param: Found keyword
*/
int is_known_field(char * keyword) {
    if (keyword == NULL) return 0;
    
    if (strcmp(keyword, SSID_KEYWORD)) return 1;
    if (strcmp(keyword, PASSWORD_KEYWORD)) return 1;
    
    return 0;
}

/*
 * Allocate memory for a Configuration object and sets its field to NULL
 * Returns a new configuration object.
 * If memory allocation failed it reurns NULL value
 */
Configuration * create_configuration() {
    Configuration * configuration;
    configuration = (Configuration *) malloc(sizeof(configuration[0]));
    
    if (configuration == NULL) { 
        logger("Could not allocate configuration\n");
        return NULL; 
    }

    configuration -> ssid = NULL;
    configuration -> password = NULL;

    return configuration;
}

/*
 * Creates a new configuration file (if it is not already created) and sets the configuration values based on 
 * the given keyword. 
 * If value is set it adds one to parsed_fields.
 * If value has already been added it returns without setting it
 * 
 * @param configuration: Configuration object
 * @param key: Found keyword
 * @param value: Read value
 * @param parsed_fields: Number of parsed values from configuration file
*/
void * fill_configuration(Configuration * configuration, char * key, char * value, int * parsed_fields) {
    //If configuration is null (not instantiated yet) it creates a new configuration object
    if (configuration == NULL) { configuration = create_configuration()}

    //If there is any error during configuration object creation it returns
    if (configuration == NULL) { return NULL; }

    /*** Keyword must exists and it must not be already defined (it would be duplicated) ***/

    if (strcmp(key, SSID_KEYWORD) == 0 && configuration -> ssid == NULL) {
        (parsed_fields *)++ //Adds one to field count  
        return set_ssid(configuration, value);
    } 
    else if (strcmp(key, PASSWORD_KEYWORD) == 0  && configuration -> password == NULL) {
        (parsed_fields *)++ //Adds one to field count  
        return set_password(configuration, value);
    } 

    return NULL;
}

/*
 * Parses configuration text into a new Configuration object.
 * If all required fields were found and valid, it returns a new Configuration object with read configuration
 * If there is any missing field or there is any error it returns NULL
 * 
 * @param configuration_text: Read text from configuration file
*/ 
Configuration * parse_configuration(char * configuration_text) {
    char * line;
    char * value;
    char * keyword;

    int parsed_fields = 0;

    Configuration * configuration = NULL;
    
    if (configuration_text == NULL) { return NULL; }

    //Reads line by line
    line = strtok(configuration_text, LINE_DELIMITER);

    //Fill configuration values until we finish files or until are fields are completed
    while (line != NULL || parsed_fields == CONFIGURATION_FIELDS) {
        
        //Gets value and sets to which field refers
        value = parse_line(line, keyword);

        //Checks if field is known
        if (is_known_field(keyword)) {
            
            //Sets read field and manages parsed_fields count
            //Exits if there is any error while setting fields
            if (fill_configuration(configuration, keyword, value, &parsed_fields) == NULL) break;
        }
    }

    //If configuration text has been read and the number of expected fields is different from parsed ones, it means configuration file has missing fields
    if (parsed_fields != CONFIGURATION_FIELDS) { 
        logger("Configuration error: could not find all required fields\n");
        
        //Frees allocated memory and returns
        free_configuration(configuration);
        return NULL;
     }

     return configuration;
} 

/****  PUBLIC METHODS ****/

/*
 * Loads configuration file into a new Configuration object.
 * If all required fields were found and valid, it returns a new Configuration object with read configuration
 * If there is any missing field or there is any error it returns NULL
*/ 
Configuration * load_configuration() {
    Configuration * configuration;
    char * configuration_text = read_from_file(CONFIGURATION_FILE);
    if (configuration_text == NULL) {
        logger("Could not read configuration file\n");
        return NULL;
    }
    
    logger("Configuration file has been loaded with no errors\n");

    //Parse configuration file and create Configuration object
    configuration = parse_configuration(configuration);
    
    //Check if configuration has loaded SSID
    if (configuration -> ssid != NULL) return configuration;

    logger("SSID is NULL\n");
    free(configuration);
    return NULL;
}

/*
 * Frees allocated memory for configuration object
 * @param configuration: The configuration object to free
*/ 
void free_configuration(Configuration * configuration) {
    if (configuration == NULL) return;

    if (configuration -> ssid != NULL) free(configuration -> ssid);
    if (configuration -> password != NULL) free(configuration -> password);
    free(configuration);
}

/*
 * Gets the SSID value or NULL if configuration is NULL
 * @param configuration: The configuration object with the wanted SSID
*/ 
char * get_ssid(Configuration * configuration) {
    return (configuration == NULL) ? NULL : configuration -> ssid;
}

/*
 * Sets a SSID value to a configuration. 
 * If ssid or configuration are NULL it returns NULL
 * If ssid length is lower than 8 it returns NULL
 * Otherwise it return the configuration file with applied ssid  
 * @param configuration: The configuration object to modify
 * @param ssid: The SSID value
*/
void * set_ssid(Configuration * configuration, char * ssid) {
    int length;
    if (configuration == NULL || ssid == NULL) return NULL;

    length = strlen(ssid);

    if (srlen < 8) { logger("SSID length must be 8 digits or more\n"); }
    else {
        //Allocate memory for ssid
        configuration -> ssid = (char *) malloc((sizeof(configuration -> ssid[0]) * length) + 1);
        if (configuration -> ssid == NULL) { logger("Could not allocate memory for Configuration SSID\n"); }
        else {
            //Add str delimiter
            configuration -> ssid [length] = '\0';
            //Copy value
            strcpy(configuration -> ssid, ssid);

            return configuration;
        }
    }

    return NULL;
}

/*
 * Gets the PASSWORD value or NULL if configuration is NULL
 * @param configuration: The configuration object with the wanted PASSWORD
*/ 
char * get_password(Configuration * configuration) {
    return (configuration == NULL) ? NULL : configuration -> password;
}

/*
 * Sets a PASSWORD value to a configuration. 
 * If password or configuration are NULL it returns NULL
 * Otherwise it return the configuration file with applied password  
 * @param configuration: The configuration object to modify
 * @param password: The PASSWORD value
*/
void set_password(Configuration * configuration, char * password) {
    int length;
    if (configuration == NULL) return NULL;
    //Password is optional
    else if (password == NULL) return configuration;

    length = strlen(password);

    //Allocate memory for 
    configuration -> password = (char *) malloc((sizeof(configuration -> password[0]) * length) + 1);
    if (configuration -> password == NULL) { logger("Could not allocate memory for Configuration PASSWORD\n"); }
    else {
        //Add str delimiter
        configuration -> password [length] = '\0';
        //Copy value
        strcpy(configuration -> password, password);

        return configuration:
    }

    return NULL;
}