/**
 *  Created on 04-14-2021 by Juan Jose Daza Linares
 *  Filename: configuration.h 
 **/

#ifndef CONFIGURATION_h
#define CONFIGURATION_h

#include "Arduino.h"

/*
 * Returns a module_configuration object defined by the user 
*/
typedef void * (*module_configuration_load)();

/*
 * Deallocates module_configuration object
 * @param: module_configuration object defined by the user 
*/
typedef void (*module_configuration_free)(void*);


/*Configuration object*/
typedef struct _Configuration Configuration;

/*
 * Loads configuration file into a new Configuration object. Loads file manager.
 * If all required fields were found and valid, it returns a new Configuration object with read configuration
 * If there is any missing field or there is any error it returns NULL
*/ 
Configuration * load_configuration();

/*
 * Loads configuration file into a new Configuration object. Loads file manager.
 * It uses load and free functions, if they are NOT set to NULL, to load module configuration
 * If all required fields were found and valid, it returns a new Configuration object with read configuration
 * If there is any missing field or there is any error it returns NULL
*/ 
Configuration * load_configuration_and_module(module_configuration_load load, module_configuration_free free);

/*
 * Frees allocated memory for configuration object. Finishes file manager.
 * @param configuration: The configuration object to free
*/ 
void free_configuration(Configuration * configuration);

/*
 * Gets the SSID value or NULL if configuration is NULL
 * @param configuration: The configuration object with the wanted SSID
*/ 
char * get_ssid(Configuration * configuration);

/*
 * Sets a SSID value to a configuration. 
 * If ssid or configuration are NULL it returns NULL
 * If ssid length is lower than 8 it returns NULL
 * Otherwise it return the configuration file with applied ssid  
 * @param configuration: The configuration object to modify
 * @param ssid: The SSID value
*/
void set_ssid(Configuration * configuration, char * ssid);

/*
 * Gets the PASSWORD value or NULL if configuration is NULL
 * @param configuration: The configuration object with the wanted PASSWORD
*/ 
char * get_password(Configuration * configuration);

/*
 * Sets a PASSWORD value to a configuration. 
 * If password or configuration are NULL it returns NULL
 * Otherwise it return the configuration file with applied password  
 * @param configuration: The configuration object to modify
 * @param password: The PASSWORD value
*/
void set_password(Configuration * configuration, char * password);

/*
 * Returns loaded module configuration object, if any.
 * If configuration object is NULL it returns NULL
*/ 
void * get_module_configuration(Configuration * configuration);

#endif