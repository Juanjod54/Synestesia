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

#define DNS_PORT 53

struct {
    DNSServer dns_server;
    WiFiServer configuration_server;
} _Wireless;

DNSServer set_captive_portal(char * domain_name) {
    DNSServer dns_server;
    dns_server.start(DNS_PORT, domain_name, WiFi.localIP());
    return dns_server;
}

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

Wireless init_wireless(Configuration * configuration) {
    
    Wireless * wireless;

    if (configuration == NULL) return NULL;

    wireless = (Wireles **) malloc(sizeof(wireless[0]));
    if (wireless == NULL) {
        logger("(init_wireless) Could not allocate wireless object\n");
        return NULL;
    }

    create_access_point(configuration);

    //Create web server
    (wireless -> configuration_server)(80);
    //Create dns server
    wireless -> dns_server = set_captive_portal(get_domain(configuration));

    return wireless;
}

void handle_client() {
    dnsServer.processNextRequest();
    webServer.handleClient();
}

/**
 * Ends access point configuration
*/
void end_acces_point() {
    WiFi.softAPdisconnect(true);
}

