/**
 *  Created on 04-25-2021 by Juan Jose Daza Linares
 *  Filename: wireless.h
 **/

#ifndef WIRELESS_h
#define WIRELESS_h

#include <Arduino.h>
#include "Synestesia.h"

/**
 * Starts wireless services:
 *      HTTP Server in master mode for master devices
 *      HTTP Server in receiver mode for receiver devices
 *      PostUp service for both master and receiver devices
 **/
void start_wireless_services(Synestesia * synestesia);

/**
 * Ends wireless services
*/
void end_wireless_services();

void handle_client();

void broadcast_frequency(String freq);

float receive_frequency();

#endif
