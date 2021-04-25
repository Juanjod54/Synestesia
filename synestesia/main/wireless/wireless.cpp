/**
 *  Created on 04-25-2021 by Juan Jose Daza Linares
 *  Filename: wireless.cpp
 **/

#include "wireless.h"

#include <ESP8266WiFi.h>

/** Logger **/
#ifdef DEBUG_ESP_PORT
#define logger(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define logger(...)
#endif

/**
 * Sets board as an access_point, by setting given configuration ssid and password
 * @param configuration: Configuration object
*/
void create_access_point(Configuration * configuration) {
    char * ssid, * password;

    if (configuration == NULL) {
        logger("(create_access_point) Configuration object is NULL\n");
        return;
    }

    WiFi.mode(WIFI_AP);
    
    //If password is NULL AP will be open
    ssid = get_ssid(configuration);
    password = get_password(configuration);
    logger("(create_access_point) Values\n\tSSID: %s\n\t%s\n", ssid, (password == NULL) ? "NULL" : password);
    
    while(! WiFi.softAP(ssid, password)){
        logger(".");
        delay(100);
    }
    
    logger("\n");


}

/**
 * Ends access point configuration
*/
void end_acces_point() {
    WiFi.softAPdisconnect(true);
}