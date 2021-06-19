#include "Arduino.h"

#include "postUp.h"
#include <WiFiUdp.h>

#define PORT 2512

#define TIMEOUT 50
#define BUFF_SIZE 5

WiFiUDP postUpService;

char buff[BUFF_SIZE];

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
    int i;
    int read_size = postUpService.parsePacket();
    
    if (read_size == 0) { return -1; }
    for (i = 0; i < BUFF_SIZE; i++) { buff[i] = '\0'; }

    postUpService.read((char *) buff, BUFF_SIZE);
    return String(buff).toFloat();
}
