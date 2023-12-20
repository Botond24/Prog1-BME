
#define KEY_ESC 27
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#include <stdio.h>
#include <conio.h>
#include "moves.h"


void printBoard(char board[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

int treasureNum(char board[10][10]) {
    int count = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 11; j++) {
            if (board[i][j] == ASS) {
                count++;
            }
        }
    }
    return count;
}

/*
###### ###
#$   #   #
#### ### #
#        #
# #### ###
# #    #$#
# ##$# # #
#@ ### # #
#   $#   #
##########
*/
int main() {
    char board[10][10] = {
        "###### ###",
        "#$   #   #",
        "#### ### #",
        "#        #",
        "# #### ###",
        "# #    #$#",
        "# ##$# # #",
        "#@ ### # #",
        "#   $#   #",
        "##########"
    };
    int ch;
    printf("Press ESC to quit\n\n");
    printBoard(board);
    after_move moved = -1;
    while ((ch = getch()) != KEY_ESC) {
        switch (ch) {
            case 'w':
            case KEY_UP:
                moved = move(board, UP);
                break;
            case 's':
            case KEY_DOWN:
                moved = move(board, DOWN);
                break;
            case 'a':
            case KEY_LEFT:
                moved = move(board, LEFT);
                break;
            case 'd':
            case KEY_RIGHT:
                moved = move(board, RIGHT);
                break;
            default:
                break;
        }
        system("cls");
        printBoard(board);
        printf("\n");
        if (moved == EXIT)
        {
            if (treasureNum(board) == 0)
            {
                printf("You win!\n");
                break;
            }
            else
            {
                printf("You lose!\n");
                printf("Treasures left: %d\n", treasureNum(board));
                break;
            }
        } else if (moved == WALL_)
        {
            printf("You hit a wall!\n");
        }
    }
    return 0;
}