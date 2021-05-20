
#ifndef SYNESTESIA_h
#define SYNESTESIA_h

#include "Arduino.h"
#include "configuration.h"

typedef enum DeviceType {MASTER, SLAVE, RECEIVER, NONE};

typedef struct _Synestesia Synestesia;

typedef struct ModuleFunctions {
    module_configuration_load load_fn; 
    module_configuration_free free_fn;
    module_configuration_save save_fn;
    module_configuration_marshal marshal_fn;
    module_configuration_unmarshal unmarshal_fn;
};

Synestesia * initialize(ModuleFunctions * mFn);

Synestesia * initialize_by_type(DeviceType type);

int set_configuration(Synestesia * synestesia, Configuration * configuration);

Configuration * get_configuration(Synestesia * synestesia);

float run_core(Synestesia * synestesia);

DeviceType get_device_type(Synestesia * synestesia);

#endif