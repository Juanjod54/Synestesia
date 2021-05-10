
/**
 *  Created on 04-19-2021 by Juan Jose Daza Linares
 *  Filename: map.h 
 **/

#include "little_hash_map.h"

#ifdef DEBUG_ESP_PORT
#define logger(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define logger(...)
#endif

/** PRIVATE **/

typedef struct {
    long hash; //Colisiones
    void * key;
    void * value;
} KeyValue;

struct _LittleHashMap {
    int n_items;
    int max_items;
    
    hash_function hash_fn;
    free_map_key free_key_fn;
    free_map_value free_value_fn;

    KeyValue ** key_values;
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
void free_key_value(KeyValue * key_value, free_map_key free_key, free_map_value free_value) {
    if (!key_value) return;

    if (! (!key_value -> key)) {
        free_key(key_value -> key);
        key_value -> key = NULL;
    }
    
    if (! (!key_value -> value)) {
        free_value(key_value -> value);
        key_value -> value = NULL;
    }

    //Frees object
    free(key_value);
    key_value = NULL;
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
 * Returns the key of a KeyValue object
 * It returns NULL if KeyValue object is NULL
*/
void * get_key(KeyValue * key_value) {
    if (key_value == NULL) return NULL;
    return key_value -> key;
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
int get_index(LittleHashMap * map, long hash, long original_hash) {
    int index = hash % map->max_items;
    
    if (index < 0) { return -1; }

    //New item
    if (map -> key_values[index] == NULL) {
        return index;
    }

    //Item update
    else if (get_hash(map -> key_values[index]) == original_hash) {
        return index;
    }

    //Colision
    return get_index(map, (hash + 1) * (map->max_items + 1), original_hash);
}

/** PUBLIC **/

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
LittleHashMap * create_map(hash_function hash_fn, free_map_key free_key, free_map_value free_value, int max_items) {
    int i;
    KeyValue ** key_values;

    //Check function pointers
    if (hash_fn == NULL || free_key == NULL || free_value == NULL || max_items <= 0) {
        logger("(create_map) INPUT PARAM ERROR\n"); 
        return NULL;
    }

    //Creates map
    LittleHashMap * map = (LittleHashMap *) malloc(sizeof(map[0]));
    if (map == NULL) { 
        logger("(create_map) Cannot allocate LittleHashMap object\n");
        return NULL; 
    }

    //Creates KeyValue's array
    key_values = (KeyValue **) malloc(sizeof(key_values[0]) * max_items);
    if (key_values == NULL) {
        logger("(create_map) Cannot allocate KeyValue's array\n");
        free_map(map);
        return NULL; 
    }
    //Initialize key_values to NULL
    for(i = 0; i < max_items; i++) key_values[i] = NULL;

    map -> n_items = 0;
    map -> max_items = max_items;

    map -> key_values = key_values;

    //Assign function pointers
    map -> hash_fn = hash_fn;
    map -> free_key_fn = free_key;
    map -> free_value_fn = free_value;

    return map;
}

/*
 * Frees a map and its content 
*/
void free_map(void * pt_map) {
    int item;
    LittleHashMap * map = (LittleHashMap *) pt_map;
    
    if (!map) { return; }

    //First version O(N) -> We dont know indexes
    //It is better to have lower performance on exit than memory overflows 
    for (item = 0; item < map -> max_items; item ++) {
        if (map -> key_values[item]) {
            free_key_value(map->key_values[item], map -> free_key_fn, map -> free_value_fn);
        }
    }

    //Frees KeyValue's array
    free(map -> key_values);
    map -> key_values = NULL;

    free(map);
    map = NULL;
}   

/*
 * Adds a key - value pair into the map.
 * This method does NOT copy any item.
 * For the same key it overrides the existing value. Old value is NOT freed
*/
int map_put(LittleHashMap * map, void * key, void * value) {
    long hash;
    int index;

    KeyValue * key_value;

    //If map is NULL or map is full it returns without adding new object
    if (map == NULL || map->n_items == map->max_items) { 
        logger("(map_put) LittleHashMap object is NULL or full\n");
        return 0; 
    }
    
    if (key == NULL || value == NULL) { logger("(map_put) WARNING There's a NULL input value\n"); }

    hash = map -> hash_fn(key); //Gets hash
    
    index = get_index(map, hash, hash); //Gets index in structure
    
    if (index < 0) {
        logger("(map_put) Index can not be below 0: %d\n", index);
        return 0;
    }

    //Creates new key_value
    if (map -> key_values[index] == NULL) {
        map -> key_values[index] = create_key_value(key, value, hash);
        map -> n_items ++;
    }

    //Updates key_value
    else {
        update_value(map -> key_values[index], value);
    } 

    return 1;

}

/*
 * Returns the value for a key. 
 * If there is no value it returns NULL
*/
void * map_get(LittleHashMap * map, void * key) {
    long hash;
    int index;

    KeyValue * key_value;

    if (map == NULL || key == NULL) return NULL;

    hash = map -> hash_fn(key); //Gets hash

    index = get_index(map, hash, hash); //Gets index in structure
    
    return get_value(map -> key_values[index]);
}

/**
 * Returns an array with the keys stored in the map.
 * LittleHashMap does NOT copy any item, so the returned keys are the stored ones
 * If map is NULL it returns NULL
 */ 
void ** map_keys(LittleHashMap * map) {
    int items;
    int current;
    void ** keys;

    if (map == NULL) return NULL;

    keys = (void **) malloc(sizeof(keys[0]) * map -> n_items);
    if (keys == NULL) return NULL;

    current = 0;

    for (items = 0; items < map -> max_items; items++) {
        if (map->key_values[items] != NULL) keys[current ++] = get_key(map -> key_values[items]);
    }

    return keys;
}

/**
 * Returns an array with the values stored in the map.
 * LittleHashMap does NOT copy any item, so the returned keys are the stored ones
 * If map is NULL it returns NULL
 */ 
void ** map_values(LittleHashMap * map) {
    int items;
    int current;
    void ** values;

    if (map == NULL) return NULL;

    values = (void **) malloc(sizeof(values[0]) * map -> n_items);
    if (values == NULL) return NULL;

    current = 0;

    for (items = 0; items < map -> max_items; items++) {
        if (map->key_values[items] != NULL) values[current ++] = get_value(map -> key_values[items]);
    }

    return values;
}

int map_size(LittleHashMap * map) {
    return (map == NULL) ? -1 : map -> n_items; 
}