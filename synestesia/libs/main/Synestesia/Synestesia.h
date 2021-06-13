#ifndef SYNESTESIA_h
#define SYNESTESIA_h

#include "Arduino.h"
#include "configuration.h"
#include "module_functions.h"

typedef struct _Synestesia Synestesia;
typedef enum DeviceType {MASTER, SLAVE, RECEIVER, NONE};

float run_core(Synestesia * synestesia);

Synestesia * initialize(ModuleFunctions * mFn, DeviceType type);

Synestesia * initialize_slave();

DeviceType get_device_type(Synestesia * synestesia);

Configuration * get_configuration(Synestesia * synestesia);

int set_configuration(Synestesia * synestesia, Configuration * configuration);

void free_all(Synestesia * synestesia);

int write_file_to_flash(char * path, char * content);

char * read_file_from_flash(char * path);

#endif