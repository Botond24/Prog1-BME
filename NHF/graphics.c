//
// Created by sziha on 07/12/2023.
//

#include "graphics.h"

int redraw(graphics_t *graphics, box_t *boxes, int num_boxes) {
    SDL_SetRenderDrawColor(graphics->renderer, graphics->theme->main_.background.r, graphics->theme->main_.background.g, graphics->theme->main_.background.b, graphics->theme->main_.background.a);
    for (int i = 0; i < num_boxes; i++) {
        if (boxes[i].type != -1) {
            drawBox(boxes[i],graphics);
        } else {
            return -1;
        }
    }
    SDL_RenderClear(graphics->renderer);
    SDL_RenderPresent(graphics->renderer);
    return 0;
}

int initGraphics(graphics_t *graphics, SDL_Window *window) {
    graphics->scale = 1;
    graphics->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (graphics->renderer == NULL) {
        return -1;
    }
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0 || TTF_Init() == -1) {
        return -1;
    }
    graphics->font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 16);
    if (graphics->font == NULL) {
        return -1;
    }
    graphics->pixel_format = SDL_GetWindowPixelFormat(window);
    return 0;
}

int initBoxes(graphics_t *graphics, node_t *start_node, box_t *boxes) {
    int i = 0;
    while (start_node != NULL) {
        boxes = realloc(boxes, (i + 1) * sizeof(box_t));
        boxes[i].text = (char*) malloc(1);
        boxes[i].text[0] = '\0';
        if (boxes == NULL) {
            return -1;
        }
        int res = nodeToBox(start_node, &boxes[i]);
        TTF_SizeText(graphics->font, boxes[i].text, &boxes[i].rect.w, &boxes[i].rect.h);
        //if (i == 0) {
        boxes[i].rect.x = 0;
        boxes[i].rect.y = 0;
        //}
        if (res == -1) {
            return -1;
        }
        else if (res == 1) {
            i++;
            start_node = start_node->next;
        }
        else if (res == 0) {
            break;
        }
    }
    return i;
}

int nodeToBox(node_t *node, box_t *box) {
    if (node == NULL) {
        return 0;
    }
    box->type = node->type;
    switch (node->type) {
        case function:
            box->text = cat(box->text, node->func.return_type);
            box->text = cat(box->text, " ");
            /* FALLTHRU */
        case func_call:
            box->text = cat(box->text, node->name);
            box->text = cat(box->text, "(");
            for (int i = 0; node->func.args[i] != NULL; ++i) {
                box->text = cat(box->text, node->func.args[i]);
                if (node->func.args[i + 1] != NULL) {
                    box->text = cat(box->text, ", ");
                }
            }
            box->text = cat(box->text, ")\n");
            break;
        case variable:
            //box->text = cat(box->text, node->variable.type[0] == ' ' ? "" : node->variable.type);
            box->text = cat(box->text, " ");
            box->text = cat(box->text, node->name);
            if (node->variable.value != NULL){
                box->text = cat(box->text, " = ");
                box->text = cat(box->text, node->variable.value);
            }
            box->text = cat(box->text, "\n");
            break;
        case structs:
            box->text = cat(box->text, node->name);
            box->text = cat(box->text, "\n");
            box->text = cat(box->text, node->struct_.args);
            break;
        case other:
            box->text = cat(box->text, node->name);
            box->text = cat(box->text, "\n");
            break;
        case loop:
        case conditional:
            switch (node->loop_cond.type) {
                case if_:
                    box->text = cat(box->text, "if ");
                    box->text = cat(box->text, node->loop_cond.condition);
                    break;
                case else_if:
                    box->text = cat(box->text, "else if ");
                    box->text = cat(box->text, node->loop_cond.condition);
                    break;
                case else_:
                    box->text = cat(box->text, "else ");
                    break;
                case switch_:
                    box->text = cat(box->text, "switch ");
                    box->text = cat(box->text, node->loop_cond.condition);
                    break;
                case case_:
                    box->text = cat(box->text, "case ");
                    box->text = cat(box->text, node->loop_cond.condition);
                    break;
                case while_:
                    box->text = cat(box->text, "while ");
                    box->text = cat(box->text, node->loop_cond.condition);
                    break;
                case for_:
                    box->text = cat(box->text, "for ");
                    box->text = cat(box->text, node->loop_cond.condition);
                    break;
                case do_while:
                    box->text = cat(box->text, "do ");
                    break;
                default:
                    break;
            }
            box->text = cat(box->text, "\n");
            return 1;
        default:
            return -1;
    }
    return 1;
}

int drawRect(rect_t rect, graphics_t *graphics,SDL_Colour colour) {
    SDL_Rect sdl_rect;
    sdl_rect.x = rect.x;
    sdl_rect.y = rect.y;
    sdl_rect.w = rect.w;
    sdl_rect.h = rect.h;
    boxRGBA(graphics->renderer, sdl_rect.x, sdl_rect.y, sdl_rect.w, sdl_rect.h, colour.r, colour.g, colour.b, colour.a);
    return 0;
}

int drawBox(box_t box, graphics_t *graphics) {
    SDL_Colour colour;
    switch (box.type) {
        case function:
            colour = graphics->theme->functions.background;
            break;
        case structs:
            colour = graphics->theme->structs.background;
            break;
        case other:
        case func_call:
        case variable:
            colour = graphics->theme->variables.background;
            break;
        case conditional:
            colour = graphics->theme->conditionals.background;
            break;
        case loop:
            colour = graphics->theme->loops.background;
            break;
        default:
            break;

    }
    drawRect(box.rect, graphics, colour);
    return 0;
}

int drawArrow(box_t start, box_t end, graphics_t *graphics) {
    SDL_Colour colour = graphics->theme->main_.text;
    lineRGBA(graphics->renderer, start.rect.x+start.rect.w/2, start.rect.y+start.rect.h, end.rect.x+end.rect.w/2, end.rect.y, colour.r, colour.g, colour.b, colour.a);
    double angle = atan2(end.rect.y - start.rect.y, end.rect.x - start.rect.x);
    double length = sqrt(pow(end.rect.x - start.rect.x, 2) + pow(end.rect.y - start.rect.y, 2))/10;
    double dif_angle = M_PI/6;
    lineRGBA(graphics->renderer,end.rect.x+end.rect.w/2,end.rect.y,
             end.rect.x+end.rect.w/2+length * cos(angle+dif_angle),
             end.rect.y+length*sin(angle+dif_angle),
             colour.r, colour.g, colour.b, colour.a);
    lineRGBA(graphics->renderer,end.rect.x+end.rect.w/2,end.rect.y,
             end.rect.x+end.rect.w/2+length * cos(angle-dif_angle),
             end.rect.y+length*sin(angle-dif_angle),
             colour.r, colour.g, colour.b, colour.a);
    return 0;
}

int saveToFile(graphics_t *graphics, const char *filename, file_type_e file_type) {
    SDL_Texture *texture = SDL_GetRenderTarget(graphics->renderer);
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(graphics->renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    switch (file_type) {
        case file_type_jpg:
            IMG_SaveJPG(surface, filename,100);
            break;
        case file_type_png:
            IMG_SavePNG(surface, filename);
            break;
        default:
            break;
    }
    return 0;
}

char* cat(char* a, char* b) {
    a = realloc(a,strlen(a) + strlen(b)+1);
    strcat(a, b);
    return a;
}

void freeBoxes(box_t *boxes, int box_count) {
    for (int i = 0; i < box_count; i++) {
        free(boxes[i].text);
    }
    free(boxes);
}
