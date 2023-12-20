//
// Created by sziha on 16/10/2023.
//

#include "ini_reader.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "debugmalloc.h"

int read_ini(const char *filename, theme_t *theme) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr,"Couldn't open file @ line %d of %s\n", __LINE__, __FILE__);
        return -1;
    }
    mapping_t mappings_sc[2];
    mapping_t mappings_c[] = {
            {"functions", &(theme->functions)},
            {"structs", &(theme->structs)},
            {"vars", &(theme->variables)},
            {"conds", &(theme->conditionals)},
            {"loops", &(theme->loops)},
            {"main", &(theme->main_)},
    };
    //printf("file open\n") ;
    char line[256]; //if type == -1 go to next line else check subcontext and add to theme
    colour_t *context = NULL;
    SDL_Colour *subContext = NULL;
    while (fgets(line, 256, fp) != NULL) {
        //printf("%s", line);
        if (line[0] == '[') {
            char *name = line + 1;
            name = strtok(name, "]");
            name = strlwr(name);
            //printf("%s\n", name);
            for (int i = 0; i < 6; i++) {
                if (strcmp(name, mappings_c[i].key) == 0) {
                    context = (colour_t *) mappings_c[i].value;
                    break;
                }
            }
            if (context == NULL) {
                fprintf(stderr, "Unknown type: %s @ line %d of %s\n", name, __LINE__, __FILE__);
            }
            mappings_sc[0].key = "background";
            mappings_sc[0].value = &(context->background);
            mappings_sc[1].key = "text";
            mappings_sc[1].value = &(context->text);
            continue;
        }
        else if (line[0] != ';' && context != NULL)
        {
            char *value = strtok(line, "=");
            unsigned long long int val_len = strlen(value);
            //printf("%s", value);
            char *valend = value+val_len-1;
            while (isspace(*valend)){
                *valend = '\0';
                valend--;
            }
            for (int i = 0; i < 2; i++) {
                if (strcmp(value, mappings_sc[i].key) == 0) {
                    subContext = (SDL_Color *) mappings_sc[i].value;
                    break;
                }
            }
            if (subContext == NULL) {
                fprintf(stderr, "Unknown sub type: %s @ line %d of %s\n", value, __LINE__, __FILE__);
            }
            value = strtok(NULL, "=");
            while (isspace(*value))
                value++;
            set_rgba(value, subContext);
        }
    }
    fclose(fp);
    return 0;
}

void set_rgba(char *hex, SDL_Colour *colour) {
    char rgba[3] = {hex[1], hex[2],'\0'};
    colour->r = strtoul(rgba, NULL, 16);
    rgba[0] = hex[3]; rgba[1] = hex[4];
    colour->g = strtoul(rgba, NULL, 16);
    rgba[0] = hex[5]; rgba[1] = hex[6];
    colour->b = strtoul(rgba, NULL, 16);
    rgba[0] = hex[7]; rgba[1] = hex[8];
    colour->a = strtoul(rgba, NULL, 16);
}
