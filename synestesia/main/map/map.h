
#include "Arduino.h"

typedef long (*hash_function)(void *);

typedef void (*free_map_key)(void *);

typedef void (*free_map_value)(void *);

typedef struct _Map Map;

/*
 * Creates a new map object, assigning the given data type functions 
 * If any given data type function is NULL it returns NULL
 * Otherwise it returns a map object
 * 
 * @param hash_fn: should return -1 if there is any error. Otherwise it should return a number >= 0
 * @param free_key_fn: function to free key objects
 * @param free_value_fn: function to free value objects
*/
Map * create_map(hash_function hash_fn, free_map_key free_key_fn, free_map_value free_value_fn);

/*
 * Frees a map and all its content 
*/
void free_map(void * map);

/*
 * Adds a key - value pair into the map.
 * This method does NOT copy any item.
 * For the same key it overrides the existing value. Old value is NOT freed
*/
void map_put(Map * map, void * key, void * value);

/*
 * Returns the value for a key. 
 * If there is no value it returns NULL
*/
void * map_get(Map * map, void * key);