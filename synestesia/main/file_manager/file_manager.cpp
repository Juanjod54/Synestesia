/**
 * Created on 04-11-2021 by Juan Jose Daza Linares
 * This class uses SPIFFS system from ESP8266 Core, so it uses C++ objects
 * File: file_manager.cpp
 **/

#include <FS.h> 

#ifdef DEBUG_ESP_PORT
#define logger(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define logger(...)
#endif

#define READ_MODE "r"
#define WRITE_MODE "w"

#define ERROR -1

/*
 * Starts SPIFFS system
 */
void init_file_manager() {
    logger("Initiating SPIFFS\n");
    SPIFFS.begin();
}

/*
 * Finishes SPIFFS system 
 */
void end_file_manager() {
    SPIFFS.end();
}

/*
 * Reads content from file into a new allocated char array.
 * @param path: The absolute path (including /) to file
 * @return Returns file content if file exists. Otherwise it returns NULL
 */
char * read_from_file(char * path) {
    int size;
    char *content;

    File file = SPIFFS.open(path, READ_MODE);

    if (!file) {
        logger("Could not open file %s\n", path);
        return NULL;
    }

    size = file.size();
    logger("File size %d\n", size);

    if (size > 0) {

        //Allocate memory for content
        content = (char *) malloc((sizeof(content[0]) * size) + 1);
        
        if (content != NULL) {
            //Reads 'size' bytes into content buffer
            file.readBytes(content, size);
            content[size] = '\0'; //Adds end character
        }
        else 
            logger("Could not allocate memory to store file content (%d)\n", size);
    }

    file.close();

    return content;
}

/*
 * Writes char array content into a file.
 * @param path: The absolute path (including /) to file
 * @param content: The text to persist
 * @return Returns the number of written bytes or -1 if there was any error
 */
int write_to_file(char * path, char * content) {
    int bytes;

    File file = SPIFFS.open(path, WRITE_MODE);

    if (! file) {
        logger("Could not create file %s\n", path);
        return ERROR;
    }
    
    //Writes content and closes file
    bytes = file.print(content);
    file.close();

    return bytes;
}