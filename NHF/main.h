//
// Created by sziha on 18/10/2023.
//

#ifndef NHF_MAIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_version.h>
#include <SDL_ttf.h>
#include "console.h"
#include "ini_reader.h"
#include <windows.h>
#include <shobjidl.h>
#include "source_reader.h"
#include "debugmalloc.h"
#include "graphics.h"

#define ID_OPEN_FILE 1
#define ID_SAVE_FLOW 2
#define ID_LOAD_THEME 3
#define ID_RESET_THEME 4
#define ID_ZOOM_IN 5
#define ID_ZOOM_OUT 6
#define ID_ZOOM_RESET 7
#define ID_EXIT 8

/**
 * Obvious
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char** argv);

/**
 * Gets win32 window handle
 * @param window sdl window
 * @return win32 window handle
 */
HWND GetHwnd(SDL_Window *window);
/**
 * Creates a menu for the given window handle
 * @param windowRef win32 window handle
 */
void ActivateMenu(HWND windowRef);
//code from https://stackoverflow.com/questions/51250046/sdl2-win32-api-menubar-click-event-not-working
/**
 * Creates a file open dialog for opening source files
 * @param windowRef win32 window handle
 * @param name filter name
 * @param file_spec filter spec
 * @return file to open
 */
char* file_open_dialog(HWND windowRef, const wchar_t *name, const wchar_t *file_spec);
/**
 * Creates a file save dialog for saving image files
 * @param windowRef win32 window handle
 * @return file to save
 */
char* file_save_dialog(HWND windowRef);

/**
 * Default theme for the viewport
 */
static theme_t default_theme = {
        .main_ = {
            .background = {.r = 255,.g = 255,.b = 255,.a = 255},
            .text = {.r = 0,.g = 0,.b = 0,.a = 255}},
        .functions = {
            .background = {.r = 255,.g = 255,.b = 255,.a = 255},
            .text = {.r = 0,.g = 0,.b = 0,.a = 255}},
        .structs = {
            .background = {.r = 255,.g = 255,.b = 255,.a = 255},
            .text = {.r = 0,.g = 0,.b = 0,.a = 255}},
        .variables = {
            .background = {.r = 255,.g = 255,.b = 255,.a = 255},
            .text = {.r = 0,.g = 0,.b = 0,.a = 255}},
        .conditionals = {
            .background = {.r = 255,.g = 255,.b = 255,.a = 255},
            .text = {.r = 0,.g = 0,.b = 0,.a = 255}},
        .loops = {
            .background = {.r = 255,.g = 255,.b = 255,.a = 255},
            .text = {.r = 0,.g = 0,.b = 0,.a = 255}}
};

#define NHF_MAIN_H
#endif//NHF_MAIN_H
