#ifndef MODULE_FUNCTIONS_h
#define MODULE_FUNCTIONS_h
/*
 * Returns a module_configuration object defined by the user 
*/
typedef void * (*module_configuration_load)();

/*
 * Saves a module_configuration object into a text file 
*/
typedef int (*module_configuration_save)(void *);

/*
 * Saves a module_configuration object into a text file
*/
typedef char * (*module_configuration_marshal)(void *);

/*
 * Loads a module configuration object from a string
 * First param contains marshaled data
 * Second param its the delimiter string
*/
typedef void * (*module_configuration_unmarshal)(char *);

/*
 * Deallocates module_configuration object
 * @param: module_configuration object defined by the user 
*/
typedef int (*module_configuration_free)(void*);

typedef struct ModuleFunctions {
    module_configuration_load load_fn; 
    module_configuration_free free_fn;
    module_configuration_save save_fn;
    module_configuration_marshal marshal_fn;
    module_configuration_unmarshal unmarshal_fn;
};

#endif