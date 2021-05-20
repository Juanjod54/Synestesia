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
#define GLOBAL_CONFIGURATION_FILE "/global.syn"

/* Configuration keywords */
#define SSID_KEYWORD "SSID"
#define PASSWORD_KEYWORD "PSWD"
#define ADMIN_PASSWORD_KEYWORD "ADM"

#define COMMENT '#'
#define DELIMITER_CHARACTER '$'

/* Number of fields in global parsing */
#define MOCK_SIZE 16

/* Number of configuration fields */
#define CONFIGURATION_FIELDS 3

/* Defaults values are loaded if any error occurred */
#define SSID_DEFAULT "Synestesia"
#define PSWD_DEFAULT NULL
#define ADMIN_PASSWORD_DEFAULT "admin"


struct _Configuration {
    char * ssid;
    char * password;
    char * admin_password;
    void * module;
    module_configuration_load load_module;
    module_configuration_save save_module;
    module_configuration_marshal marshal_module;
    module_configuration_unmarshal unmarshal_module;
    module_configuration_free free_module;
};

/****  PRIVATE METHODS ****/

/*
 * Checks if keyword is known as a configuration keyword
 * If so, non-zero value is returned.
 * Otherwise it returns 0
 * 
 * @param: Found keyword
*/
int is_known_field(char * keyword) {
    if (keyword == NULL) return 0;

    if (strcmp(keyword, SSID_KEYWORD) == 0) return 1;
    if (strcmp(keyword, PASSWORD_KEYWORD) == 0) return 1;
    if (strcmp(keyword, ADMIN_PASSWORD_KEYWORD) == 0) return 1;
    
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
        logger("(create_configuration) Could not allocate configuration\n");
        return NULL; 
    }

    configuration -> ssid = NULL;
    configuration -> password = NULL;
    configuration -> admin_password = NULL;
    configuration -> module = NULL;

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
    logger("(fill_configuration) Setting %s... Parsed Fields: %d\n", key, (*parsed_fields));

    //If there is any error during configuration object creation it returns
    if (configuration == NULL) { 
        logger("(fill_configuration) Configuration is NULL");
        return NULL; 
    }

    /*** Keyword must exists and it must not be already defined (it would be duplicated) ***/

    if (strcmp(key, SSID_KEYWORD) == 0 && configuration -> ssid == NULL) {
        if (set_ssid(configuration, value)) (* parsed_fields)++; //Adds one to field count
    } 
    else if (strcmp(key, PASSWORD_KEYWORD) == 0  && configuration -> password == NULL) {
        if (set_password(configuration, value)) (* parsed_fields)++; //Adds one to field count  
    } 
    else if (strcmp(key, ADMIN_PASSWORD_KEYWORD) == 0  && configuration -> admin_password == NULL) {
        if (set_admin_password(configuration, value)) (* parsed_fields)++; //Adds one to field count  
    } 

    else {return NULL; }

    return configuration;
}

/*
 * Parses configuration text into a new Configuration object.
 * If all required fields were found and valid, it returns a new Configuration object with read configuration
 * If there is any missing field or there is any error it returns NULL
 * 
 * @param configuration_text: Read text from configuration file
*/ 
Configuration * parse_configuration(char * configuration_text, char * delimiter) {
    char * line;
    char * lines;
    char * value;
    char * keyword;

    int parsed_fields = 0;

    Configuration * configuration;
    
    if (configuration_text == NULL) { return NULL; }

    configuration = create_configuration();
    if (configuration == NULL) {
        logger("(parse_configuration) Could not allocate configuration\n");
        return NULL;
    }

    logger("(parse_configuration) Configuration text:\n%s\n\n", configuration_text);

    //Reads line by line
    line = strtok_r(configuration_text, delimiter, &lines);

    //Fill configuration values until we finish files or until are fields are completed
    while (line != NULL && parsed_fields != CONFIGURATION_FIELDS) {
        
        //Gets value and sets to which field refers
        value = parse_line(line, &keyword);

        //Checks if field is known
        if (is_known_field(keyword)) {
            //Sets read field and manages parsed_fields count
            //Exits if there is any error while setting fields
            if (fill_configuration(configuration, keyword, value, &parsed_fields) == NULL) { break; } 
        } else logger("(parse_configuration) Unknown field %s\n", (keyword == NULL) ? "NULL" : keyword);

        line = strtok_r(NULL, delimiter, &lines);
    }

    logger("(parse_configuration) Parsed fields: %d\n", parsed_fields);

    //If configuration text has been read and the number of expected fields is different from parsed ones, it means configuration file has missing fields
    if (parsed_fields != CONFIGURATION_FIELDS) { 
        logger("(parse_configuration) Configuration error: could not find all required fields\n");
        
        //Frees allocated memory and returns
        free_global(configuration);
        return NULL;
     }

     return configuration;
} 

