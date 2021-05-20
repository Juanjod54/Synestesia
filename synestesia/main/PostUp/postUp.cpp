#include "Arduino.h"

#include "postUp.h"
#include <WiFiUdp.h>

#define PORT 2512

WiFiUDP postUpService;

void start_postUp_service() { 
    postUpService.begin(PORT); 
}

void send_package(char * msg, IPAddress address) {
    postUpService.beginPacket(address, PORT);
    postUpService.write(msg);
    postUpService.endPacket();
}

int receive_package(char ** msg) {
    int read_size = postUpService.parsePacket();
    if (! read_size) return NULL;

    IPAddress address = postUpService.remoteIP();
    postUpService.read(*msg, read_size);
    return read_size;
}