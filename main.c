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

typedef struct Player {
    int id;
    int direction; //1 for p1, -1 for p2
    int validMoves;
} Player;

typedef struct BrickMovement {
    int cX;
    int cY;
    int priority; //0: not valid, 1: step, 2: jump
    char printable[2];
} BrickMovement;

typedef struct PlayerBrick {
    int id;
    int cX;
    int cY;
    int playerId;
    bool isKing;
    BrickMovement brickMovement[4];
} PlayerBrick;

typedef struct GameState {
    PlayerBrick playerBricks[24];
    PlayerBrick currentBrick;
    Player allPlayers[2];
    Player currentPlayer;
    int boardArray[8][8];
    bool gameIsOn;
} GameState;

Player player1 = {.id=0, .direction=1, .validMoves=0};
Player player2 = {.id=1, .direction=-1, .validMoves=0};

GameState GAMES_STATE = {
        .playerBricks={ //id starts at 1 to give the value 0 = empty
                {1,  1, 0, 0, false, 0},
                {2,  3, 0, 0, false, 0},
                {3,  5, 0, 0, false, 0},
                {4,  7, 0, 0, false, 0},
                {5,  0, 1, 0, false, 0},
                {6,  2, 1, 0, false, 0},
                {7,  4, 1, 0, false, 0},
                {8,  6, 1, 0, false, 0},
                {9,  1, 2, 0, false, 0},
                {10, 3, 2, 0, false, 0},
                {11, 5, 2, 0, false, 0},
                {12, 7, 2, 0, false, 0},
                {13, 0, 5, 1, false, 0},
                {14, 2, 5, 1, false, 0},
                {15, 4, 5, 1, false, 0},
                {16, 6, 5, 1, false, 0},
                {17, 1, 6, 1, false, 0},
                {18, 3, 6, 1, false, 0},
                {19, 5, 6, 1, false, 0},
                {20, 7, 6, 1, false, 0},
                {21, 0, 7, 1, false, 0},
                {22, 2, 7, 1, false, 0},
                {23, 4, 7, 1, false, 0},
                {24, 6, 7, 1, false, 0},
        },
        .boardArray={0},
        .allPlayers={
                {.id=0, .direction=1, .validMoves=0},
                {.id=1, .direction=-1, .validMoves=0}
        },
        .currentPlayer={.id=0, .direction=1, .validMoves=0},
        .currentBrick={0},
        .gameIsOn=true
};

int getDirectionOfBrick(PlayerBrick brick) {
    int id = brick.playerId;

    return GAMES_STATE.allPlayers[id].direction;
}

void generateBoard() {
    int cX = 0;
    int cY = 0;

    while (cY < 8) {
        printf("   ---------------------------------\n");
        printf(" %d |", cY);

        while (cX < 8) {
            int brickId = GAMES_STATE.boardArray[cX][cY];

            if (brickId == 0) {
                printf("   |");

            } else {
                PlayerBrick brick = GAMES_STATE.playerBricks[brickId - 1];

                if (brick.playerId == player1.id) {
                    if (brick.isKing) {
                        printf(" 0 |");
                        continue;
                    }
                    printf(" o |");

                } else {

                    if (brick.isKing) {
                        printf(" + |");
                        continue;
                    }
                    printf(" x |");
                }
            }
            cX++;
        }
        cX = 0;
        printf("\n");
        cY++;
    }
    printf("   ---------------------------------\n");
    printf("     A   B   C   D   E   F   G   H  \n\n");
    printf("    P1(king): o(0)   P2(king): x(+) \n\n");
}

void getBoardArray() {
    for (int i = 0; i < 24; i++) {
        int cX = GAMES_STATE.playerBricks[i].cX;
        int cY = GAMES_STATE.playerBricks[i].cY;
        GAMES_STATE.boardArray[cX][cY] = GAMES_STATE.playerBricks[i].id;
    }
}

int inputIsValid(int inputX, int inputY) {

    if (inputX >= 0 && inputX < 8 && inputY < 8 && inputY >= 0) {
        int brickId = GAMES_STATE.boardArray[inputX][inputY];

        if (brickId == 0) {
            return false;
        }

        PlayerBrick brick = GAMES_STATE.playerBricks[brickId - 1];

        if (brick.playerId == GAMES_STATE.currentPlayer.id) {

            for (int i = 0; i < 4; i++) {

                if (brick.brickMovement[i].priority != 0) {
                    GAMES_STATE.currentBrick = brick;
                    return true;
                }
            }
        }
    }

    return false;
}

void updateBrickMovement(int brickIndex, int brickMovement, int cX, int cY, int pri) {
    GAMES_STATE.playerBricks[brickIndex].brickMovement[brickMovement].cX = cX;
    GAMES_STATE.playerBricks[brickIndex].brickMovement[brickMovement].cY = cY;
    GAMES_STATE.playerBricks[brickIndex].brickMovement[brickMovement].priority = pri;
    GAMES_STATE.playerBricks[brickIndex].brickMovement[brickMovement].printable[0] = cX + 65;
    GAMES_STATE.playerBricks[brickIndex].brickMovement[brickMovement].printable[1] = cY + 48;
    GAMES_STATE.allPlayers[GAMES_STATE.playerBricks[brickIndex].playerId].validMoves++;
}

