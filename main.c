//
//  main.c
//  checkers
//
//  Created by Halvard Mørstad on 25/10/2019.
//  Copyright © 2019 Halvard Mørstad. All rights reserved.
//

//0 =   = empty
//1 = o = p1
//2 = 0 = p1 king
//3 = x = p2
//4 = + = p2 king

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define EMPTY 0
#define P1 1
#define P1K 2
#define P2 3
#define P2K 4

typedef struct Player {
    int pawnValue;
    int kingValue;
    int direction; //1 for p1, -1 for p2
} Player;

typedef struct BrickMovement {
    int coX;
    int coY;
    int priority; //0: not valid, 1: step, 2: jump
} BrickMovement;

typedef struct PlayerBrick {
    int type;
    int x;
    int y;
    BrickMovement brickMovement[4];
} PlayerBrick;

typedef struct GameState {
    PlayerBrick playerBricks[24];
    PlayerBrick currentBrick;
    Player allPlayers[2];
    Player currentPlayer;
    int boardArray[8][8];
} GameState;

Player player1 = {.pawnValue=1, .kingValue=2, .direction=1};
Player player2 = {.pawnValue=3, .kingValue=4, .direction=-1};

GameState GAMES_STATE = {
        .playerBricks={
                {1, 0, 1, 0},
                {1, 0, 3, 0},
                {1, 0, 5, 0},
                {1, 0, 7, 0},
                {1, 1, 0, 0},
                {1, 1, 2, 0},
                {1, 1, 4, 0},
                {1, 1, 6, 0},
                {1, 2, 1, 0},
                {1, 2, 3, 0},
                {1, 2, 5, 0},
                {1, 2, 7, 0},
                {3, 5, 0, 0},
                {3, 5, 2, 0},
                {3, 5, 4, 0},
                {3, 5, 6, 0},
                {3, 6, 1, 0},
                {3, 6, 3, 0},
                {3, 6, 5, 0},
                {3, 6, 7, 0},
                {3, 7, 0, 0},
                {3, 7, 2, 0},
                {3, 7, 4, 0},
                {3, 7, 6, 0},
        },
        .boardArray={0},
        .allPlayers={
                {.pawnValue=1, .kingValue=2, .direction=1},
                {.pawnValue=3, .kingValue=4, .direction=-1}
        },
        .currentPlayer={.pawnValue=1, .kingValue=2, .direction=1},
        .currentBrick={0, 0, 0}
};

bool brickIsCurrentPlayer(int playerType) {
    if (playerType == GAMES_STATE.currentPlayer.pawnValue ||
        playerType == GAMES_STATE.currentPlayer.kingValue) {
        return true;
    }
    return false;
}

bool playerIsCurrentPlayer(Player player) {
    if (GAMES_STATE.currentPlayer.pawnValue == player.pawnValue &&
        GAMES_STATE.currentPlayer.kingValue == player.kingValue) {
        return true;
    }
    return false;
}

bool brickIsPawn(PlayerBrick brick) {
    if (brick.type == player1.pawnValue && brick.type == player2.pawnValue) {
        return true;
    }
    return false;
}

bool brickIsKing(PlayerBrick brick) {
    if (brick.type == player1.kingValue && brick.type == player2.kingValue) {
        return true;
    }
    return false;
}

void generateBoard(char board[1000]) {
    int cX = 0;
    int cY = 0;

    while (cY < 8) {
        strcat(board, "   ---------------------------------\n");
        char iY[5];
        sprintf(iY, " %d |", cY);
        strcat(board, iY);

        while (cX < 8) {

            switch (GAMES_STATE.boardArray[cX][cY]) {
                case 0:
                    strcat(board, "   |");
                    break;
                case 1:
                    strcat(board, " o |");
                    break;
                case 2:
                    strcat(board, " 0 |");
                    break;
                case 3:
                    strcat(board, " x |");
                    break;
                case 4:
                    strcat(board, " + |");
                    break;

                default:
                    break;
            }

            cX++;
        }
        cX = 0;
        strcat(board, "\n");
        cY++;
    }
    strcat(board, "   ---------------------------------\n");
    strcat(board, "     A   B   C   D   E   F   G   H  \n\n");
    strcat(board, "    P1(king): o(0)   P2(king): x(+) \n\n\0");
}

int inputIsValid(int inputX, int inputY) {

    if (inputX >= 0 && inputX < 8 && inputY < 8 && inputY >= 0) {
        for (int i = 0; i < 24; i++) {

            if (GAMES_STATE.playerBricks[i].x == inputX && GAMES_STATE.playerBricks[i].y == inputY &&
                brickIsCurrentPlayer(GAMES_STATE.playerBricks[i].type)) {
                GAMES_STATE.currentBrick = GAMES_STATE.playerBricks[i];
                return true;
            }
        }
    }

    return false;
}

