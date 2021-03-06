/**
 *  Created on 04-25-2021 by Juan Jose Daza Linares
 *  Filename: wireless.cpp
 **/

#include "wireless.h"

#ifdef ARDUINO_ESP8266_NODEMCU

#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "postUp.h"
#include "file_manager.h"
#include "configuration.h"


#define DNS_PORT 53

/************* Logger ************/

#ifdef DEBUG_ESP_PORT
#define logger(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define logger(...)
#endif

/*********************************/

/********** Content type *********/

#define CSS "text/css"
#define HTML "text/html"
#define PLAIN "text/plain"
#define JS "text/javascript"

/*********************************/

#define MODULE_HTML "/module.html"

#define OK "OK"
#define ERROR "ERROR"

static int session;
static int AP_SET = 0;
static Synestesia * syn;

DNSServer dns_server;
ESP8266WebServer web_server(80);

/******* IPS ******/

IPAddress apIP(192, 168, 1, 1);

IPAddress apStationIP(192, 168, 1, 10);

IPAddress broadcastIP(192, 168, 1, 255);

/************* DATA HANDLERS *************/

/**
 * Marshals global configuration and returns it to the client
 * If it couldnt marshal global data it returns a 500 error
**/
void get_global_data () {
    Configuration * conf = get_configuration(syn); //just a reference
    char * parsed_data = marshal_global_configuration(conf);
    
    if (parsed_data == NULL) { 
        web_server.send(500, PLAIN, ERROR); 
    }
    else { 
        web_server.send(200, PLAIN, parsed_data); 
        free(parsed_data);
    }    
}

/**
 * Marshals module configuration and returns it to the client
 * If there is no module it returns a 404 error
**/
void get_module_data () {
    Configuration * conf = get_configuration(syn); //just a reference
    char * parsed_data = marshal_module_configuration(conf);

    if (parsed_data == NULL) { 
        web_server.send(404, PLAIN, ERROR); 
    }
    else { 
        web_server.send(200, PLAIN, parsed_data); 
        free(parsed_data);
    }    
}

/**
 * This handler unmarshals global and module (if any) configuration.
 * If both are correct, it persists them.
 */ 
void wireless_save_configuration() {
    char * global_text = NULL;
    char * module_text = NULL;

    Configuration * new_configuration = NULL;
    Configuration * conf = get_configuration(syn);
    
    global_text = (char *) web_server.arg("global").c_str();
    new_configuration = unmarshal_global_configuration(global_text);

    if (get_module_configuration(conf) != NULL) {
        module_text = (char *) web_server.arg("module").c_str();

        //Updates module references
        update_global_configuration_module_references(conf, new_configuration);

        //Unmarshals module
        new_configuration = unmarshal_module_configuration(new_configuration, module_text);
    }

    if (new_configuration != NULL && save_configuration(new_configuration)) {
        
        //Sets new configuration
        set_configuration(syn, new_configuration);

        web_server.send(200, PLAIN, OK);
        return;
    }

    //There was an error
    web_server.send(500, PLAIN, ERROR);
}

void receiver_connect() {
    int attempts;
    char * ssid = (char *) web_server.arg("target-ssid").c_str();
    char * pswd = (char *) web_server.arg("target-password").c_str();

    if (ssid == NULL || ssid == "") {
        logger("(receiver_connect) Could not get SSID\n");
        web_server.send(500, PLAIN, ERROR);
        return;
    }

    if (pswd == "") pswd = NULL;

    logger("(receiver_connect) Connecting to: '%s', with password: '%s'\n", ssid, (pswd == NULL) ? "NULL" : pswd);

    WiFi.begin(ssid, pswd);
    for (attempts = 0; attempts < 100; attempts++) {
        if (WiFi.status() == WL_CONNECTED) { break; }    
        delay(500);
        logger(".");
    }
    
    if (WiFi.status() != WL_CONNECTED ) {
        WiFi.disconnect(true); //Forget last attemts
        logger("(receiver_connect) Failed to connect\n");
        web_server.send(500, PLAIN, ERROR);
    } else {
        web_server.send(200, PLAIN, OK);
    }

}

/************* PRIVATE HANDLERS *************/

/**
 * Resets the board
 **/
void reset () {
    web_server.send(200, PLAIN, OK);
    wdt_disable();
    wdt_enable(WDTO_15MS);
    while (1);
}

/************* PRIVATE METHODS *************/

