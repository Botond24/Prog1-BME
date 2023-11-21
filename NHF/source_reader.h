//
// Created by sziha on 21/10/2023.
//

#ifndef NHF_SOURCE_READER_H
#define NHF_SOURCE_READER_H
#include <stdio.h>
#include <stdlib.h>
#include "ini_reader.h"
#include "debugmalloc.h"
typedef struct {
    char *return_type;
    char **args;
} func_type_t;

typedef struct {
    char *args;
} struct_type_t;

typedef struct {
    char *value;
} variable_type_t;

typedef struct {
    char *condition;
} conditional_type_t;

typedef struct {
    char *condition;
} loop_type_t;
/**
 * linked list structure
 */
typedef struct node{
    context_e type; /// type of the node
    char name[100]; /// name of the node
    union {
        func_type_t func; /// function
        struct_type_t struct_; /// struct
        variable_type_t variable; /// variable
        conditional_type_t conditional; /// conditional
        loop_type_t loop; /// loop
    };
    struct node **nextList; /// list of next nodes
} node_t;
/**
 * NOT FULLY IMPLEMENTED YET
 * @param source_file to read
 * @return linked_list of all the lines
 */
node_t *read_source(char *filename);
/**
 * Returns a substirng from the first appearance of start until the first appearance of end
 * @param str string to get substring from
 * @param start starting char
 * @param end ending char
 * @return Substring from start char (inclusive) to end char (non inclusive)
 */
char* substr(char *str, char start, char end);

/**
 * Truncates a string from spaces and endlines \n
 * if len < 0, string length doesnt change \n
 * if len > 0, truncates to len
 * @param str string to get truncated
 * @param len truncation length
 */
void truncate(char *str, int len);
#endif //NHF_SOURCE_READER_H
