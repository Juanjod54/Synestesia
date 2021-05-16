/**
 *  Created on 04-25-2021 by Juan Jose Daza Linares
 *  Filename: wireless.cpp
 **/

#include "wireless.h"
#include "file_manager.h"

#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/** Logger **/
#ifdef DEBUG_ESP_PORT
#define logger(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define logger(...)
#endif

#define DNS_PORT 53

#define CSS "text/css"
#define HTML "text/html"
#define PLAIN "text/plain"
#define JS "text/javascript"

#define MODULE_HTML "/module.html"

#define OK "OK"
#define ERROR "ERROR"

static int AP_SET = 0;
static int session;

static Configuration * conf;

ESP8266WebServer web_server(80);
DNSServer dns_server;

/************* DATA HANDLERS *************/

void get_global_data () {
    char * parsed_data = marshall_global_configuration(conf);
    
    if (parsed_data == NULL) { 
        web_server.send(500, PLAIN, ERROR); 
    }
    else { 
        web_server.send(200, PLAIN, parsed_data); 
        free(parsed_data);
    }    
}

void get_module_data () {
    char * parsed_data = marshall_module_configuration(conf);
    if (parsed_data == NULL) { 
        web_server.send(404, PLAIN, ERROR); 
    }
    else { 
        web_server.send(200, PLAIN, parsed_data); 
        free(parsed_data);
    }    
}

void wireless_save_configuration() {
    char * global_text = NULL;
    char * module_text = NULL;

    Configuration * new_configuration = NULL;
    
    global_text = (char *) web_server.arg("global").c_str();
    new_configuration = unmarshall_global_configuration(global_text);

    if (get_module_configuration(conf) != NULL) {
        module_text = (char *) web_server.arg("module").c_str();

        //Updates module references
        update_global_configuration_module_references(conf, new_configuration);

        //Unmarshalls module
        new_configuration = unmarshall_module_configuration(new_configuration, module_text);
    }

    if (new_configuration != NULL && save_configuration(new_configuration)) {
        //Frees global configuration object
        free_configuration(conf);

        //Update configuration reference
        conf = new_configuration;

        web_server.send(200, PLAIN, OK);
        return;
    }

    //There was an error
    web_server.send(500, PLAIN, ERROR);
}

/************* OTHER HANDLERS *************/

bool is_authentified(){
  if (web_server.hasHeader("Cookie")){
    String cookie = web_server.header("Cookie");
    return cookie.indexOf(String(session)) != -1;
  }

  return false;
}

void handle_login(){
    logger("Loggin new user\n");
    if (web_server.hasArg("password") && web_server.arg("password") == "admin") {
        session = rand(); //Store new session ID
        logger("New session\n");
        web_server.sendHeader("Location","/configuration");
        web_server.sendHeader("Cache-Control","no-cache");
        web_server.sendHeader("Set-Cookie", String(session));
    }
    else {
        logger("New session\n");
        web_server.send(401);
    }
  
}

void handle_captive () {
    //web_server.send(200, "text/html", login_html);
}


/**
 * Sets board as an access_point, by setting given configuration ssid and password
 * @param configuration: Configuration object
*/
void start_access_point() {
    char * ssid;
    char * password;

    IPAddress apIP(192, 168, 1, 1);
    
    if (conf == NULL) {
        logger("(start_access_point) Configuration object is NULL\n");
        return;
    }

    if (AP_SET != 0) {
        logger("(start_access_point) AP Already started\n");
        return;
    }

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

    //If password is NULL AP will be open
    ssid = get_ssid(conf);
    password = get_password(conf);
    logger("(start_access_point) Values\n\tSSID: %s\n\t%s\n", ssid, (password == NULL) ? "NULL" : password);

    while (! WiFi.softAP(ssid, password)) { delay(100); }

    //Sets captive portal for any route
    //dns_server.start(DNS_PORT, "synestesia", apIP);

    AP_SET = 1;
}

void start_server(Configuration * configuration) {
    conf = configuration;

    start_access_point();

    web_server.serveStatic("/vue", SPIFFS, "/web/js/vue.min.js");
    web_server.serveStatic("/scripts", SPIFFS, "/web/js/scripts.js");
    web_server.serveStatic("/styles", SPIFFS, "/web/css/bootstrap.min.css");

    web_server.serveStatic("/configuration", SPIFFS, "/web/html/configuration.html");

    web_server.serveStatic("/global", SPIFFS, "/web/components/global.js");
    web_server.serveStatic("/module", SPIFFS, "/web/components/module.js");

    //web_server.onNotFound(handle_captive);
    //web_server.on("/login", handle_login);
    
    //Marshalls configuration data and returns it
    web_server.on("/global-data", HTTP_GET, get_global_data);
    web_server.on("/module-data", HTTP_GET, get_module_data);

    web_server.on("/configuration", HTTP_POST, wireless_save_configuration);
    
    web_server.begin();
}

void handle_client() {
    web_server.handleClient();
    //if (! is_authentified()) { dns_server.processNextRequest(); }
}

/**
 * Ends access point configuration
*/
void end_server() {
    AP_SET = 0;
    conf = NULL;
    WiFi.softAPdisconnect(true);
}
