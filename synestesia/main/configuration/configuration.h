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
 * Saves a module_configuration object into a text file 
*/
typedef int (*module_configuration_save)(void *);

/*
 * Saves a module_configuration object into a text file
*/
typedef char * (*module_configuration_marshall)(void *);

/*
 * Loads a module configuration object from a string
 * First param contains marshalled data
 * Second param its the delimiter string
*/
typedef void * (*module_configuration_unmarshall)(char *);

/*
 * Deallocates module_configuration object
 * @param: module_configuration object defined by the user 
*/
typedef void (*module_configuration_free)(void*);


/*Configuration object*/
typedef struct _Configuration Configuration;

/****************** GLOBAL *********************/

/*
 * Loads configuration file into a new Configuration object. Loads file manager.
 * If all required fields were found and valid, it returns a new Configuration object with read configuration
 * If there is any missing field or there is any error it returns NULL
*/ 
Configuration * load_configuration();

/*
 * Loads configuration file into a new Configuration object. Loads file manager.
 * It uses load, save, marshall, unmarshall and free functions, if they are NOT set to NULL, to load module configuration
 * If all required fields were found and valid, it returns a new Configuration object with read configuration
 * If there is any missing field or there is any error it loads default values.
 * If default values could not be loaded it returns NULL
*/ 
Configuration * load_configuration_and_module(module_configuration_load load_fn, 
                                              module_configuration_save save_fn, 
                                              module_configuration_marshall marshall_fn, 
                                              module_configuration_unmarshall unmarshall_fn, 
                                              module_configuration_free free_fn);

/*
 * Saves global configuration object into disk
*/
int save_configuration(Configuration * configuration);

int save_global_configuration(Configuration * configuration);

/*
 * Frees allocated memory for global and module configuration objects.
 * @param configuration: The configuration object to free
*/ 
void free_configuration(Configuration * configuration);

/*
 * Frees allocated memory for global configuration object.
 * @param configuration: The configuration object to free
*/ 
void free_global(Configuration * configuration);

/*
 * Gets the SSID value or NULL if configuration is NULL
 * @param configuration: The configuration object with the wanted SSID
*/ 
char * get_ssid(Configuration * configuration);

/*
 * Sets a SSID value to a configuration. 
 * If ssid or configuration are NULL it returns 0
 * If ssid length is greater than 12 characters it returns 0
 * Otherwise it returns 1
 * @param configuration: The configuration object to modify
 * @param ssid: The SSID value
*/
int set_ssid(Configuration * configuration, char * ssid);

/*
 * Gets the PASSWORD value or NULL if configuration is NULL
 * @param configuration: The configuration object with the wanted PASSWORD
*/ 
char * get_password(Configuration * configuration);

/*
 * Sets a PASSWORD value to a configuration. 
 * If password or configuration are NULL it returns 0
 * If password length is not equals to 8 it returns 0
 * Otherwise it returns 1
 * @param configuration: The configuration object to modify
 * @param password: The PASSWORD value
*/
int set_password(Configuration * configuration, char * password);

/*
 * Gets the ADMIN PASSWORD value or NULL if configuration is NULL
 * @param configuration: The configuration object with the wanted ADMIN PASSWORD
*/ 
char * get_admin_password(Configuration * configuration);

/*
 * Sets a ADMIN PASSWORD value to a configuration. 
 * If admin password or configuration are NULL it returns 0
 * If admin password length is greater than 12 characters it returns 0
 * Otherwise it returns 1
 * @param configuration: The configuration object to modify
 * @param password: The ADMIN PASSWORD value
*/
int set_admin_password(Configuration * configuration, char * admin_password);

/*
 * Saves a configuration object into a text file, delimiting fields by ';'
*/
char * marshall_global_configuration(Configuration * configuration);

/*
 * Loads global configuration from a text file, in which fields are delimited by 
*/
Configuration * unmarshall_global_configuration(char * configuration_text);

/*
 * References module's pointers into a new configuration object
*/
void update_global_configuration_module_references(Configuration * old_configuration, Configuration * new_configuration);

/****************** Module *********************/

/*
 * Returns loaded module configuration object, if any.
 * If configuration object is NULL it returns NULL
*/ 
void * get_module_configuration(Configuration * configuration);

int save_module_configuration(Configuration * configuration);

/*
 * Frees allocated memory for module configuration object.
 * @param configuration: The configuration object which has the module to free
*/ 
void free_module(Configuration * configuration);

/*
 * Saves a module configuration object into a text file
*/
char * marshall_module_configuration(Configuration * configuration);

/*
 * Loads module configuration from a text file 
*/
Configuration * unmarshall_module_configuration(Configuration * configuration, char * configuration_text);

#endif