//
// Created by sziha on 21/10/2023.
//

#include "source_reader.h"


node_t *read_source(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Unable to open file %s\n", filename);
        return NULL;
    }
    node_t *first_node = (node_t *) malloc(sizeof(node_t));
    node_t *current_node = first_node;
    node_t **functions = (node_t **) malloc(sizeof(node_t *));
    int n_functions = 0;
    functions[0] = NULL;
    current_node->type = -1;
    regex_t struct_dec_regex;
    regcomp(&struct_dec_regex, "struct *\\w*\\s*\\{(.+)\\}.*;", REG_EXTENDED);
    regex_t struct_name_regex1;
    regcomp(&struct_name_regex1, "}\\ *\\w+\\ *;", REG_EXTENDED);
    regex_t struct_name_regex2;
    regcomp(&struct_name_regex2, "(struct)\\ \\w+", REG_EXTENDED);
    regex_t func_dec_regex;
    regcomp(&func_dec_regex, "\\w+\\s+\\w+\\s*\\(.*\\)\\s*\\{", REG_EXTENDED);
    regex_t func_call_regex;
    regcomp(&func_call_regex, "\\w+\\s*\\(.*\\)", REG_EXTENDED);
    regex_t var_def_regex;
    regcomp(&var_def_regex, "\\s*(\\w+\\s+)+\\w+\\s*\\=*\\s*.+\\s*;", REG_EXTENDED); ///is variable defintion;
    regex_t var_ass_regex;
    regcomp(&var_ass_regex,"\\s*\\w+\\s*[\\+\\-\\*/]*\\=\\s*.+\\s*;|\\s*\\w+\\s*[\\+\\-]{2};|[\\+\\-]{2}\\s*\\w+\\s*;", REG_EXTENDED);
    regmatch_t match;
    int regi;
    char buffer[10000];
    int skip = 0;
    int c = 0;
    int c_bracket[2] = {0,0};
    int n_bracket[2] = {0,0};
    for (int i = 0; c != EOF; i = (i == 9999) ? 0 : i + 1) {
        c = getc(fp);
        if (( skip == 1  && c!='\n' )) {
            i--;
            continue;
        }
        if (c == '\n') {
            skip = 0;
            i--;
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
        if (buffer[i] == '#' || (i >= 1 && (buffer[i-1] == '/' && buffer[i] == '/'))) {
            i--;
            skip = 1;
            if (i >= 1 && (buffer[i-1] == '/' && buffer[i] == '/')) i--;
            continue;
        }
        fprintf(stderr,"%s", buffer);
        char *arg;
        switch (current_node->type) {
            default:{
                Struct: {
                    regi = regexec(&struct_dec_regex, buffer, 1, &match, 0);
                    if (!regi && c_bracket[0] == 0) {
                        current_node->type = structs;
                        current_node->struct_.args = (char *) malloc(sizeof(char) * (match.rm_eo - match.rm_so + 1));
                        strncpy(current_node->struct_.args, buffer + match.rm_so, match.rm_eo - match.rm_so);
                        current_node->struct_.args[match.rm_eo - match.rm_so] = '\0'; //args
                        break;
                    }
                }
                Func_dec: {
                    regi = regexec(&func_dec_regex, buffer, 1, &match, 0);
                    if (!regi && n_bracket[0] == 0) {
                        current_node->type = function;
                        current_node->func.args = (char **) malloc(sizeof(char *) * 2);
                        current_node->func.args[0] = (char *) malloc(sizeof(char) * (match.rm_eo - match.rm_so + 1));
                        current_node->func.args[1] = NULL;
                        strncpy(current_node->func.args[0], buffer + match.rm_so, match.rm_eo - match.rm_so);
                        current_node->func.args[0][match.rm_eo - match.rm_so] = '\0';
                        break;
                    }
                }
                Var: {
                    regi = regexec(&var_def_regex, buffer, 1, &match, 0);
                    if (!regi) {
                        current_node->type = variable;
                        current_node->variable.value = (char *) malloc(sizeof(char) * (match.rm_eo - match.rm_so + 1));
                    } else{
                        regi = regexec(&var_ass_regex, buffer, 1, &match, 0);
                        if (!regi) {
                            current_node->type = variable;
                            current_node->variable.value = (char *) malloc(sizeof(char) * (match.rm_eo - match.rm_so + 1));
                        }
                    }
                }
                Func_call: {
                    regi = regexec(&func_call_regex, buffer, 1, &match, 0);
                    if (!regi) {
                        char *name = (char *) malloc(sizeof(char) * (match.rm_eo - match.rm_so + 1));
                        strncpy(name, buffer + match.rm_so, match.rm_eo - match.rm_so);
                        name[match.rm_eo - match.rm_so] = '\0';
                        name = substr(name,-1,'(');
                        //TODO figue out how to return to a different pos each time
                        //maybe return[1] gos to first call, return[2] goes to second call and so on
                        //what if multiple returns?
                        //TODO reset i more often
                    }
                }
                break;
                }
            case structs:{

                regi = regexec(&struct_name_regex1, current_node->struct_.args, 1, &match, 0);
                if (regi) {
                    fprintf(stderr, "No match\n");
                    regi = regexec(&struct_name_regex2, current_node->struct_.args, 1, &match, 0); //name 2
                    if (regi) {
                        fprintf(stderr, "No match\n");
                        strcpy(current_node->name, "Default name");
                    } else {
                        strncpy(current_node->name, buffer + match.rm_so + 8, match.rm_eo - match.rm_so - 7);
                        current_node->name[match.rm_eo - match.rm_so - 7] = '\0';
                    }
                } else {
                    if (*(buffer + match.rm_so + 1) == ' ') {
                        strncpy(current_node->name, buffer + match.rm_so + 1, match.rm_eo - match.rm_so - 2);
                        current_node->name[match.rm_eo - match.rm_so - 2] = '\0';
                    } else {
                        strncpy(current_node->name, buffer + match.rm_so + 2, match.rm_eo - match.rm_so - 3);
                        current_node->name[match.rm_eo - match.rm_so - 3] = '\0';
                    }
                }
                arg = substr(current_node->struct_.args, '{', ';');
                free(current_node->struct_.args);
                current_node->struct_.args = arg;
                *(current_node->struct_.args) = '\0';
                current_node->struct_.args++;
                int new_len = lastOccurence(current_node->struct_.args, '}');
                current_node->struct_.args = (char *) realloc(current_node->struct_.args, new_len);
                current_node->struct_.args[new_len] = '\0';
                nextNode(current_node);
                break;
                }
            case function:{
                arg = strtok(current_node->func.args[0], " ");
                size_t arg_len = 3;
                if (arg != NULL) {
                    arg_len = strlen(arg);
                    current_node->func.return_type = (char *) malloc(sizeof(char) * (arg_len + 1));
                    strcpy(current_node->func.return_type, arg);
                } else {
                    current_node->func.return_type = (char *) malloc(sizeof(char) * (arg_len + 1));
                    strcpy(current_node->func.return_type, "int");
                }
                arg = strtok(NULL, " ");
                if (arg != NULL)
                    strcpy(current_node->name, truncate(arg, 100));
                else
                    strcpy(current_node->name, "Default name");
                char *arg1 = strtok(NULL, ",(");
                if (arg1 != NULL && arg1[0] != ')') {
                    arg1++;
                    arg = arg1;
                } else {
                    free(arg1);
                    free(current_node->func.args[0]);
                    current_node->func.args = (char **) realloc(current_node->func.args, 1);
                    current_node->func.args[0] = NULL;
                    break;
                }
                int n_args = 1;
                while (arg != NULL && arg[0] != ')') {
                    current_node->func.args = (char **) realloc(current_node->func.args, n_args + 2);
                    current_node->func.args[n_args] = (char *) malloc(sizeof(char) * (strlen(arg) + 1));
                    strcpy(current_node->func.args[n_args], arg);
                    n_args++;
                    current_node->func.args[n_args] = NULL;
                    arg = strtok(NULL, ",(");
                }
                free(current_node->func.args[0]);
                current_node->func.args[0] = malloc(strlen(arg1) + 1);
                strcpy(current_node->func.args[0], arg1);
                arg = substr(current_node->func.args[n_args - 1], -1, ')');
                arg[strlen(arg)] = '\0';
                free(current_node->func.args[n_args - 1]);
                current_node->func.args[n_args - 1] = arg;
                ///args
                functions = (node_t **) realloc(functions, n_functions + 2);
                functions[n_functions] = current_node;
                functions[++n_functions] = NULL;
                nextNode(current_node);
                break;
                }
            case variable:{
                char *assign;
                arg = strtok_r(current_node->variable.value, "=", &assign);
                char *declare;
                if (arg != NULL) {
                    char *arg1 = strtok_r(arg, " ", &declare);
                    if (arg1 != NULL) {
                        arg = strtok_r(NULL, " ", &declare);
                    }
                    free(arg1);
                    strcpy(current_node->name, truncate(arg, 100));
                    current_node->name[strlen(current_node->name) - 1] = '\0';
                    arg = strtok_r(NULL, "='", &assign);
                    size_t len = strlen(arg);
                    arg1 = (char *) malloc(len);
                    strncpy(arg1, arg, len);
                    arg1[len] = '\0';
                    free(current_node->variable.value);
                    current_node->variable.value = arg1;

                }else {
                    arg = strtok_r(current_node->variable.value, " ", &assign);
                    arg = strtok_r(NULL, " ", &assign);
                    strcpy(current_node->name, truncate(arg, 100));
                    current_node->name[strlen(current_node->name) - 1] = '\0';
                }
                nextNode(current_node);
                break;
                }
            case conditional:{
                // TODO: add conditional
                break;
                }
            case loop:{
                // TODO: add loop
                break;
                }
            case other: {
                // TODO: add return/break/continue/goto
                break;
                }
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

char *truncate(char *str, int len){
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
    str[len] = '\0';
    return str;
}
/*
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
    for (size_t i = 1; i < strlen(str); i++) {
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
*/
int lastOccurence(char *str, char c) {
    int i = strlen(str) - 1;
    while (i >= 0 && str[i] != c) {
        i--;
    }
    return i;
}

node_t *createNode(int type) {
    node_t *n = malloc(sizeof(node_t));
    n->type = type;
    n->nextList = NULL;
    n->name[0] = '\0';
    return n;
}

void nextNode(node_t *current_node) {
    node_t *new_node = createNode(-1);
    current_node->nextList = (node_t **) malloc(sizeof(node_t *)*2);
    current_node->nextList[0] = new_node;
    current_node->nextList[1] = NULL;
    current_node = new_node;
}
