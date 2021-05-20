#include <Wire.h>
#include "Thread.h"

#include "Synestesia.h"

#include "audio.h"
#include "wireless.h"

#define MAX_INSTANCES 1


Thread server_thread = Thread();

static Synestesia * static_syn;
static int instances = 0;

static float slave_freq;

struct _Synestesia {
    DeviceType type;
    Configuration * configuration;
};

/******************* PRIVATE METHODS *************************/

int check_module_functions (ModuleFunctions * mFn) {
    if (mFn == NULL) return 0;

    else if (mFn -> load_fn == NULL) return 0;
    else if (mFn -> free_fn == NULL) return 0;
    else if (mFn -> save_fn == NULL) return 0;
    else if (mFn -> load_fn == NULL) return 0;
    else if (mFn -> marshal_fn == NULL) return 0;
    else if (mFn -> unmarshal_fn == NULL) return 0;

    return 1;
}

Synestesia * create_object(DeviceType type) {
    
    //For performance and integrity reasons there can only be one Synestesia object at a time
    if (instances >= MAX_INSTANCES) return static_syn;
    
    Synestesia * syn = (Synestesia *) malloc(sizeof(syn[0]));
    
    if (syn == NULL) { 
        Serial.println("[FATAL ERROR] Couldn't allocate Synestesia object."); 
        return NULL;
    }

    syn -> type = type;
    syn -> configuration = NULL;
    
    //Static variables update
    instances ++;
    static_syn = syn;

    return syn;
} 

void free_object(Synestesia * synestesia) {
    if (synestesia == NULL) return;
    if (synestesia -> configuration) {
        free_configuration(synestesia -> configuration);
        synestesia -> configuration = NULL;
    }

    free(synestesia);
    synestesia = NULL;

    //Static variables update
    instances --;
    static_syn = NULL;

    end_wireless_services();
}

/************ MASTER ************/ 

float run_master(Synestesia * synestesia) {
    //Protothread to run web server
    if (server_thread.shouldRun()) { server_thread.run(); }

    Wire.requestFrom(1, 10);
    float freq = Wire.parseFloat();
    broadcast_frequency(String(freq));
    return freq;
}

Synestesia * initialize_master(ModuleFunctions * mFn) {
    Synestesia * syn;
    Configuration * configuration;

    //Check if it must load the module
    int load_module = check_module_functions(mFn);
    
    //Creates a Synestesia object
    syn = create_object(MASTER);
    if (syn == NULL) { return NULL; }

    if (load_module) {
        //Module should load, use mFn functions
        configuration = load_configuration_and_module(mFn -> load_fn, 
                                                      mFn -> save_fn, 
                                                      mFn -> marshal_fn, 
                                                      mFn -> unmarshal_fn, 
                                                      mFn -> free_fn);
    } 
    else {
        //Module should not load
        Serial.println("[Warning] Initializing without module...");
        configuration = load_configuration();
    }

    //Check configuration status. Even if there is no configuration file, defaults are loaded
    if (configuration == NULL) {
        Serial.println("[FATAL ERROR] Couldn't load configuration object.");
        free_object(syn);
        return NULL;
    }

    syn -> configuration = configuration;

    return syn;
}

/*******************************/ 

/************ SLAVE ************/ 

void slave_on_request() { Wire.write(String(slave_freq).c_str()); }
float run_slave(Synestesia * synestesia) { slave_freq = get_frequency(); }

Synestesia * initialize_slave() {
    Wire.begin(1); 
    Wire.onRequest(slave_on_request);

    Synestesia * synestesia = create_object(SLAVE);
    return synestesia;
}

/*******************************/ 

/*********** RECEIVER ***********/
float run_receiver(Synestesia * synestesia) {return 0;}

Synestesia * initialize_receiver() {
    return NULL;
}

/*******************************/ 

/************************************************************/

/******************* PUBLIC METHODS *************************/

float run_core(Synestesia * synestesia) {
    if (synestesia == NULL) return 0;
    
    if (synestesia -> type == MASTER) { return run_master(synestesia); }
    else if (synestesia -> type == SLAVE) { run_slave(synestesia); }
    else if (synestesia -> type == RECEIVER) { run_receiver(synestesia); }

    return 0;

}

Synestesia * initialize(ModuleFunctions * mFn) {
    //Begins wire
    Wire.begin(D1, D2);

    //Creates thread
    server_thread.onRun(handle_client);

    Synestesia * synestesia = initialize_master(mFn);
    if (synestesia != NULL) { start_wireless_services(synestesia); }
    return synestesia;
}

Synestesia * initialize_by_type(DeviceType type) {
    Synestesia * synestesia;

    //If it is a master device it loads it without module
    if (type == MASTER) { return initialize(NULL); }

    //If it is slave device it should 
    if (type == SLAVE) { return initialize_slave(); }

    if (type == RECEIVER) { return initialize_receiver(); }

    return NULL;
}

int set_configuration(Synestesia * synestesia, Configuration * configuration) {
    if (synestesia == NULL || configuration == NULL) { return 0; }
    if (configuration == synestesia -> configuration) { return 1; }

    if (free_configuration(synestesia -> configuration)) {
        synestesia -> configuration = configuration;
        return 1;
    }

    return 0;
    
}

Configuration * get_configuration(Synestesia * synestesia) {
    return (synestesia == NULL) ? NULL : synestesia -> configuration;
}

DeviceType get_device_type(Synestesia * synestesia) {
    return (synestesia == NULL) ? NONE : synestesia -> type;
}

/************************************************************/
