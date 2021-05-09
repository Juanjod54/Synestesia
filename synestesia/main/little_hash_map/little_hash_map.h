
#include "Arduino.h"

typedef long (*hash_function)(void *);

typedef void (*free_map_key)(void *);

typedef void (*free_map_value)(void *);

typedef struct _LittleHashMap LittleHashMap;

/*
 * Creates a new map object, assigning the given hash function 
 * This map does NOT copy objects, IT ONLY MAPS POINTERS
 * If hash function is NULL it returns NULL
 * Otherwise it returns a map object
 * 
 * @param hash_fn: should return -1 if there is any error. Otherwise it should return a number >= 0
 * @param free_key: Function pointer to free key. 
 * @param free_key: Function pointer to free value.
 * @param max_items: max number of items that this map should contain
*/
LittleHashMap * create_map(hash_function hash_fn, free_map_key free_key, free_map_value free_value, int max_items);

/*
 * Frees a map and its content 
*/
void free_map(void * map);

/*
 * Adds a key - value pair into the map.
 * This method does NOT copy any item.
 * For the same key it overrides the existing value. Old value is NOT freed
*/
int map_put(LittleHashMap * map, void * key, void * value);

/*
 * Returns the value for a key. 
 * If there is no value it returns NULL
*/
void * map_get(LittleHashMap * map, void * key);

void ** map_keys(LittleHashMap * map);

void ** map_values(LittleHashMap * map);

int map_size(LittleHashMap * map);