/**
 * Sets board as an access_point, by setting given configuration ssid and password
*/
void init_master_server() {
    char * ssid;
    char * password;

    Configuration * conf = get_configuration(syn);

    if (conf == NULL) {
        logger("(start_master_server) Configuration object is NULL\n");
        return;
    }

    if (AP_SET != 0) {
        logger("(start_master_server) AP Already started\n");
        return;
    }

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

    //If password is NULL AP will be open
    ssid = get_ssid(conf);
    password = get_password(conf);
    logger("(start_master_server) Values\n\t* SSID: %s\n\t* Password: %s\n", ssid, (password == NULL) ? "NULL" : password);

    while (! WiFi.softAP(ssid, password)) { delay(10); }
    
    //Sets captive portal for any route
    dns_server.start(DNS_PORT, "synestesia.com", apIP);

    AP_SET = 1;
}

//Sets HTTP SERVER
void set_master_http_handlers () {
    web_server.serveStatic("/vue", SPIFFS, "/web/js/vue.min.js");
    web_server.serveStatic("/scripts", SPIFFS, "/web/js/scripts.js");
    web_server.serveStatic("/styles", SPIFFS, "/web/css/bootstrap.min.css");

    web_server.serveStatic("/configuration", SPIFFS, "/web/html/master.html");

    web_server.serveStatic("/global", SPIFFS, "/web/components/global.js");
    web_server.serveStatic("/module", SPIFFS, "/web/components/module.js");

    //marshals configuration data and returns it
    web_server.on("/global-data", HTTP_GET, get_global_data);
    web_server.on("/module-data", HTTP_GET, get_module_data);
    
    //Reset
    web_server.on("/reset", HTTP_POST, reset);
    
    //Saving
    web_server.on("/configuration", HTTP_POST, wireless_save_configuration);
}

void init_receiver_server() {
    char * ssid;
    char * password;

    Configuration * conf = get_configuration(syn);

    if (conf == NULL) {
        logger("(start_master_server) Configuration object is NULL\n");
        return;
    }

    if (AP_SET != 0) {
        logger("(start_master_server) AP Already started\n");
        return;
    }

    //WiFi.disconnect(true); //Forget last attempts
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig(apStationIP, apIP, IPAddress(255, 255, 255, 0));

    //If password is NULL AP will be open
    ssid = get_ssid(conf);
    password = get_password(conf);
    logger("(start_master_server) Values\n\t* SSID: %s\n\t* Password: %s\n", ssid, (password == NULL) ? "NULL" : password);
 
    while (! WiFi.softAP(ssid, password)) { delay(10); }
    
    //Sets captive portal for any route
    dns_server.start(DNS_PORT, "synestesia.com", apStationIP);

    AP_SET = 1;
}

void set_receiver_http_handlers () {
    web_server.serveStatic("/vue", SPIFFS, "/web/js/vue.min.js");
    web_server.serveStatic("/scripts", SPIFFS, "/web/js/scripts.js");
    web_server.serveStatic("/styles", SPIFFS, "/web/css/bootstrap.min.css");

    web_server.serveStatic("/configuration", SPIFFS, "/web/html/receiver.html");

    web_server.serveStatic("/global", SPIFFS, "/web/components/global.js");
    web_server.serveStatic("/module", SPIFFS, "/web/components/module.js");
    web_server.serveStatic("/receiver", SPIFFS, "/web/components/receiver.js");

    //marshals configuration data and returns it
    web_server.on("/global-data", HTTP_GET, get_global_data);
    web_server.on("/module-data", HTTP_GET, get_module_data);
    
    //Reset
    web_server.on("/reset", HTTP_POST, reset);
    
    //Saving
    web_server.on("/configuration", HTTP_POST, wireless_save_configuration);
        
    //Connect to target
    web_server.on("/connect", HTTP_POST, receiver_connect);
}

/************* PUBLIC HANDLERS *************/

void handle_client() {
    dns_server.processNextRequest();
    web_server.handleClient();
}

/************* PUBLIC METHODS *************/

/**
 * Starts wireless services:
 *      HTTP Server in master mode for master devices
 *      HTTP Server in receiver mode for receiver devices
 *      PostUp service for both master and receiver devices
 **/
void start_wireless_services(Synestesia * synestesia) {
    //Assigns pointers
    syn = synestesia;

    DeviceType type = get_device_type(syn);

    //Run http server in master mode
    if (type == MASTER) {
        init_master_server();
        set_master_http_handlers();
    }
    else if (type == RECEIVER) {
        init_receiver_server();
        set_receiver_http_handlers();
    }
    else { return; }

    web_server.begin();
    start_postUp_service(); // Run post up service
}

/**
 * Ends wireless services
*/
void end_wireless_services() {
    AP_SET = 0;
    syn = NULL;
    WiFi.softAPdisconnect(true);
}

void broadcast_frequency(String freq) {
    send_package((char *) freq.c_str(), broadcastIP);
}

float receive_frequency() { 
    return receive_udp_freq();
}

#else 

void start_wireless_services(Synestesia * synestesia) { return; }
void end_wireless_services() { return; }
void handle_client() { return; }
void broadcast_frequency(String freq) { return; }
float receive_frequency() { return 0; }

#endif