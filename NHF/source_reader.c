//
// Created by sziha on 21/10/2023.
//

#include "source_reader.h"

node_t *read_source(char *filename){
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Unable to open file %s\n", filename);
        return NULL;
    }
    node_t *first_node = (node_t *) malloc(sizeof(node_t));
    node_t *current_node = first_node;
    current_node->type = -1;
    char buffer[10000];
    int skip = 0;
    int c = 0;
    int c_bracket[2] = {0,0};
    int n_bracket[2] = {0,0};
    for (int i = 0; c != EOF; i = (i == 999) ? 0 : i + 1) {
        c = getc(fp);
        if (skip == 1 && (c != '\n' || c != 32)) {
            continue;
        }
        switch (c) {
            case '{':
                c_bracket[1] = c_bracket[0];
                c_bracket[0]++;
                break;
            case '}':
                c_bracket[1] = c_bracket[0];
                c_bracket[0]--;
                break;
            case '(':
                n_bracket[1] = n_bracket[0];
                n_bracket[0]++;
                break;
            case ')':
                n_bracket[1] = n_bracket[0];
                n_bracket[0]--;
                break;
            default:
                break;
        }
        skip = 0;
        buffer[i] = (char) c;
        buffer[i + 1] = '\0';
        if (buffer[i] == '#' || buffer[i] == '\n' || (i > 1 && (buffer[i-1] == '/' && buffer[i] == '/'))) {
            i--;
            skip = 1;
            if (i > 1 && (buffer[i-1] == '/' && buffer[i] == '/')) i--;
            continue;
        }
        fprintf(stderr,"%s", buffer);
        switch (current_node->type) {
            default:
                if (strstr(buffer, "struct") != NULL) { ///Structs
                    printf("test");
                    current_node->type = structs;
                    if (sscanf(buffer,"struct %s", current_node->name) != 1) { //struct [name]
                        if (sscanf(buffer, "struct %s{", current_node->name) != 1) current_node->name[0] = '\0'; //struct [name]{
                    }
                }
                else if (buffer[i] == '(') { ///Function calls
                    if(isalpha(buffer[i-1])){
                        current_node->type = function;
                        for (int j = i-1; isalpha(buffer[j]); --j) {
                            strncpy(current_node->name, buffer+j+1, i-j-1);
                            current_node->name[i-j-1] = '\0';
                            if (!isValidIdentifier(current_node->name)){

                            }
                        }
                    } else {
                        continue;
                    };
                }
                break;
            case structs:
                if (current_node->name[0] == '\0') {
                    if (c_bracket[0] == 1)
                        i = 0;
                    if (c_bracket[0] == 0 && c_bracket[1] == 1) {
                        if (c != ';') continue;
                        char *name = substr(buffer, '}', ';');
                        if (name == NULL)
                        {
                            fprintf(stderr, "Unable to read struct name\n");
                            strcpy(current_node->name,"Default name");
                            continue;
                        }
                        name++;
                        truncate(name, 100);
                        strcpy(current_node->name, name);
                    }
                }
                else {
                    if (c_bracket[0] == 1) continue;
                    if (c_bracket[0] == 0 && c_bracket[1] == 1) {
                        strcpy(current_node->struct_.args, substr(buffer,-1,'}'));
                        c_bracket[1]--;
                    }
                }
                if (current_node->name[0] != '\0' && current_node->struct_.args[0] != '\0') {
                    node_t *new_node = (node_t *) malloc(sizeof(node_t));
                    new_node->type = -1;
                    current_node->nextList = (node_t **) malloc(sizeof(node_t *)*2);
                    current_node->nextList[0] = new_node;
                    current_node->nextList[1] = NULL;
                    current_node = new_node;
                    i = 0;
                }
                break;
            case function:
                if (n_bracket[0] < n_bracket[1]) {

                }
                break;
            case variable:
                break;
            case conditional:
                break;
            case loop:
                break;
            case main_:
                break;
        }
    }
    fclose(fp);
    return first_node;
}

char* substr(char *str, char start, char end) {
    char* s = str;
    if (start >= 0) s = strchr(str, start);
    char* e = strchr(str, end);
    if (s == NULL || e == NULL)
        return NULL;
    char* result = malloc((e - s) + 1);
    if (result == NULL)
        return NULL;
    strncpy(result, s, e-s);
    result[e-s+1] = '\0';
    return result;
}

void truncate(char *str, int len){
    char *s = str;
    char *e = str + strlen(str)-1;
    while (*s != '\0')
    {
        if (*s == ' ' || *s == '\t') s++;
    }
    while (*e != '\0')
    {
        *e = '\0';
        if (*e == ' ' || *e == '\t' || *e == '\n' || *e == '\r') e--;
    }
    if (len < 0) strcpy(str,s);
    else strncpy(str, s, len);
}

int isValidIdentifier(const char* str) {
    // Check if the string is empty
    if (strlen(str) == 0) {
        return 0;
    }

    // Check if the first character is a letter or underscore
    if (!isalpha(str[0]) && str[0] != '_') {
        return 0;
    }

    // Check the remaining characters
    for (int i = 1; i < strlen(str); i++) {
        // Check if the character is a letter, digit, or underscore
        if (!isalnum(str[i]) && str[i] != '_') {
            return 0;
        }
    }

    // Check if the string is a reserved keyword
    // Add more keywords as needed
    const char* keywords[] = {
            "auto", "break", "case", "char", "const", "continue", "default",
            "do", "double", "else", "enum", "extern", "float", "for", "goto",
            "if", "int", "long", "register", "return", "short", "signed",
            "sizeof", "static", "struct", "switch", "typedef", "union",
            "unsigned", "void", "volatile", "while", NULL
    };

    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 0;
        }
    }
    // The string is a valid identifier
    return 1;
}

void extractFunctionCallParameters(const char* str) {
    // Check if the string starts with a valid C identifier followed by an opening parenthesis
    int len = strlen(str);
    if (len > 0 && isalpha(str[0]) && isValidIdentifier(str)) {
        int i = 1;
        while (i < len && str[i] != '(') {
            i++;
        }
        if (i < len && str[i] == '(') {
            i++;
            int start = i;
            int depth = 1;
            while (i < len && depth > 0) {
                if (str[i] == '(') {
                    depth++;
                } else if (str[i] == ')') {
                    depth--;
                }
                i++;
            }
            if (depth == 0) {
                // Extract the function call parameters
                char parameters[100];  // Adjust the size as per your requirements
                strncpy(parameters, str + start, i - start - 1);
                parameters[i - start - 1] = '\0';

                printf("Function call parameters: %s\n", parameters);
                return;
            }
        }
    }
}