int getStatusOfMovementForBrick(int brickIndex, int cX, int cY, int brickMovement) {
    int direction = getDirectionOfBrick(GAMES_STATE.playerBricks[brickIndex]);
    PlayerBrick brick = GAMES_STATE.playerBricks[brickIndex];

    int closeX = brick.cX + cX;
    int closeY = brick.cY + cY;
    int farX = brick.cX + (cX * 2);
    int farY = brick.cY + (cY * 2);

    if (direction == 0) {
        return 0;
    } else if (closeX >= 0 && closeX < 8 && closeY >= 0 && closeY < 8) {
        int brickId = GAMES_STATE.boardArray[closeX][closeY];

        bool directionIsValid = (direction * cY) > 0;

//        Check if the brick can move in this direction
        if (!brick.isKing && !directionIsValid) {
            return 0;

        } else if (brickId != 0 && GAMES_STATE.playerBricks[brickId - 1].playerId == brick.playerId) {
            return 0;

        } else if (brickId == 0) {
            updateBrickMovement(brickIndex, brickMovement, closeX, closeY, 1);
            return 1;

        } else {
//            if it reaches here, it is an enemy player, and we have to check if we can jump over it
            if (farX >= 0 && farX < 8 && farY >= 0 && farY < 8) {
                int farCell = GAMES_STATE.boardArray[farX][farY];

                if (farCell == 0) {
                    updateBrickMovement(brickIndex, brickMovement, farX, farY, 2);
                    return 2;
                }
            }
        }
    }
    return 0;
}

void getValidityOfBricks() {
    GAMES_STATE.allPlayers[0].validMoves = 0;
    GAMES_STATE.allPlayers[1].validMoves = 0;

    for (int i = 0; i < 24; i++) {

        getStatusOfMovementForBrick(i, 1, -1, 0);
        getStatusOfMovementForBrick(i, 1, 1, 1);
        getStatusOfMovementForBrick(i, -1, 1, 2);
        getStatusOfMovementForBrick(i, -1, -1, 3);


    }
}

void printValidOutputCellsForBrick(BrickMovement *brickMovement) {
//    GAMES_STATE.currentBrick.brickMovement[i].cX+65,GAMES_STATE.currentBrick.brickMovement[i].cY+48
    printf("[");
    for (int i = 0; i < 4; i++) {
        printf("%s", brickMovement[i].printable);
        printf("|");
    }
    printf("]\n");
}

bool inputToIsValidForBrick(PlayerBrick brick, char inputTo[2]) {
    for (int i = 0; i < 4; i++) {
        if (brick.brickMovement[i].printable[0] == inputTo[0] && brick.brickMovement[i].printable[1] == inputTo[1]) {
            return true;
        }
    }
    return false;
}

void adjustPlayerMovement() {
    char inputSelect[2];
    bool correctInput = false;

    while (!correctInput) {
        printf(" Select brick to move (ex: A2):\n");

        scanf("%s", inputSelect);

        bool isValidPlayerBrick = inputIsValid(inputSelect[0] - 65, inputSelect[1] - 48);

        if (isValidPlayerBrick) {
            char inputTo[2];
            printf(" Move brick to one of cells: ");
            printValidOutputCellsForBrick(GAMES_STATE.currentBrick.brickMovement);

            scanf("%s", inputTo);

            bool inputToIsValid = inputToIsValidForBrick(GAMES_STATE.currentBrick, inputTo);
            if (!inputToIsValid) {
                printf(" Please choose a valid movement\n");
                continue;
            }

            GAMES_STATE.playerBricks[GAMES_STATE.currentBrick.id - 1].cX = inputTo[0] - 65;
            GAMES_STATE.playerBricks[GAMES_STATE.currentBrick.id - 1].cY = inputTo[1] - 48;

            correctInput = true;
        } else {
            printf(" Please choose a valid brick\n");
        }
    }
}

void checkIfGameOver() {
    int p1ValidMoves = GAMES_STATE.allPlayers[player1.id].validMoves;
    int p2ValidMoves = GAMES_STATE.allPlayers[player2.id].validMoves;

    if (p1ValidMoves < 1 || p2ValidMoves < 1) {
        char *winner;
        bool p1Wins = p1ValidMoves > p2ValidMoves;

        if (p1Wins) {
            winner = "Player 1";
        } else {
            winner = "Player 2";
        }

        GAMES_STATE.gameIsOn = false;
        printf("#### %s is the winner!!! ####", winner);
    }
}

void switchCurrentPlayer() {
    if (GAMES_STATE.currentPlayer.id == player1.id) {
        GAMES_STATE.currentPlayer = player2;
    } else {
        GAMES_STATE.currentPlayer = player1;
    }
}

int main(int argc, const char *argv[]) {

    while (GAMES_STATE.gameIsOn) {

        getBoardArray();
        generateBoard();

        getValidityOfBricks();

        adjustPlayerMovement();

        checkIfGameOver();

        switchCurrentPlayer();
    }

    return 0;
}
