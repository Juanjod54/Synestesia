#include <ESP8266WiFi.h>

#ifndef POSTUP_H
#define POSTUP_H

void start_postUp_service();

void send_package(char * msg, IPAddress address);

float receive_udp_freq();

#endif