/*
 * Writes GLOBAL configuration object to a text file, separating fields by delimiter character
*/ 
char * global_configuration_to_text(Configuration * configuration, char delimiter) {
    int length;
    char * parsed;
    char * safe_adm;
    char * safe_ssid;
    char * safe_pswd;
    char * empty = ""; 

    if (configuration == NULL) return NULL;

    safe_ssid = (configuration -> ssid == NULL) ? empty : configuration -> ssid;
    safe_pswd = (configuration -> password == NULL) ? empty : configuration -> password;
    safe_adm = (configuration -> admin_password == NULL) ? empty : configuration -> admin_password;

    length = strlen(safe_ssid) + strlen(safe_pswd) + strlen(safe_adm) + MOCK_SIZE;
    parsed = (char *) malloc(sizeof(parsed[0]) * (length + 1));
    if (parsed == NULL) {
        logger("(parse_global_configuration) Could not allocate memory for parsed fields\n");
        return NULL;
    }

    sprintf(parsed, "SSID:%s%cPSWD:%s%cADM:%s", safe_ssid, delimiter, safe_pswd, delimiter, safe_adm);

    return parsed;
}

/*
 * Loads default values to a new configuration object to allow device to startup when there is any error on configuration file
 * If there is any error during defaults loading it returns NULL
 * Otherwise it returns a configuration object with default configuration
*/ 
Configuration * load_defaults() {
    Configuration * configuration = create_configuration();
    if (configuration == NULL) return NULL;
    
    set_ssid(configuration, SSID_DEFAULT);
    set_password(configuration, PSWD_DEFAULT);
    set_admin_password(configuration, ADMIN_PASSWORD_DEFAULT);
    
    return configuration;
}

/****  PUBLIC METHODS ****/

/****************** GLOBAL *********************/

/*
 * Loads configuration file into a new Configuration object. Loads file manager.
 * If all required fields were found and valid, it returns a new Configuration object with read configuration
 * If there is any missing field or there is any error it returns NULL
*/ 
Configuration * load_configuration() {
    return load_configuration_and_module(NULL, NULL, NULL, NULL, NULL);
}

/*
 * Loads configuration file into a new Configuration object. Loads file manager.
 * It uses load, save, marshal, unmarshal and free functions, if they are NOT set to NULL, to load module configuration
 * If all required fields were found and valid, it returns a new Configuration object with read configuration
 * If there is any missing field or there is any error it loads default values.
 * If default values could not be loaded it returns NULL
*/ 
Configuration * load_configuration_and_module(module_configuration_load load_fn, 
                                              module_configuration_save save_fn, 
                                              module_configuration_marshal marshal_fn, 
                                              module_configuration_unmarshal unmarshal_fn, 
                                              module_configuration_free free_fn) {
                                                  
    char delimiter = '\n';
    char * configuration_text;
    Configuration * configuration;

    init_file_manager();

    configuration_text = read_from_file(GLOBAL_CONFIGURATION_FILE);
    if (configuration_text == NULL) {
        logger("(load_configuration_and_module) Could not read configuration file. Defaults will be loaded\n");
        configuration = load_defaults();
    }
    else {
        logger("(load_configuration_and_module) Configuration file has been loaded with no errors\n");

        //Parse configuration file and create Configuration object
        configuration = parse_configuration(configuration_text, &delimiter);

        //Free configuration text
        free(configuration_text);

        if (configuration == NULL) { configuration = load_defaults(); }
    }

    /*** Module load START ****/

    if (load_fn != NULL && save_fn != NULL && marshal_fn != NULL && 
        unmarshal_fn != NULL && free_fn != NULL && configuration != NULL) {

        logger("(load_configuration_and_module) Loading component\n");
        //Assign function pointers
        configuration -> load_module = load_fn;
        configuration -> save_module = save_fn;
        configuration -> marshal_module = marshal_fn;
        configuration -> unmarshal_module = unmarshal_fn;
        configuration -> free_module = free_fn;

        //Load module configuration
        configuration -> module = (configuration -> load_module)();
    }
    /*** Module load END ****/

    return configuration;
}

