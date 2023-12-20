//
// Created by sziha on 16/10/2023.
//
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "console.h"
int init_console(int argc, char** argv, char* theme_file, char* output_file, char* in_file) {
    int c;
    while ((c = getopt(argc,argv,":o:t:h")) != -1) {
        switch (c) {
            case 't':
                strcpy(theme_file, optarg);
                break;
            case 'h':
                printf("Usage: console -t <theme_file> -o <output_file> -i <input_file>\n");
                return  -1;
            case 'o':
                strcpy(output_file, optarg);
                break;
            case ':':
                strcpy(in_file, optarg);
                break;
            case '?':
                if (optopt == 'o') {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                } else if (optopt == 't') {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                } else {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                }
            default:
                break;
        }
    }
    return 0;
}

file_type_e ends_with(char* file)
{
    char* fileExt = strrchr(file, '.');
    if (strcmp(fileExt, file) != 0 || fileExt != NULL)
    {
        if (strcmp(fileExt, ".jpg") == 0)
            return file_type_jpg;
        else if (strcmp(fileExt, ".png") == 0)
            return file_type_png;
        /*else if (strcmp(fileExt, "md") == 0)
            return file_type_md;*/
    }
    return DEFAULT_FILE_TYPE;
}
