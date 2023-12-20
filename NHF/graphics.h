//
// Created by sziha on 07/12/2023.
//

#ifndef NHF_GRAPHICS_H
#define NHF_GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL2_rotozoom.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "types.h"
#include "source_reader.h"

/// type for graphics
typedef struct {
    SDL_Renderer *renderer;/// renderer
    theme_t *theme;/// theme
    SDL_PixelFormatEnum pixel_format;/// pixel format
    TTF_Font *font;/// font
    double scale;/// scale
} graphics_t;

/// type for rectangles
typedef struct {
    int x;
    int y;
    int w;
    int h;
} rect_t;

/// type for boxes
typedef struct {
    rect_t rect;/// rectangle
    int type;/// type
    char* text;/// text
    int radius;/// radius
} box_t;

/**
 * Redraws the screen
 * @param graphics collection of the important variables for the graphics
 * @param boxes the boxes to draw
 * @param num_boxes the number of boxes
 * @return 0 if success, -1 if error
 */
int redraw(graphics_t *graphics, box_t *boxes, int num_boxes);

/**
 * intializes the graphics
 * @param graphics collection of the important variables for the graphics
 * @param window the window
 * @return 0 if success, -1 if error
 */
int initGraphics(graphics_t *graphics, SDL_Window *window);

/**
 * intializes the boxes
 * @param graphics collection of the important variables for the graphics
 * @param start_node the start node of the linked list
 * @param boxes the list of boxes to append to
 * @return 0 if success, -1 if error
 */
int initBoxes(graphics_t *graphics, node_t *start_node, box_t *boxes);

/**
 * draw an arrow
 * @param start start box
 * @param end end box
 * @param graphics collection of the important variables for the graphics
 * @return 0 if success, -1 if error
 */
int drawArrow(box_t start, box_t end, graphics_t *graphics);

/**
 * draw a box
 * @param box box to draw
 * @param graphics collection of the important variables for the graphics
 * @return 0 if success, -1 if error
 */
int drawBox(box_t box, graphics_t *graphics);

/**
 * draw a rectangle
 * @param rect rectangle to draw
 * @param graphics collection of the important variables for the graphics
 * @param colour colour of the rectangle
 * @return
 */
int drawRect(rect_t rect, graphics_t *graphics,SDL_Colour colour);

/**
* @param node start node
* @param boc box to fill
* @return -1 if error, 0 if end_of nodes, 1 new box needed
*/
int nodeToBox(node_t *node, box_t *box);

/**
 *  save the screen to a file
 * @param graphics collection of the important variables for the graphics
 * @param filename name of the file
 * @param file_type the type of the file jpg or png
 * @return 0 if success, -1 if error
 */
int saveToFile(graphics_t *graphics, const char *filename, file_type_e file_type);

/**
 * appends two strings with realloc
 * @param a string to append to
 * @param b string to append
 * @return a
 */
char* cat(char* a, char* b);

/**
 * free the boxes
 * @param boxes list of boxes
 * @param box_count number of boxes
 */
void freeBoxes(box_t *boxes, int box_count);

#endif //NHF_GRAPHICS_H
