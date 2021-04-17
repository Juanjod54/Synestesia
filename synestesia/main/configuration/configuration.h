/**
 *  Created on 04-14-2021 by Juan Jose Daza Linares
 *  Filename: configuration.h 
 **/

#ifndef CONFIGURATION_h
#define CONFIGURATION_h

#include "Arduino.h"

typedef struct _Configuration Configuration;

/*
 * Loads configuration file into a new Configuration object.
 * If all required fields were found and valid, it returns a new Configuration object with read configuration
 * If there is any missing field or there is any error it returns NULL
*/ 
Configuration * load_configuration();

/*
 * Frees allocated memory for configuration object
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
void * set_ssid(Configuration * configuration, char * ssid);

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

#endif