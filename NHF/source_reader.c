//
// Created by sziha on 21/10/2023.
//

#include "source_reader.h"


node_t *read_source(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Unable to open file %s @ line %d of %s\n", filename, __LINE__, __FILE__ );
        return NULL;
    }
    node_t *first_node = createNode(-1);
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
    regcomp(&func_call_regex, "\\w+\\s*\\(.*\\);", REG_EXTENDED);
    regex_t var_def_regex;
    regcomp(&var_def_regex, ".+?\\s+(\\w+\\s+)+\\s*\\=*\\s*.+\\s*;", REG_EXTENDED);
    regex_t var_ass_regex;
    regcomp(&var_ass_regex,"\\s*\\w+\\s*[\\+\\-\\*/]*\\=\\s*.+\\s*;|\\s*\\w+\\s*[\\+\\-]{2};|[\\+\\-]{2}\\s*\\w+\\s*;", REG_EXTENDED);
    regex_t loop_regex;
    regcomp(&loop_regex, "for\\s*\\(.+\\)\\s*\\{|while\\s*\\(.+\\)\\s*\\{|do\\s*\\{", REG_EXTENDED);
    regex_t cond_regex;
    regcomp(&cond_regex, "if\\s*\\(.+?\\)\\s*\\{|else\\s*\\{|else\\s*if\\s*\\(.+?\\)\\s*\\{|switch\\s*\\(.+?\\)\\s*\\{", REG_EXTENDED);
    regex_t case_regex;
    regcomp(&case_regex, "case\\s*.+?:|default\\s*?:", REG_EXTENDED);
    regex_t return_regex;
    regcomp(&return_regex, "return\\s*.+?\\s*;", REG_EXTENDED);
    regex_t other_regex;
    regcomp(&other_regex, "break\\s*;|goto\\s*\\w*;|continue\\s*;",REG_EXTENDED);
    regmatch_t match;
    int regi;
    char buffer[10000];
    int skip = 0;
    int c = 0;
    int multi_line = 0;
    int c_bracket[2] = {0,0};
    int n_bracket[2] = {0,0};
    for (int i = -1; c != EOF; i = (i == 9999) ? 0 : i + 1) {
        int pc = c;
        c = getc(fp);
        if (skip == 1  && c !='\n') {
            i--;
            continue;
        }
        if (multi_line == 1) {
            if (pc == '*' && c == '/') {
                multi_line = 0;
                skip = 0;
            } else {
                i--;
                continue;
            }
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
        if (c == '\n' ) {
            i--;
            continue;
        }
        buffer[i] = (char) c;
        buffer[i + 1] = '\0';
        if (strstr(buffer, "case \'t\':")){
            printf("\n");
        }
        if (buffer[i] == '#') {
            i--;
            skip = 1;
            continue;
        }
        if (i >= 1 && ((buffer[i-1] == '/' && buffer[i] == '/') || (buffer[i-1] == '/' && buffer[i] == '*'))) {
            i-=2;
            skip = 1;
            if (buffer[i] == '*') {
                multi_line = 1;
            }
        }
        if (strstr(buffer,"char* fileExt = strrchr(file, '.');")){
            printf("\n");
        }
        //fprintf(stderr,"%s\n", buffer);
        char *arg;
        switch (current_node->type) {
            default:{
                current_node->indent = c_bracket[0];
                /// Structures
                {
                    regi = regexec(&struct_dec_regex, buffer, 1, &match, 0);
                    if (!regi && c_bracket[0] == 0) {
                        current_node->type = structs;
                        current_node->struct_.args = (char *) malloc(sizeof(char) * (match.rm_eo - match.rm_so + 1));
                        strncpy(current_node->struct_.args, buffer + match.rm_so, match.rm_eo - match.rm_so);
                        current_node->struct_.args[match.rm_eo - match.rm_so] = '\0'; //args
                        break;
                    }
                }
                /// Function definitions
                {
                    regi = regexec(&func_dec_regex, buffer, 1, &match, 0);
                    if (!regi && n_bracket[0] == 0) {
                        current_node->type = function;
                        current_node->func.args = (char **) malloc(sizeof(char*) * 2);
                        current_node->func.args[0] = (char *) malloc(sizeof(char) * (match.rm_eo - match.rm_so + 1));
                        current_node->func.args[1] = NULL;
                        strncpy(current_node->func.args[0], buffer + match.rm_so, match.rm_eo - match.rm_so);
                        current_node->func.args[0][match.rm_eo - match.rm_so] = '\0';
                        break;
                    }
                }
                /// Variables
                {
                    regi = regexec(&var_def_regex, buffer, 1, &match, 0);
                    if (!regi) {
                        current_node->type = variable;
                        current_node->variable.value = (char *) malloc(sizeof(char) * (match.rm_eo - match.rm_so + 1));
                        strncpy(current_node->variable.value, buffer + match.rm_so, match.rm_eo - match.rm_so);
                        current_node->variable.value[match.rm_eo - match.rm_so] = '\0';
                        break;
                    } else{
                        regi = regexec(&var_ass_regex, buffer, 1, &match, 0);
                        if (!regi) {
                            current_node->type = variable;
                            current_node->variable.value = (char *) malloc(sizeof(char) * (match.rm_eo - match.rm_so + 1));
                            strncpy(current_node->variable.value, buffer + match.rm_so, match.rm_eo - match.rm_so);
                            current_node->variable.value[match.rm_eo - match.rm_so] = '\0';
                            break;
                        }
                    }
                }
                /// Function calls
                {
                    regi = regexec(&func_call_regex, buffer, 1, &match, 0);
                    if (!regi) {
                        char *name = (char *) malloc(sizeof(char) * (match.rm_eo - match.rm_so));
                        strncpy(name, buffer + match.rm_so, match.rm_eo - match.rm_so);
                        name[match.rm_eo - match.rm_so] = '\0';
                        current_node->func.args = (char **) malloc(sizeof(char*) * 2);
                        current_node->func.args[0] = name;
                        current_node->func.args[1] = NULL;
                        current_node->name = (char *) malloc(100+1);
                        strcpy(current_node->name, truncate(substr(name,-1,'('),100));
                        current_node->type = func_call;
                        break;
                    }
                }
                /// Conditionals
                {
                    regi = regexec(&cond_regex, buffer, 1, &match, 0);
                    if (!regi) {
                        current_node->loop_cond.condition = NULL;
                        current_node->type = conditional;
                        if(strstr(buffer + match.rm_so, "else if") != NULL) {
                            current_node->loop_cond.type = else_if;
                            current_node->loop_cond.condition = malloc(match.rm_eo-match.rm_so+1);
                            strncpy(current_node->loop_cond.condition, buffer + match.rm_so, match.rm_eo - match.rm_so);
                            current_node->loop_cond.condition[match.rm_eo - match.rm_so] = '\0';
                        } else if(strstr(buffer + match.rm_so, "if") != NULL) {
                            current_node->loop_cond.type = if_;
                            current_node->loop_cond.condition = malloc(match.rm_eo-match.rm_so+1);
                            strncpy(current_node->loop_cond.condition, buffer + match.rm_so, match.rm_eo - match.rm_so);
                            current_node->loop_cond.condition[match.rm_eo - match.rm_so] = '\0';
                        } else if(strstr(buffer + match.rm_so, "else") != NULL) {
                            current_node->loop_cond.type = else_;
                        } else if(strstr(buffer + match.rm_so, "switch") != NULL) {
                            current_node->loop_cond.type = switch_;
                            current_node->loop_cond.condition = malloc(match.rm_eo-match.rm_so+1);
                            strncpy(current_node->loop_cond.condition, buffer + match.rm_so, match.rm_eo - match.rm_so);
                            current_node->loop_cond.condition[match.rm_eo - match.rm_so] = '\0';
                        } else {
                            current_node->loop_cond.type = -1;
                            current_node->loop_cond.condition = malloc(match.rm_eo-match.rm_so+1);
                            strncpy(current_node->loop_cond.condition, buffer + match.rm_so, match.rm_eo - match.rm_so);
                            current_node->loop_cond.condition[match.rm_eo - match.rm_so] = '\0';
                        }
                        break;
                    } else{
                        regi = regexec(&case_regex, buffer, 1, &match, 0);
                        if (!regi) {
                            current_node->loop_cond.condition = NULL;
                            current_node->type = conditional;
                            current_node->loop_cond.type = case_;
                            current_node->loop_cond.condition = malloc(match.rm_eo-match.rm_so+1);
                            strncpy(current_node->loop_cond.condition, buffer + match.rm_so, match.rm_eo - match.rm_so);
                            current_node->loop_cond.condition[match.rm_eo - match.rm_so] = '\0';
                            break;
                        }
                    }
                }
                /// Loops
                {
                    regi = regexec(&loop_regex, buffer, 1, &match, 0);
                    if (!regi) {
                        current_node->type = loop;
                        if(strstr(buffer + match.rm_so, "while") != NULL) {
                            current_node->loop_cond.type = while_;
                            current_node->loop_cond.condition = malloc(match.rm_eo-match.rm_so+1);
                            strncpy(current_node->loop_cond.condition, buffer + match.rm_so, match.rm_eo - match.rm_so);
                            current_node->loop_cond.condition[match.rm_eo - match.rm_so] = '\0';
                        } else if(strstr(buffer + match.rm_so, "for") != NULL) {
                            current_node->loop_cond.type = for_;
                            current_node->loop_cond.condition = malloc(match.rm_so-match.rm_eo+1);
                            strncpy(current_node->loop_cond.condition, buffer + match.rm_so, match.rm_eo - match.rm_so);
                            current_node->loop_cond.condition[match.rm_eo - match.rm_so] = '\0';
                        } else if(strstr(buffer + match.rm_so, "do") != NULL) {
                            current_node->loop_cond.type = do_while;
                            current_node->loop_cond.condition = malloc(5);
                            strcpy(current_node->loop_cond.condition, "while");
                        } else {
                            current_node->loop_cond.type = -1;
                            current_node->loop_cond.condition = malloc(match.rm_so-match.rm_eo+1);
                            strncpy(current_node->loop_cond.condition, buffer + match.rm_so, match.rm_eo - match.rm_so);
                            current_node->loop_cond.condition[match.rm_eo - match.rm_so] = '\0';
                        }
                        break;
                    }
                }
                /// Others
                {
                    regi = regexec(&return_regex, buffer, 1, &match, 0);
                    if (!regi) {
                        current_node->type = other;
                        current_node->other.value = malloc(match.rm_eo-match.rm_so+1-8);
                        strncpy(current_node->other.value, buffer + match.rm_so+7, match.rm_eo - match.rm_so);
                        current_node->other.value[match.rm_eo - match.rm_so] = '\0';
                        current_node->name = malloc(6);
                        strcpy(current_node->name, "return");
                        break;
                    } else {
                        regi = regexec(&other_regex, buffer, 1, &match, 0);
                        if (!regi){
                            current_node->type = other;
                            current_node->other.value = malloc(match.rm_eo-match.rm_so+1);
                            strncpy(current_node->other.value, buffer + match.rm_so, match.rm_eo-match.rm_so);
                            current_node->other.value[match.rm_eo-match.rm_so] = '\0';
                            current_node->name = current_node->other.value;
                            break;
                        }
                    }
                }
                break;
                }
            case structs:{
                i = -1;
                regi = regexec(&struct_name_regex1, current_node->struct_.args, 1, &match, 0);
                if (regi) {
                    fprintf(stderr, "No match @ line %d of %s\n", __LINE__, __FILE__);
                    regi = regexec(&struct_name_regex2, current_node->struct_.args, 1, &match, 0); //name 2
                    if (regi) {
                        fprintf(stderr, "No match@ line %d of %s\n", __LINE__, __FILE__);
                        current_node->name = malloc(13+1);
                        strcpy(current_node->name, "Default name");
                    } else {
                        current_node->name = malloc(match.rm_so-match.rm_eo+1-8);
                        strncpy(current_node->name, buffer + match.rm_so + 8, match.rm_eo - match.rm_so - 7);
                        current_node->name[match.rm_eo - match.rm_so - 7] = '\0';
                    }
                } else {
                    if (*(buffer + match.rm_so + 1) == ' ') {
                        current_node->name = malloc(match.rm_so-match.rm_eo);
                        strncpy(current_node->name, buffer + match.rm_so + 1, match.rm_eo - match.rm_so - 2);
                        current_node->name[match.rm_eo - match.rm_so - 2] = '\0';
                    } else {
                        current_node->name = malloc(match.rm_so-match.rm_eo-1);
                        strncpy(current_node->name, buffer + match.rm_so + 2, match.rm_eo - match.rm_so - 3);
                        current_node->name[match.rm_eo - match.rm_so - 3] = '\0';
                    }
                }
                arg = substr(current_node->struct_.args, '{', ';');
                free(current_node->struct_.args);
                current_node->struct_.args = arg;
                *(current_node->struct_.args) = '\0';
                current_node->struct_.args++;
                int new_len = lastOccurrence(current_node->struct_.args, '}');
                current_node->struct_.args = (char *) realloc(current_node->struct_.args, new_len);
                current_node->struct_.args[new_len] = '\0';
                current_node = nextNode(current_node);
                break;
                }
            case function:{
                i = -1;
                char *strd;
                arg = strtok_r(current_node->func.args[0], " ",&strd);
                size_t arg_len = 3;
                if (arg != NULL) {
                    arg_len = strlen(arg);
                    current_node->func.return_type = (char *) malloc(sizeof(char) * (arg_len + 1));
                    strcpy(current_node->func.return_type, arg);
                } else {
                    current_node->func.return_type = (char *) malloc(sizeof(char) * (arg_len + 1));
                    strcpy(current_node->func.return_type, "int");
                }
                arg = strtok_r(NULL, " (", &strd);
                if (arg != NULL){
                    current_node->name = (char *) malloc(100 + 1);
                    strcpy(current_node->name, truncate(arg, 100));
                }
                else{
                    current_node->name = (char *) malloc(14);
                    strcpy(current_node->name, "Default name");
                }
                char *arg1 = strtok_r(NULL, ",(", &strd);
                if (arg1 != NULL && arg1[0] != ')') {
                    arg1++;
                    arg = arg1;
                } else {
                    free(arg1);
                    current_node->func.args = (char **) realloc(current_node->func.args, 1* sizeof(char *));
                    current_node->func.args[0] = NULL;
                    current_node = nextNode(current_node);
                    break;
                }
                int n_args = 1;
                while (arg != NULL && arg[0] != ')') {
                    current_node->func.args = (char **) realloc(current_node->func.args, n_args + 2);
                    current_node->func.args[n_args] = (char *) malloc(sizeof(char) * (strlen(arg) + 1));
                    strcpy(current_node->func.args[n_args], truncate(arg,-1));
                    n_args++;
                    current_node->func.args[n_args] = NULL;
                    arg = strtok_r(NULL, ",(", &strd);
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
                current_node = nextNode(current_node);
                break;
                }
            case variable:{
                i = -1;
                char *equals = strchr(current_node->variable.value, '=');
                char *assign;
                arg = strtok_r(current_node->variable.value, "=", &assign);
                char *declare;
                if (arg != NULL && equals != NULL) {
                    arg = truncate(arg,-1);
                    char *arg1 = strtok_r(arg, " ", &declare);
                    if (arg1 != NULL) {
                        current_node->variable.type = (char *) malloc(sizeof(char) * (strlen(arg1) + 1));
                        strcpy(current_node->variable.type, arg1);
                    }
                    else {
                        current_node->variable.type = (char *) malloc(sizeof(char) * (strlen(arg) + 1));
                        strcpy(current_node->variable.type, " ");
                    }
                    current_node->name = malloc(strlen(declare));
                    strcpy(current_node->name, declare);
                    arg = assign;
                    size_t len = strlen(arg);
                    arg1 = (char *) malloc(len);
                    strncpy(arg1, arg, len);
                    arg1[len] = '\0';
                    free(current_node->variable.value);
                    current_node->variable.value = arg1;
                }else {
                    current_node->variable.type = NULL;
                    arg = strtok_r(current_node->variable.value, " ", &assign);
                    arg = assign;
                    current_node->name = malloc(100+1);
                    strcpy(current_node->name, truncate(arg, 100));
                    current_node->name[strlen(current_node->name) - 1] = '\0';
                }
                current_node = nextNode(current_node);
                break;
                }
            case func_call:{
                i = -1;
                node_t *func = findFunction(current_node, functions);
                if (func != NULL) {
                    current_node->goList = appendNodeArray(current_node->goList, func);
                    node_t *new_node = createNode(-1);
                    node_t *last_node = findLastByIndent(current_node, func->indent);
                    last_node->goList = appendNodeArray(last_node->goList, new_node);
                }
                //current_node->func.args;
                char *cald;
                char *arg1 = strtok_r(current_node->func.args[0], "(", &cald);
                arg1 = strtok_r(NULL, ",(", &cald);
                if (arg1 != NULL && arg1[0] != ')') {
                    arg = arg1;
                } else {
                    free(arg1);
                    free(current_node->func.args[0]);
                    current_node->func.args = (char **) realloc(current_node->func.args, sizeof(char *));
                    current_node->func.args[0] = NULL;
                    current_node = nextNode(current_node);
                    break;
                }
                int n_args = 1;
                while (arg != NULL && arg[0] != ')' && arg[0] != ';') {
                    arg = strtok_r(NULL, ",", &cald);
                    if (arg == NULL) {
                        break;
                    }
                    current_node->func.args = (char **) realloc(current_node->func.args, (n_args + 2)*sizeof(char *));
                    current_node->func.args[n_args] = (char *) malloc(sizeof(char) * (strlen(arg) + 1));
                    strcpy(current_node->func.args[n_args], arg);
                    n_args++;
                    current_node->func.args[n_args] = NULL;
                }
                //free(current_node->func.args[0]);
                current_node->func.args[0] = malloc(strlen(arg1) + 1);
                strcpy(current_node->func.args[0], arg1);
                arg = substr(current_node->func.args[n_args - 1], -1, ')');
                arg[strlen(arg)-1] = '\0';
                free(current_node->func.args[n_args - 1]);
                current_node->func.args[n_args - 1] = arg;
                current_node = nextNode(current_node);
                break;
            } //TODO a counter for the showing
            case conditional:{
                current_node->goList = NULL;
                i = -1;
                char *cond;
                char *true_cond;
                switch (current_node->loop_cond.type) {
                    case if_:{
                        cond = strstr(current_node->loop_cond.condition, "if");
                        if (cond != NULL) {
                            cond+=2;
                            cond = truncate(cond,-1);
                            cond++;
                            cond[strlen(cond)] = '\0';
                            true_cond = malloc(strlen(cond) + 1);
                            strcpy(true_cond, cond);
                            free(current_node->loop_cond.condition);
                            current_node->loop_cond.condition = true_cond;
                        }
                        else {
                            fprintf(stderr, "Unknown conditional with value: %s @ line %d of %s\n", current_node->loop_cond.condition, __LINE__, __FILE__);
                        }
                        break;
                    }
                    case else_:{
                        current_node->loop_cond.finished = true;
                        node_t *prev = findPreviousByType(first_node,current_node,conditional,current_node->indent);
                        while (prev->loop_cond.type != if_ && prev != NULL) {
                            prev->loop_cond.finished = true;
                            prev = findPreviousByType(first_node,prev,conditional,prev->indent);
                        }
                        break;
                    }
                    case else_if:{
                        cond = strstr(current_node->loop_cond.condition, "if");
                        if (cond != NULL) {
                            cond+=2;
                            cond = truncate(cond,-1);
                            cond++;
                            cond[strlen(cond)] = '\0';
                            true_cond = malloc(strlen(cond) + 1);
                            strcpy(true_cond, cond);
                            free(current_node->loop_cond.condition);
                            current_node->loop_cond.condition = true_cond;
                        }
                        else {
                            fprintf(stderr, "Unknown conditional with value: °%s type @ line %d of %s\n", current_node->loop_cond.condition, __LINE__, __FILE__);
                        }
                        break;
                    }
                    case switch_:
                    case case_:
                    default: {
                        fprintf(stderr, "Unknown conditional with value: °%s type @ line %d of %s\n", current_node->loop_cond.condition, __LINE__, __FILE__);
                        break;
                    }
                }
                current_node = nextNode(current_node);
                break;
                }
            case loop:{
                i = -1;
                switch (current_node->loop_cond.type) {
                    case for_:{
                        // TODO: add for
                    }
                    case while_:{
                        // TODO: add while
                    }
                    case do_while:{
                        // TODO: add do while
                    }
                    default: {
                        fprintf(stderr, "Unknown loop type with value: %s @ line %d of %s\n", current_node->loop_cond.condition, __LINE__, __FILE__);
                        break;
                    }
                }
                current_node = nextNode(current_node);
                break;
                }
            case other: {
                i = -1;
                // TODO: add return/break/continue/goto
                current_node = nextNode(current_node);
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
    char *e = str + strlen(str);
    while (*s != '\0')
    {
        if (*s == ' ' || *s == '\t') s++;
        else break;
    }
    while (e != s)
    {
        e--;
        if (*e == ' ' || *e == '\t' || *e == '\n' || *e == '\r' || *e == '\0') *e = '\0';
        else break;
    }
    if (len < 0) strcpy(str,s);
    else {
        strncpy(str, s, len);
        str[len] = '\0';
    }
    return str;
}

int lastOccurrence(char *str, char c) {
    int i = (int) strlen(str) - 1;
    while (i >= 0 && str[i] != c) {
        i--;
    }
    return i;
}

node_t *createNode(int type) {
    node_t *n = malloc(sizeof(node_t));
    if (n == NULL) {
        fprintf(stderr,"IDK HOW THIS HAPPENED @ line %d of %s\n", __LINE__, __FILE__);
    }
    n->indent = -1;
    n->call_num = 0;
    n->goList = createNodeArray(0);
    n->type = type;
    n->next = NULL;
    return n;
}

node_t *nextNode(node_t *current_node) {
    node_t *new_node = createNode(-1);
    current_node->next = new_node;
    current_node = new_node;
    return current_node;
}

node_t *findFunction(node_t *node, node_t *functions[]) {
    int i = 0;
    while (functions[i] != NULL) {
        if (functions[i]->name == node->name) return functions[i];
        i++;
    }
    return NULL;
}

node_t *findByName(node_t *start, char* name, int indent) {
    if (start != NULL) {
        if (strcmp(start->name, name) == 0 && indent == -1 ? 1 : indent == start->indent) return start;
        else return findByName(start->next, name, indent);
    }
    return NULL;
}
node_t *findByType(node_t *start, int type, int indent) {
    if (start != NULL) {
        if (start->type == type && indent == -1 ? 1 : indent == start->indent) return start;
        else return findByType(start->next, type, indent);
    }
    return NULL;
}
node_t *findLastByIndent(node_t *start, int indent) {
    if (start == NULL) {
        return NULL;
    }
    if (start->indent < indent) {
        return NULL;
    }
    node_t *temp = findLastByIndent(start->next, indent);
    if (temp != NULL) {
        return temp;
    }
    if (start->indent == indent) {
        return start;
    }
    return NULL;
}

node_t *findPreviousByType(node_t *start, node_t *current, int type, int indent) {
    if (start == NULL) {
        return NULL;
    }
    if (start == current) {
        return NULL;
    }
    node_t *temp = findPreviousByType(start->next, current, type, indent);
    if (temp != NULL) {
        return temp;
    }
    if (start->type == type && indent == -1 ? 1 : indent == start->indent) {
        return start;
    }
    return NULL;
}

int sizeOfNodeArray(node_t **arr) {
    int i = 0;
    while (arr[i] != NULL) {
        i++;
    }
    return i;
}

node_t **createNodeArray(int size) {
    node_t **arr = (node_t **)malloc(sizeof(node_t *)*(size+1));
    for (int i = 0; i <= size; i++) {
        arr[i] = NULL;
    }
    return arr;
}

node_t **appendNodeArray(node_t **arr, node_t *node) {
    int i = sizeOfNodeArray(arr);
    arr = (node_t **)realloc(arr, sizeof(node_t *)*(i+2));
    arr[i] = node;
    arr[i+1] = NULL;
    return arr;
}

void freeLinkedList(node_t *start) {
    if (start != NULL) {
        freeLinkedList(start->next);
        switch (start->type) {
            case function:
                free(start->func.return_type);
                /* FALLTHRU */
            case func_call:
                free(start->func.args);
                break;
            case variable:
                free(start->variable.type);
                //free(start->variable.value);
                break;
            case loop:
            case conditional:
                free(start->goList);
                free(start->loop_cond.condition);
                break;
            case other:
                free(start->other.value);
                break;
            case structs:
                free(start->struct_.args);
                break;
            default:
                break;
        }
        free(start);
    }
}
