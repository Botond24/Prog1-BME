#ifndef MOVES_H
#define MOVES_H
#include <stdlib.h>
#include <stdbool.h>

#define WALL '#'
#define PLAYER '@'
#define EMPTY ' '
#define ASS '$'
//treasure = booty = ass

typedef struct
{
    int x;
    int y;
} coords;

typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction;

typedef enum
{
    WALL_,
    EXIT,
    TREASURE,
    EMPTY_
} after_move;

coords getPlayerPos(char board[10][10]);
bool canGo(char board[10][10], direction dir);
after_move move_up(char board[10][10]);
after_move move_down(char board[10][10]);
after_move move_left(char board[10][10]);
after_move move_right(char board[10][10]);

after_move move(char board[10][10], direction dir);

#endif