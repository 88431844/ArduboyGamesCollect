#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "globals.h"
#include "inputs.h"
#include "player.h"
#include "enemies.h"
#include "elements.h"
#include "levels.h"
#include "levelgenerator.h"

void stateMenuPlay()
//void stateGamePrepareLevel()
{
  player.score = 0; 
  gameState = STATE_GAME_NEXT_LEVEL;
  //Serial.print("play");
};


void stateGameNextLevel()
{
  level++;
  gameState = STATE_GAME_PLAYING;
};


void stateGamePlaying()
{
  drawTiles();
  checkInputs();
  updateSpawners();
  checkPlayer();
  drawPlayer();
  checkCollisions();

};

void stateGameInventory()
{
  sprites.drawSelfMasked(3, 45, inventoryTiles, 0);
  sprites.drawSelfMasked(4, 5, inventoryTiles, 1);
  if (arduboy.justPressed(A_BUTTON)) gameState = STATE_GAME_PLAYING;
};

void stateGameOver()
{
  
};



#endif
