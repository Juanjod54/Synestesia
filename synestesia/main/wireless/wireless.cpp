/**
 *  Created on 04-25-2021 by Juan Jose Daza Linares
 *  Filename: wireless.cpp
 **/

#include "login.h"
#include "global.h"
#include "styles.h"
#include "scripts.h"
#include "web_configuration.h"


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


static int AP_SET = 0;
static int session;

static Configuration * conf;

ESP8266WebServer web_server(80);
DNSServer dns_server;

/************* FILE HANDLERS *************/
//TODO SET STATIC
void styles () {
    web_server.send(200, CSS, styles_css);
}

void scripts () {
    web_server.send(200, JS, scripts_js);
}

void handle_configuration() {
    web_server.send(200, HTML, configuration_html);
}

void handle_global () {
    char * html = read_from_file("/global.html");
    web_server.send(200, HTML, html);
}

void handle_module () {
    char * html = read_from_file(MODULE_HTML);
    if (html == NULL) { web_server.send(200, HTML, ""); }
    else {
        web_server.send(200, HTML, html); 
        free(html);
    }
}

/************* DATA HANDLERS *************/

void get_global_data () {
    char * parsed_data = marshall_global_configuration(conf);
    
    if (parsed_data == NULL) { 
        web_server.send(500, PLAIN, "ERROR\r\n"); 
    }
    else { 
        web_server.send(200, PLAIN, parsed_data); 
        free(parsed_data);
    }    
}

void get_module_data () {
    char * parsed_data = marshall_module_configuration(conf);
    if (parsed_data == NULL) { 
        web_server.send(500, PLAIN, "ERROR\r\n"); 
    }
    else { 
        web_server.send(200, PLAIN, parsed_data); 
        free(parsed_data);
    }    
}

/*
 * HANDLER
 * Parses global configuration text into an object and saves it
 * It updates this class reference to configuration object and frees the old one
*/
void save_global_data() {
    //Gets global configuration text from body
    char * global_text = (char *) web_server.arg("plain").c_str();
    
    //Unmarshalls it
    Configuration * new_configuration = unmarshall_global_configuration(global_text);
    
    //If unmarshalled configuration its ok and it could be saved
    if (new_configuration != NULL && save_configuration(new_configuration)) {

        //Frees global configuration object
        free_global(conf);

        //Update configuration reference
        conf = new_configuration;

        web_server.send(200, PLAIN, "OK\r\n");
        return;
    }

    //There was an error
    web_server.send(500, PLAIN, "ERROR\r\n");
}

/*void save_module_data() {
    //Gets module configuration text from body
    char * module_text = (char *) web_server.arg("plain").c_str();
    
    //Unmarshalls it
    Configuration * new_configuration = unmarshall_module_configuration(conf, module_text);
    
    //If unmarshalled configuration its ok and it could be saved
    if (new_configuration != NULL && save_configuration(new_configuration)) {
        
        //Updates new_configuration's module references and invalidates old ones
        update_global_configuration_module_references(conf, new_configuration);

        //Frees global configuration object
        free_global(conf);

        web_server.send(200, "text/plain", "OK\r\n");
        return;
    }

    //There was an error
    web_server.send(500, "text/plain", "OK\r\n");
}*/

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
    web_server.send(200, "text/html", login_html);
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

    web_server.serveStatic("/vue", SPIFFS, "/vue.min.js");
    web_server.serveStatic("/styles", SPIFFS, "/bootstrap.min.css");

    web_server.on("/scripts", scripts);

    //web_server.onNotFound(handle_captive);
    //web_server.on("/login", handle_login);
    
    //Loads configuration page
    web_server.on("/configuration", handle_configuration);
    web_server.on("/global", HTTP_GET, handle_global);
    web_server.on("/module", HTTP_GET, handle_module);
    
    //Marshalls configuration data and returns it
    web_server.on("/global-data", HTTP_GET, get_global_data);
    web_server.on("/module-data", HTTP_GET, get_module_data);

    //Unmarshalls configuration data and saves it
    web_server.on("/global-data", HTTP_POST, save_global_data);
    //web_server.on("/module", HTTP_POST, save_global_data);
    
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