void gBoardArray(int boardArray[8][8]) {

    int cX = 0;
    int cY = 0;

    char bricks[4] = {P1, P1K, P2, P2K};

    while (cY < 8) {
        while (cX < 8) {
            int player = 0;
            switch (cY) {
                case 0:
                case 2:
                case 6:
                    if (cY > 3) {
                        player += 2;
                    }

                    switch (cX) {
                        case 1:
                        case 3:
                        case 5:
                        case 7:
                            boardArray[cX][cY] = player + 1;
                            break;

                        default:
                            boardArray[cX][cY] = 0;
                            break;
                    }
                    break;

                case 1:
                case 5:
                case 7:
                    switch (cX) {
                        case 0:
                        case 2:
                        case 4:
                        case 6:
                            if (cY < 4) {
                                boardArray[cX][cY] = 1;
                            } else {
                                boardArray[cX][cY] = 3;
                            }

                            break;

                        default:
                            boardArray[cX][cY] = 0;
                            break;
                    }
                    break;

                default:
                    boardArray[cX][cY] = 0;
                    break;
            }

            cX++;
        }
        cX = 0;
        cY++;
    }
}

void getBoardArray() {

    for (int i = 0; i < 24; i++) {
        int coX = GAMES_STATE.playerBricks[i].x;
        int coY = GAMES_STATE.playerBricks[i].y;
        GAMES_STATE.boardArray[coX][coY] = GAMES_STATE.playerBricks[i].type;
    }
}

int getStatusOfMovementForBrick(int brickIndex, int coX, int coY, int brickMovement) {
    int direction = GAMES_STATE.currentPlayer.direction;
    PlayerBrick brick = GAMES_STATE.playerBricks[brickIndex];

    int closeX = brick.x + (coX * direction);
    int closeY = brick.x + (coY * direction);
    int farX = brick.x + (coX * 2 * direction);
    int farY = brick.x + (coY * 2 * direction);

    if (closeX >= 0 && closeX < 8 && closeY >= 0 && closeY < 8) {
        int closeCell = GAMES_STATE.boardArray[closeY][closeX];

//        Check if the brick can move in this direction
        if (brickIsPawn(brick) && (direction * coY) < 0) {
            return 0;

        } else if (brickIsCurrentPlayer(closeCell)) {
            return 0;

        } else if (closeCell == 0) {
            brick.brickMovement[brickMovement].coY = closeY;
            brick.brickMovement[brickMovement].coX = closeX;
            brick.brickMovement[brickMovement].priority = 1;
            return 1;

        } else {
//            if it reaches here, it is an enemy player, and we have to check if we can jump over it
            if (farX >= 0 && farX < 8 && farY >= 0 && farY < 8) {
                int farCell = GAMES_STATE.boardArray[farY][farX];

                if (farCell == 0) {
                    brick.brickMovement[brickMovement].coY = farX;
                    brick.brickMovement[brickMovement].coX = farY;
                    brick.brickMovement[brickMovement].priority = 2;
                    return 2;
                }
            }
        }
    }
    return 0;
}

void getValidMovementsForBrick(int brickIndex) {
    // 1 for p1, -1 for p2, 0 for kings

//  Check corners of brick (N and S is reversed for p2)
//  0 = blocked, 1 = step, 2 = jump
    getStatusOfMovementForBrick(brickIndex, 1, -1, 0);
    getStatusOfMovementForBrick(brickIndex, 1, 1, 1);
    getStatusOfMovementForBrick(brickIndex, -1, 1, 2);
    getStatusOfMovementForBrick(brickIndex, -1, -1, 3);


}

void getValidityOfBricks() {
    for (int i = 0; i < 24; i++) {

        if (brickIsCurrentPlayer(GAMES_STATE.playerBricks[i].type)) {
            getValidMovementsForBrick(i);
        }
    }
}

void adjustPlayerMovement() {
    char inputX[1];
    int inputY[1];
    bool correctInput = false;

    while (!correctInput) {
        getValidityOfBricks();
        printf(" Select brick to move (ex: A2):\n");

        scanf("%c%d", inputX, inputY);


        bool isValidPlayerBrick = inputIsValid(inputX[0] - 65, inputY[0]);

        if (isValidPlayerBrick) {

            printf(" Move brick to (ex: A2):\n");
        } else {

            printf(" Please choose a valid brick\n");
        }
    }
}

int main(int argc, const char *argv[]) {

    getBoardArray();

    char board[1000];

    generateBoard((char *) board);

    printf("%s", board);

    bool gameOn = true;

    while (gameOn) {

        adjustPlayerMovement();

    }


    return 0;
}
