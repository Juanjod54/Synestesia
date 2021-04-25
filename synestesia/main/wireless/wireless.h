/**
 *  Created on 04-25-2021 by Juan Jose Daza Linares
 *  Filename: wireless.h 
 **/

#ifndef WIRELESS_h
#define WIRELESS_h

#include "Arduino.h"
#include "configuration.h"

/**
 * Sets board as an access_point, by setting given configuration ssid and password
 * @param configuration: Configuration object
*/
void create_access_point(Configuration * configuration);

/**
 * Ends access point configuration
*/
void end_acces_point();

#endif