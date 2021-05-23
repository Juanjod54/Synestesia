#include "Arduino.h"

#include "postUp.h"
#include <WiFiUdp.h>

#define PORT 2512

#define TIMEOUT 10

WiFiUDP postUpService;

void start_postUp_service() { 
    postUpService.begin(PORT); 
    postUpService.setTimeout(TIMEOUT);
}

void send_package(char * msg, IPAddress address) {
    postUpService.beginPacket(address, PORT);
    postUpService.write(msg);
    postUpService.endPacket();
}

float receive_udp_freq() {
    char * buff;
    int read_size = postUpService.parsePacket();
    if (! read_size) return NULL;
    postUpService.read(buff, 10);
    return String(buff).toFloat();
}