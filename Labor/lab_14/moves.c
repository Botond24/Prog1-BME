#include "moves.h"

coords getPlayerPos(char board[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 11; j++) {
            if (board[i][j] == PLAYER) {
                return (coords) {j, i};
            }
        }
    }
}


bool canGo(char board[10][10], direction dir) {
    coords player = getPlayerPos(board);
    switch (dir) {
        case UP:
            return board[player.y - 1][player.x] != WALL;
        case DOWN:
            return board[player.y + 1][player.x] != WALL;
        case LEFT:
            return board[player.y][player.x - 1] != WALL;
        case RIGHT:
            return board[player.y][player.x + 1] != WALL;
        default:
            return false;
    }
}
after_move move_up(char board[10][10]) {
    coords player = getPlayerPos(board);
    after_move move = -1;
    switch (board[player.y-1][player.x])
    {
        case ASS:
            move = TREASURE;
            break;
        case EMPTY:
            move = (player.y - 1 == 0) ? EXIT : EMPTY_;
            break;
        default:
            break;
    }
    board[player.y][player.x] = EMPTY;
    board[player.y - 1][player.x] = PLAYER;
    return move;
}
after_move move_down(char board[10][10]) {
    coords player = getPlayerPos(board);
    after_move move = -1;
    switch (board[player.y + 1][player.x])
    {
        case ASS:
            move = TREASURE;
            break;
        case EMPTY:
            move = (player.y + 1 == 9) ? EXIT : EMPTY_;
            break;
        default:
            break;
    }
    board[player.y][player.x] = EMPTY;
    board[player.y + 1][player.x] = PLAYER;
    return move;
}
after_move move_left(char board[10][10]) {
    coords player = getPlayerPos(board);
    after_move move = -1;
    switch (board[player.y][player.x-1])
    {
        case ASS:
            move = TREASURE;
            break;
        case EMPTY:
            move = (player.x - 1 == 0) ? EXIT : EMPTY_;
            break;
        default:
            break;
    }
    board[player.y][player.x] = EMPTY;
    board[player.y][player.x-1] = PLAYER;
    return move;
}
after_move move_right(char board[10][10]) {
    coords player = getPlayerPos(board);
    after_move move = -1;
    switch (board[player.y][player.x+1])
    {
        case ASS:
            move = TREASURE;
            break;
        case EMPTY:
            move = (player.x + 1 == 9) ? EXIT : EMPTY_;
            break;
        default:
            break;
    }
    board[player.y][player.x] = EMPTY;
    board[player.y][player.x+1] = PLAYER;
    return move;
}


after_move move(char board[10][10], direction dir) {
    coords player = getPlayerPos(board);
    bool can = canGo(board, dir);
    switch (dir) {
        case UP:
            return can ? move_up(board) : WALL_;
        case DOWN:
            return can ? move_down(board): WALL_;
        case LEFT:
            return can ? move_left(board) : WALL_;
        case RIGHT:
            return can ? move_right(board) : WALL_;
        default:
            return -1;
    }
}
