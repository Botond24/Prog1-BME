//
// Created by sziha on 21/10/2023.
//

#ifndef NHF_SOURCE_READER_H
#define NHF_SOURCE_READER_H
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "ini_reader.h"
#include "debugmalloc.h"

///enum extension for node types
typedef enum {
    other = main_ + 1,
    func_call = other + 1,
} type_e; ///extending type enum

///enum for loop conditions
typedef enum {
    if_,///if
    else_,///else
    else_if,///else if
    switch_,///switch
    case_,///case
    while_,///while
    for_,///for
    do_while,///do while
} loop_cond_e;


///types of nodes
typedef struct {
    char *return_type;
    char **args;
} func_type_t;

typedef struct {
    char *args;
} struct_type_t;

typedef struct {
    char *type;
    char *value;
} variable_type_t;

typedef struct {
    loop_cond_e type;
    char *condition;
    bool finished;
} loop_cond_type_t;

typedef struct {
    char *value;
} other_type_t;


/**
 * linked list structure
 */
typedef struct node{
    int type; /// type of the node
    char *name; /// name of the node
    int indent; /// indent of the node
    int call_num; /// number of time the node has been called
    struct node **goList; /// list of return to nodes
    union {
        func_type_t func; /// function
        struct_type_t struct_; /// struct
        variable_type_t variable; /// variable
        loop_cond_type_t loop_cond; /// loop or conditional statement
        other_type_t other; /// other
    };
    struct node *next; /// list of next nodes
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
char *truncate(char *str, int len);

/**
 * find the last occurence of a char in a string
 * @param str string to search
 * @param c char to search for
 * @return -1 if not found, else the index
 */
int lastOccurrence(char *str, char c);

/**
 * Creates a new node
 * @param type type of the node
 * @return new node
 */
node_t *createNode(int type);

/**
 * advances to the next node
 * @param node current node
 * @return next node
 */
node_t *nextNode(node_t *node);

/**
 * finds a function by name in the fucntions list
 * @param node node to get name from
 * @param functions functions list
 * @return function node
 */
node_t *findFunction(node_t *node, node_t *functions[]);


/**
 * finds a node with a given name starting from a specified node.
 * @param start The starting node for the search.
 * @param name The name of the node to find.
 * @param indent The indentation level to consider for the search.
 * @return A pointer to the found node, or NULL if no node with the given name is found.
 */
node_t *findByName(node_t *start, char* name, int indent);

/**
 * finds a node with a specified type and indentation level starting from a specified node.
 * @param start The starting node for the search.
 * @param type The type of the node to find.
 * @param indent The indentation level to consider for the search.
 * @return A pointer to the found node with the specified type and indentation level, or NULL if no such node is found.
 */
node_t *findByType(node_t *start, int type, int indent);


/**
 * finds the last node with a specified indentation level starting from a specified node.
 * @param start The starting node for the search.
 * @param indent The indentation level to consider for the search.
 * @return A pointer to the last node with the specified indentation level, or NULL if no such node is found.
 */
node_t *findLastByIndent(node_t *start, int indent);


/**
 * finds the previous node of a specified type and indentation level from a given starting node up to the current node.
 * @param start The starting node for the search.
 * @param current The current node, acting as the endpoint for the search.
 * @param type The type of the node to find.
 * @param indent The indentation level to consider for the search.
 * @return A pointer to the previous node with the specified type and indentation level, or NULL if no such node is found.
 */
node_t *findPreviousByType(node_t *start, node_t *current, int type, int indent);

/**
 * calculates the number of nodes in an array of node pointers.
 * @param arr The array of node pointers.
 * @return The number of nodes in the array.
 */
int sizeOfNodeArray(node_t **arr);

/**
 * Creates an array of node pointers with a specified size.
 * @param size The number of node pointers to allocate in the array.
 * @return A pointer to the newly allocated array of node pointers, or NULL if allocation fails.
 */
node_t **createNodeArray(int size);


/**
 * Appends a node to the end of an array of node pointers.
 * @param arr The array of node pointers to which the node will be appended.
 * @param node The node pointer that will be appended to the array.
 * @return A pointer to the array of node pointers with the new node appended, or NULL if the operation fails.
 */
node_t **appendNodeArray(node_t **arr, node_t *node);


/**
 * Frees all the nodes in a linked list starting from the given node.
 * @param start The starting node of the linked list to be freed.
 */
void freeLinkedList(node_t *start);

#endif //NHF_SOURCE_READER_H