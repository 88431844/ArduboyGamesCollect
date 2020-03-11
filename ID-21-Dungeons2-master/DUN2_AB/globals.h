#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include "Arglib.h"
#include "bitmaps.h"

//define menu states (on main menu)
#define STATE_MENU_INTRO             0
#define STATE_MENU_MAIN              1
#define STATE_MENU_HELP              2
#define STATE_MENU_PLAY              3
#define STATE_MENU_INFO              4
#define STATE_MENU_SOUNDFX           5

//define game states (on main menu)
#define STATE_GAME_NEXT_LEVEL        6
#define STATE_GAME_PLAYING           7
#define STATE_GAME_INVENTORY         8
#define STATE_GAME_OVER              9

#define GAME_TOP                     0
#define GAME_BOTTOM                  48
#define GAME_LEFT                    0
#define GAME_RIGHT                   112

#define FACING_NORTH                 0
#define FACING_EAST                  1
#define FACING_SOUTH                 2
#define FACING_WEST                  3

#define FIRST_LEVEL                  0

#define PLAYER_SPEED                 2


Arduboy arduboy;
Sprites sprites(arduboy);

byte gameState = STATE_MENU_INTRO;   // start the game with the TEAM a.r.g. logo
byte menuSelection = STATE_MENU_PLAY; // PLAY menu item is pre-selected
byte globalCounter = 0;
byte level = FIRST_LEVEL;
byte levelArray[32];

Point cam = {.x = 0, .y = 0};

#endif
