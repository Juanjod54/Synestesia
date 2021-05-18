#include <ESP8266WiFi.h>

#ifndef POSTUP_H
#define POSTUP_H

void start_postUp_service();

void broadcast_package(char * msg);

void send_package(char * msg, IPAddress address);

int receive_package(char ** msg);

#endif