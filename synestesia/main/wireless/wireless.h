/**
 *  Created on 04-25-2021 by Juan Jose Daza Linares
 *  Filename: wireless.h 
 **/

#ifndef WIRELESS_h
#define WIRELESS_h

#include "Arduino.h"
#include "configuration.h"

typedef struct _Wireless Wireless;

/**
 * Sets board as an access_point, by setting given configuration ssid and password
 * @param configuration: Configuration object
*/
void create_access_point(Configuration * configuration);

Wireless init_wireless(Configuration * configuration);

/**
 * Ends access point configuration
*/
void end_acces_point();

#endif