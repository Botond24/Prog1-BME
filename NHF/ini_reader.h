//
// Created by sziha on 16/10/2023.
//

#ifndef NHF_INI_READER_H
#define NHF_INI_READER_H

#include <SDL_pixels.h>
#include <SDL2_rotozoom.h>
#include "types.h"

/**
 * enum for ini file type
 */
typedef enum {
    function,
    structs,
    variable,
    conditional,
    loop,
    main_
} context_e;

/**
 * enum for ini file sub_context
 * (in ini documentation is named value, but i use it like another type so it doesnt matter)
 */
typedef enum {
    background,
    text
} sub_context_e;

/**
 * colouring struct for theme
 */
typedef struct {
    SDL_Colour background;
    SDL_Colour text;
} colour_t;

/**
 * struct for theme
 */
typedef struct {
    colour_t functions;
    colour_t structs;
    colour_t variables;
    colour_t conditionals;
    colour_t loops;
    colour_t main_;
} theme_t;

/**
 * reads the theme ini file for custom themes
 * @param filename name of the ini file (including the .ini)
 * @param theme pointer to the theme variable
 * @return 0 if ok, 1 if error
 */
int read_ini(const char *filename, theme_t *theme);
/**
 * Sets the rgba of an SDL_Colour
 * @param hex hexadecimal string beginning with an #
 * @param colour pointer to the SDL_Colour
 */
void set_rgba(char *hex, SDL_Colour *colour);
#endif //NHF_INI_READER_H
