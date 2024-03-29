#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/flappybird.h"
#include "images/tunnel.h"
#include "images/start_img.h"
#include "images/lose_screen.h"
#include "images/win_img.h"
#include "images/background.h"
#include "images/enemy.h"


/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"

/* TODO: */
// Add any additional states you need for your app. You are not requried to use
// these specific provided states.
Bird flappyBird;

Bird enemyBird;

// Bird enemyBird;

enum gba_state {
  START,
  START_MESSAGE,
  PLAY_INIT,
  PLAY,
  WIN,
  WIN_MESSAGE,
  LOSE,
  LOSE_MESSAGE,
};

int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;
  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  enum gba_state state = START;

  char start[] = "Press 'ENTER' to Start";
  char arr[35];
  
  int h = 1;
  int v = 1;

  int timer;

  int tunnelRow = 140;
  int tunnelCol = 100;

  int enemyRow = 80;
  int enemyCol = 70;

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw

    switch (state) {
      case START:
        waitForVBlank();
        drawFullScreenImageDMA(start_img);  
        timer = 600;
        state = START_MESSAGE;
        break;

      case START_MESSAGE:
        drawString(130, 55, start, WHITE);
        if (KEY_DOWN(BUTTON_START, currentButtons)) {
          state = PLAY_INIT;
        }
        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = START;
        }
        break;

      case PLAY_INIT:
        waitForVBlank();
        drawFullScreenImageDMA(background);
        Bird *flappyBirdPtr; // flappy bird pointer
        flappyBirdPtr = &flappyBird;
        (*flappyBirdPtr).row = 10;
        (*flappyBirdPtr).col = 30;
        drawImageDMA((*flappyBirdPtr).row, (*flappyBirdPtr).col, 20, 20, flappybird);
        drawImageDMA(tunnelRow, tunnelCol, 50, 37, tunnel);
        drawImageDMA(enemyRow, enemyCol, 30, 30, enemy);
        state = PLAY;
        timer--;
        break;

      case PLAY:
        waitForVBlank();
        drawRectDMA(6, 205, 10, 11, WHITE);
        waitForVBlank();
        drawImageDMA((*flappyBirdPtr).row, (*flappyBirdPtr).col, 20, 20, flappybird);
        sprintf(arr, "Time: %d", timer/60);
        drawString(8, 170, arr, BLACK);
        timer--;

        if (KEY_DOWN(BUTTON_UP, currentButtons)) {
          moveFlappyBird(flappyBirdPtr, -v, 0);
        }
        if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
          moveFlappyBird(flappyBirdPtr, v, 0);
        }
        if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
          moveFlappyBird(flappyBirdPtr, 0, h);
        }
        if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
          moveFlappyBird(flappyBirdPtr, 0, -h);
        }

        int topLeftX = (*flappyBirdPtr).col;
        int topLeftY = (*flappyBirdPtr).row;

        int reachedTunnel = 0;

        // tunnel image boundaries
        if (topLeftX + 30 >= tunnelCol && topLeftX + 20 <= tunnelCol + 20 && topLeftY >= tunnelRow && topLeftY <= tunnelRow + 20) {
          reachedTunnel = 1;
        } else if (topLeftX + 20 >= tunnelCol && topLeftX + 20 <= tunnelCol + 20 && topLeftY + 20 >= tunnelRow && topLeftY + 20 <= tunnelRow + 20) {
          reachedTunnel = 1;
        } else if (topLeftX >= tunnelCol && topLeftX <= tunnelCol + 20 && topLeftY >= tunnelRow && topLeftY <= tunnelRow + 20) {
          reachedTunnel = 1;
        } else if (topLeftX >= tunnelCol && topLeftX <= tunnelCol + 20 && topLeftY + 20 >= tunnelRow && topLeftY + 20 <= tunnelRow + 20) {
          reachedTunnel = 1;
        }


        if (timer == 0) {
          state = LOSE;
        }

        int reachEnemy = 0;

        // enemy bird image boundaries
        if (topLeftX + 20 >= enemyCol && topLeftX + 20 <= enemyCol + 20 && topLeftY >= enemyRow && topLeftY <= enemyRow + 20) {
          reachEnemy = 1;
        } else if (topLeftX + 20 >= enemyCol && topLeftX + 20 <= enemyCol + 20 && topLeftY + 20 >= enemyRow && topLeftY + 20 <= enemyRow + 20) {
          reachEnemy = 1;
        } else if (topLeftX >= enemyCol && topLeftX <= enemyCol + 20 && topLeftY >= enemyRow && topLeftY <= enemyRow + 20) {
          reachEnemy = 1;
        } else if (topLeftX >= enemyCol && topLeftX <= enemyCol + 20 && topLeftY + 20 >= enemyRow && topLeftY + 20 <= enemyRow + 20) {
          reachEnemy = 1;
        }
        if (timer == 0) {
          state = LOSE;
        }

        if (reachEnemy == 1){
          state = LOSE;
        }

        if (reachedTunnel) {
          fillScreenDMA(BLACK);
          drawImageDMA((*flappyBirdPtr).row, (*flappyBirdPtr).col, 20, 20, flappybird);
          state = WIN;
        }

        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = START;
        }

        timer--;
        break;

      case WIN:
        timer--;
        waitForVBlank();
        drawFullScreenImageDMA(win_img); 
        state = WIN_MESSAGE;
        break;

      case WIN_MESSAGE:
        waitForVBlank();
        drawString(65, 98, "WINNER!", WHITE);
        drawString(135, 45, "Press 'BACKSPACE' to Restart", WHITE);
        sprintf(arr, "Time: %d", timer/60);
        drawString(8, 170, arr, BLACK);

        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = START;
        }
        break;

      case LOSE:
        drawFullScreenImageDMA(lose_screen); 
        state = LOSE_MESSAGE;
        break;

      case LOSE_MESSAGE:
        drawString(150, 30, "Press 'BACKSPACE' to Restart", WHITE);
        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = START;
        }
        break;
    }
    waitForVBlank();
    previousButtons = currentButtons; 
  }

  UNUSED(previousButtons); 
  return 0;
}

void moveFlappyBird(Bird *flappyBird, int row, int col) {
  drawRectDMA((*flappyBird).row, (*flappyBird).col, 20, 20, WHITE);
  if ((((*flappyBird).row + row) >= 0) && ((*flappyBird).row + row) <= (HEIGHT - 20)) {
    (*flappyBird).row += row;
  }
  if ((((*flappyBird).col + col) >= 0) && ((*flappyBird).col + col) <= (WIDTH - 20)) {
    (*flappyBird).col += col;
  }
  drawImageDMA((*flappyBird).row, (*flappyBird).col, 20, 20, flappybird);
}