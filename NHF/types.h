//
// Created by sziha on 16/10/2023.
//

#ifndef NHF_TYPES_H
#define NHF_TYPES_H
/**
 * Input file enum
 */
typedef enum {
    file_type_jpg, /// jpg file
    file_type_png, /// png file
    //file_type_md, /// <markdown file
} file_type_e;

/**
 * hash-map like struct for mapping strings to anything (not very safe)
 */
typedef struct {
    const char *key;
    const void *value;
} mapping_t;

#define DEFAULT_FILE_TYPE file_type_png

#endif//NHF_TYPES_H
