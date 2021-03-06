#include <Wire.h>
#include "Thread.h"

#include "audio.h"
#include "wireless.h"
#include "Synestesia.h"

#define MAX_INSTANCES 1

/**** Freq Analyzer ****/
static float read_freq = 0;
Thread freq_thread = Thread();
/***********************/

static int instances = 0;
static Synestesia * static_syn;

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

void request_frequency() {
    Wire.requestFrom(1, 10);
    read_freq = Wire.parseFloat();
    broadcast_frequency(String(read_freq));
}

void run_master(Synestesia * synestesia) {
    //Protothread to run web server
    if (freq_thread.shouldRun()) { freq_thread.run(); }

    handle_client();
}

/************ SLAVE ************/ 

void slave_on_request() { Wire.write(String(read_freq).c_str()); }
void run_slave(Synestesia * synestesia) { read_freq = get_frequency(); }

/*********** RECEIVER ***********/

void run_receiver(Synestesia * synestesia) {
    //if (freq_thread.shouldRun()) { freq_thread.run(); }
    float new_freq = receive_frequency();
    if (new_freq >= 0) { read_freq = new_freq; }
    handle_client();
}

/*******************************/ 

#ifdef ARDUINO_ESP8266_NODEMCU

void start_master_dependencies(Synestesia * synestesia) {
    Wire.begin(D1, D2); //Begins wire
    Wire.setTimeout(20);
    freq_thread.onRun(request_frequency); //Creates thread
    start_wireless_services(synestesia); //Start wireless services
}

void start_receiver_dependencies(Synestesia * synestesia) {
    start_wireless_services(synestesia); //Start wireless services
}

#else

void start_master_dependencies(Synestesia * synestesia) { return; }
void start_receiver_dependencies(Synestesia * synestesia) { return; }

#endif

Synestesia * initialize_by_type(ModuleFunctions * mFn, DeviceType type) {
    Synestesia * syn;
    Configuration * configuration;

    //Check if it must load the module
    int load_module = check_module_functions(mFn);
    
    //Creates a Synestesia object
    syn = create_object(type);

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

    if (type == MASTER) start_master_dependencies(syn);
    else start_receiver_dependencies(syn);

    return syn;
}

/************************************************************/

/******************* PUBLIC METHODS *************************/

float run_core(Synestesia * synestesia) {
    if (synestesia == NULL) return 0;
    
    if (synestesia -> type == MASTER) { run_master(synestesia); }
    else if (synestesia -> type == SLAVE) { run_slave(synestesia); }
    else if (synestesia -> type == RECEIVER) { run_receiver(synestesia); }

    return read_freq;
}

Synestesia * initialize(ModuleFunctions * mFn, DeviceType type) {
    if (type == MASTER || type == RECEIVER) { return initialize_by_type(mFn, type); }
    return NULL;
}

Synestesia * initialize_slave() {
    Synestesia * synestesia = create_object(SLAVE);
    if (synestesia != NULL) {
        Wire.begin(1); 
        Wire.onRequest(slave_on_request);
    }

    return synestesia;
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