/*
 * Saves global and module configuration objects into disk
*/
int save_configuration(Configuration * configuration) {
    int status;
    
    status = save_global_configuration(configuration);

    //Saves module configuration if exists
    if (configuration -> module != NULL) {
        //If module save returns 0 flag will be set to 0
        status *= configuration -> save_module(configuration -> module);
    }

    if (! status) logger("(save_configuration) Could not save configuration\n");

    return status;
}

int save_global_configuration(Configuration * configuration) {
    char * conf_text = global_configuration_to_text(configuration, '\n');

    if (conf_text != NULL) {
        if (write_to_file(GLOBAL_CONFIGURATION_FILE, conf_text) > 0) {
            free(conf_text);
            return 1; 
        }
    }

    return 0;
}

/*
 * Frees allocated memory for configuration object.
 * @param configuration: The configuration object to free
*/ 
int free_configuration(Configuration * configuration) {    
    int ret;
    ret = free_module(configuration);
    ret *= free_global(configuration);
    return ret;
}

/*
 * Frees allocated memory for global configuration object.
 * @param configuration: The configuration object to free
*/ 
int free_global(Configuration * configuration) {
    if (!configuration) return 0;

    if (configuration -> ssid) {
        free(configuration -> ssid);
        configuration -> ssid = NULL;
    }

    if (configuration -> password) {
        free(configuration -> password);
        configuration -> password = NULL;
    }

    configuration -> module = NULL;
    configuration -> load_module = NULL;
    configuration -> save_module = NULL;
    configuration -> marshal_module = NULL;
    configuration -> unmarshal_module = NULL;
    configuration -> free_module = NULL;

    free(configuration);
    configuration = NULL;

    return 1;
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
 * If ssid or configuration are NULL it returns 0
 * If ssid length is greater than 12 characters it returns 0
 * Otherwise it returns 1
 * @param configuration: The configuration object to modify
 * @param ssid: The SSID value
*/
int set_ssid(Configuration * configuration, char * ssid) {
    int length;
    if (configuration == NULL || ssid == NULL) return 0;

    length = strlen(ssid);

    if (length > 12) { 
        logger("(set_ssid) SSID length must be 12 digits or less\n"); 
        configuration -> ssid = NULL;
    }
    else {

        //Allocate memory for ssid
        configuration -> ssid = (char *) malloc((sizeof(configuration -> ssid[0]) * length) + 1);
        if (configuration -> ssid == NULL) { logger("(set_ssid) Could not allocate memory for Configuration SSID\n"); }
        else {
            //Copy value
            strcpy(configuration -> ssid, ssid);
            logger("(set_ssid) SSID has been set\n");
            return 1;
        }
    }

    return 0;
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
 * If password or configuration are NULL it returns 0
 * If password length is not equals to 8 it returns 0
 * Otherwise it returns 1
 * @param configuration: The configuration object to modify
 * @param password: The PASSWORD value
*/
int set_password(Configuration * configuration, char * password) {
    int length;
    
    if (configuration == NULL) return 0;
    else if (password == NULL) return 1; //Password is optional

    length = strlen(password);
    
    if (length != 8) { 
        logger("(set_password) Password length must be 8 digits, current length is %d\n", length); 
        configuration -> password = NULL;
    }
    else {
        
         //Allocate memory for password
        configuration -> password = (char * ) malloc((sizeof(configuration -> password[0]) * length) + 1);    
        if (configuration -> password == NULL) { logger("(set_password) Could not allocate memory for Configuration PASSWORD\n"); }
        else {
            //Copy value
            strcpy(configuration -> password, password);
            logger("(set_password) Password has been set\n");
            return 1;
        }
    }

    return 0;
}

/*
 * Gets the ADMIN PASSWORD value or NULL if configuration is NULL
 * @param configuration: The configuration object with the wanted ADMIN PASSWORD
*/ 
char * get_admin_password(Configuration * configuration) {
    return (configuration == NULL) ? NULL : configuration -> admin_password;
}

/*
 * Sets a ADMIN PASSWORD value to a configuration. 
 * If admin password or configuration are NULL it returns 0
 * If admin password length is greater than 12 characters it returns 0
 * Otherwise it returns 1
 * @param configuration: The configuration object to modify
 * @param password: The ADMIN PASSWORD value
*/
int set_admin_password(Configuration * configuration, char * admin_password) {
    int length;
    
    if (configuration == NULL || admin_password == NULL) return 0;

    length = strlen(admin_password);
    
    if (length > 12) { 
        logger("(set_admin_password) ADMIN PASSWORD length must be 12 digits or less\n"); 
        configuration -> admin_password = NULL;
    }
    else {

        configuration -> admin_password = (char * ) malloc((sizeof(configuration -> admin_password[0]) * length) + 1);    
        if (configuration -> admin_password == NULL) { logger("(set_admin_password) Could not allocate memory for Configuration ADMIN PASSWORD\n");}
        else {
            //Copy value
            strcpy(configuration -> admin_password, admin_password);
            logger("(set_admin_password) ADMIN PASSWORD has been set\n");
            return 1;
        }
    }

    return 0;
}

/*
 * Saves a configuration object into a text file, delimiting fields by ';'
*/
char * marshal_global_configuration(Configuration * configuration) {
    return global_configuration_to_text(configuration, DELIMITER_CHARACTER);
}

/*
 * Loads global configuration from a text file, in which fields are delimited by 
*/
Configuration * unmarshal_global_configuration(char * configuration_text) {
    char delimiter = DELIMITER_CHARACTER;
    return parse_configuration(configuration_text, &delimiter);
}

/*
 * References module's pointers into a new configuration object
*/
void update_global_configuration_module_references(Configuration * old_configuration, Configuration * new_configuration) {
    if (old_configuration == NULL || new_configuration == NULL) return;

    //Updates references
    new_configuration -> module = old_configuration -> module;
    new_configuration -> load_module = old_configuration -> load_module;
    new_configuration -> save_module = old_configuration -> save_module;
    new_configuration -> marshal_module = old_configuration -> marshal_module;
    new_configuration -> unmarshal_module = old_configuration -> unmarshal_module;
    new_configuration -> free_module = old_configuration -> free_module;

}

/****************** Module *********************/

/*
 * Returns loaded module configuration object, if any.
 * If configuration object is NULL it returns NULL
*/ 
void * get_module_configuration(Configuration * configuration) {
    return (configuration == NULL) ? NULL : configuration -> module;
}

int save_module_configuration(Configuration * configuration) {
    return (configuration == NULL) ? -1 : configuration -> save_module(configuration -> module);
}

/*
 * Frees allocated memory for module configuration object.
 * @param configuration: The configuration object which has the module to free
*/ 
int free_module(Configuration * configuration) {
    if (!configuration) return 0;

    if (configuration -> module) {
        int ret = configuration -> free_module (configuration -> module);

        configuration -> module = NULL;
        configuration -> load_module = NULL;
        configuration -> save_module = NULL;
        configuration -> marshal_module = NULL;
        configuration -> unmarshal_module = NULL;
        configuration -> free_module = NULL;
        
        return ret;
    }

    return 1;
}

/*
 * Saves a module configuration object into a text file
*/
char * marshal_module_configuration(Configuration * configuration) {
    if (configuration == NULL) return NULL;
    return configuration -> marshal_module(configuration -> module);
}

/*
 * Saves a module configuration object into a text file
*/
Configuration * unmarshal_module_configuration(Configuration * configuration, char * configuration_text) {
    if (configuration == NULL || configuration_text == NULL || !(configuration -> unmarshal_module)) return NULL;

    void * module = (configuration -> unmarshal_module) (configuration_text);

    if (module == NULL) { return NULL; }
    
    configuration -> module = module;
    return configuration;

}