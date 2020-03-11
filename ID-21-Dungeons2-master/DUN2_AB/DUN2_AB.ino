/*
  Dungeons 2: http://www.team-arg.org/dun2-manual.html

  Arduboy version 0.1:  http://www.team-arg.org/dun2-downloads.html

  MADE by TEAM a.r.g. : http://www.team-arg.org/more-about.html

  2016 - GAVENO - JO3RI - Castpixel

  License: MIT : https://opensource.org/licenses/MIT

*/

//determine the game
#define GAME_ID 47

#include "Arglib.h"
#include "globals.h"
#include "menu.h"
#include "game.h"
#include "inputs.h"
#include "player.h"
#include "elements.h"
#include "enemies.h"
#include "levels.h"
#include "levelgenerator.h"

typedef void (*FunctionPointer) ();

const FunctionPointer PROGMEM mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGameNextLevel,
  stateGamePlaying,
  stateGameInventory,
  stateGameOver,
};


void setup() {
  Serial.begin(9600);
  arduboy.start();
  arduboy.setFrameRate(50);                                 // set the frame rate of the game at 60 fps
  arduboy.initRandomSeed();                                 // This sets the random to more random, remove this if no random is needed !
  levelGenerate(levelArray, 1);
}


void loop() {
  //Serial.println("Level Data");
  //for (int i = 0; i < 32; i++)  Serial.println(levelArray[i]);
  if (!(arduboy.nextFrame())) return;
  arduboy.poll();
  arduboy.clearDisplay();
  ((FunctionPointer) pgm_read_word (&mainGameLoop[gameState]))();
  arduboy.display();
}

