
/**
 *  Created on 04-19-2021 by Juan Jose Daza Linares
 *  Filename: map.h 
 **/

#include "map.h"

#ifdef DEBUG_ESP_PORT
#define logger(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define logger(...)
#endif

#define N_ELEMENTS 32

/** PRIVATE **/

typedef struct {
    long hash; //Colisiones
    void * key;
    void * value;
} KeyValue;

struct _Map {
    int n_items;
    hash_function hash_fn;
    free_map_key free_key_fn;
    free_map_value free_value_fn;
    KeyValue * key_values[N_ELEMENTS];
};


/**
 * Creates a new key_value, assigning a value to a key, and saving its hash for further comparations
 */
KeyValue * create_key_value(void * key, void * value, long hash) {
    KeyValue * key_value;

    //Allocates KeyValue object
    key_value = (KeyValue *) malloc(sizeof(key_value[0]));
    if (key_value == NULL) { 
        logger("(create_key_value) Cannot allocate memory for hash key: %ld\n", hash); 
        return NULL; 
    }

    //Assigns key and value objects
    key_value -> key = key;
    key_value -> value = value;

    //Saves hash for further comparations
    key_value -> hash = hash;

    return key_value;
}

/*
 * Frees a KeyValue object, by freeing its value and key 
 */
void free_key_value(KeyValue * key_value, free_map_key free_key_fn, free_map_value free_value_fn) {
    if (key_value == NULL) return;
    
    //Frees key
    free_key_fn(key_value -> key);

    //Frees value
    free_value_fn(key_value -> value);

    //Frees object
    free(key_value);
}

/*
 * Returns the hash for a given KeyValue object. 
 * It returns -1 if KeyValue object is NULL
*/
long get_hash(KeyValue * key_value) {
    if (key_value == NULL) { return -1; }
    return key_value -> hash;
}

/*
 * Returns the value of a KeyValue object
 * It returns NULL if KeyValue object is NULL
*/
void * get_value(KeyValue * key_value) {
    if (key_value == NULL) return NULL;
    return key_value -> value;
}

/*
 * Updates given KeyValue's value with new_value object
 * It does NOT free old value
 * Returns old KeyValue's value 
*/
void * update_value(KeyValue * key_value, void * new_value) {
    void * old_value;
    if (key_value == NULL) { 
        logger("(update_value) KeyValue object is null\n"); 
        return NULL; 
    }
    
    old_value = key_value -> value;
    key_value -> value = new_value;

    return old_value;
}

/*
 * It returns the proper index in map structure given a hash value
 * It avoids colissions
*/
int get_index(Map * map, long hash) {
    int index = hash % N_ELEMENTS;

    //New item
    if (map -> key_values[index] == NULL) return index;

    //Item update
    else if (get_hash(map -> key_values[index]) == hash) return index;

    //Colision
    else return get_index(map, hash * 2);
}

/** PUBLIC **/

/*
 * Creates a new map object, assigning the given data type functions 
 * If any given data type function is NULL it returns NULL
 * Otherwise it returns a map object
 * 
 * @param hash_fn: should return -1 if there is any error. Otherwise it should return a number >= 0
 * @param free_key_fn: function to free key objects
 * @param free_value_fn: function to free value objects
*/
Map * create_map(hash_function hash_fn, free_map_key free_key_fn, free_map_value free_value_fn) {
    int i;

    //Check function pointers
    if (hash_fn == NULL || free_key_fn == NULL || free_value_fn == NULL) {
        logger("(create_map) Function pointers are NULL\n"); 
        return NULL;
    }

    //Creates map
    Map * map = (Map *) malloc(sizeof(map[0]));
    if (map == NULL) { 
        logger("(create_map) Cannot allocate Map object\n");
        return NULL; 
    }

    //Initialize key_values to NULL
    for (i = 0; i < N_ELEMENTS; i++) { map -> key_values[i] = NULL; }

    map -> n_items = 0;

    //Assign function pointers
    map -> hash_fn = hash_fn;
    map -> free_key_fn = free_key_fn;
    map -> free_value_fn = free_value_fn;

    return map;
}

/*
 * Frees a map and all its content 
*/
void free_map(void * pt_map) {
    int item;
    Map * map = (Map *) pt_map;
    
    if (map == NULL) { return; }

    //It frees all items.
    //First version O(N) -> We dont know indexes
    for (item = 0; item < N_ELEMENTS; item ++) {
        if (map -> key_values[item] != NULL) {
            free_key_value(map->key_values[item], map -> free_key_fn, map -> free_value_fn);
        }
    }

    free(map);
}   

/*
 * Adds a key - value pair into the map.
 * This method does NOT copy any item.
 * For the same key it overrides the existing value. Old value is NOT freed
*/
void map_put(Map * map, void * key, void * value) {
    long hash;
    int index;

    KeyValue * key_value;

    //If map is NULL or map is full it returns without adding new object
    if (map == NULL || map->n_items == N_ELEMENTS) { 
        logger("(map_put) Map object is NULL or full\n");
        return; 
    }

    hash = map -> hash_fn(key); //Gets hash
    index = get_index(map, hash); //Gets index in structure

    logger("(map_put) Got index %d\n", index);

    //Creates new key_value
    if (map -> key_values[index] == NULL) {
        logger("(map_put) Creating new KeyValue object\n");
        map -> key_values[index] = create_key_value(key, value, hash);
    }
    
    //Updates key_value
    else {
        logger("(map_put) Updating existing KeyValue object\n");
        update_value(map -> key_values[index], value);
    } 

    map -> n_items ++;

}

/*
 * Returns the value for a key. 
 * If there is no value it returns NULL
*/
void * map_get(Map * map, void * key) {
    long hash;
    int index;

    KeyValue * key_value;

    if (map == NULL) return NULL;

    //Get the index for the key
    hash = map -> hash_fn(key); //Gets hash
    index = get_index(map, hash); //Gets index in structure

    return get_value(map -> key_values[index]);
}