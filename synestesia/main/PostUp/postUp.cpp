#include "Arduino.h"

#include "postUp.h"
#include <WiFiUdp.h>

#define PORT 2512

WiFiUDP postUpService;

IPAddress broadcastIP(192, 168, 1, 255);

void start_postUp_service() { 
    postUpService.begin(PORT); 
}

void broadcast_package(char * msg) {
    return send_package(msg, broadcastIP);
}

void send_package(char * msg, IPAddress address) {
    postUpService.beginPacket(address, PORT);
    postUpService.write(msg);
    postUpService.endPacket();
    delay(500);
}

int receive_package(char ** msg) {
    int read_size = postUpService.parsePacket();
    if (! read_size) return NULL;

    IPAddress address = postUpService.remoteIP();
    postUpService.read(*msg, read_size);
    return read_size;
}