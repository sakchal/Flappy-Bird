#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

void StartScreen(void);

typedef struct bird {
    int row;
    int col;
    unsigned short color;
} Bird;

void moveFlappyBird(Bird *flappyBird, int row, int col);

Bird flappyBird;

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

#endif
