//
// Created by sziha on 16/10/2023.
//

#ifndef NHF_CONSOLE_H
#define NHF_CONSOLE_H
#include "types.h"

/**
 * @brief This function is used to initialize  from the console
 * @param argc argument_cont from main
 * @param argv arguments list from main
 * @param theme_file theme file path
 * @param output_file output file path
 * @return 0 if success, -1 if error
 */
int init_console(int argc, char** argv, char* theme_file, char* output_file, char* in_file);

/**
 * @brief This function is used to get the file type
 * @param file file path
 * @return file_type
 */
file_type_e ends_with(char* file);
#endif //NHF_CONSOLE_H
