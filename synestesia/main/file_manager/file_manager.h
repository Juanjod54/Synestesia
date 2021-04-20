
/**
 *  Created on 04-11-2021 by Juan Jose Daza Linares
 *  Filename: file_manager.h 
 **/

#ifndef FILE_MANAGER_h
#define FILE_MANAGER_h

#include "Arduino.h"

/*
 * Starts SPIFFS system
 */
void init_file_manager();

/*
 * Finishes SPIFFS system 
 */
void end_file_manager();

/*
 * Reads content from file into a new allocated char array.
 * @param path: The absolute path (including /) to file
 * @return Returns file content if file exists. Otherwise it returns NULL
 */
char * read_from_file(char * path);

/*
 * Writes char array content into a file.
 * @param path: The absolute path (including /) to file
 * @param content: The text to persist
 * @return Returns the number of written bytes or -1 if there was any error
 */
int write_to_file(char * path, char * content);

/*
 * Parses a configuration file line in "keyword:value" format
 * If read line starts with comment symbol (#) it returns NULL
 * If read line is null it returns NULL
 * Otherwise it returns read value and sets keyword param to found keyword 
 * 
 * @param line: Read line
 * @param keyword: Pointer in which found keyword will be set
*/
char * parse_line(char * line, char** keyword);

#endif