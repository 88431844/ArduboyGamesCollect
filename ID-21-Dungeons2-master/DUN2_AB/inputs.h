#ifndef INPUTS_H
#define INPUTS_H

#include <Arduino.h>
#include "globals.h"
#include "player.h"
#include "levelgenerator.h"

void checkInputs()
{
  player.walking = false;
  if (arduboy.pressed(DOWN_BUTTON))
  {
    player.direction = FACING_SOUTH;
    if (!getSolid(levelArray, player.x + 3, player.y + 16) && !getSolid(levelArray, player.x + 12, player.y + 16)) player.y += PLAYER_SPEED;
    player.walking = true;
  }
  else if (arduboy.pressed(LEFT_BUTTON))
  {
    player.direction = FACING_WEST;
    if (!getSolid(levelArray, player.x - 1, player.y + 3) && !getSolid(levelArray, player.x - 1, player.y + 12)) player.x -= PLAYER_SPEED;
    player.walking = true;
  }
  else if (arduboy.pressed(UP_BUTTON))
  {
    player.direction = FACING_NORTH;
    if (!getSolid(levelArray, player.x + 3, player.y - 1) && !getSolid(levelArray, player.x + 12, player.y - 1)) player.y -= PLAYER_SPEED;
    player.walking = true;
  }
  else if (arduboy.pressed(RIGHT_BUTTON))
  {
    player.direction = FACING_EAST;
    if (!getSolid(levelArray, player.x + 16, player.y + 3) && !getSolid(levelArray, player.x + 16, player.y + 12)) player.x += PLAYER_SPEED;
    player.walking = true;
  }
  
  else if (arduboy.justPressed(B_BUTTON)) {
    player.slashing = true;
    player.walking = false;
  }

  // update camera
  cam.x = max(player.x - 56, 0);
  cam.y = max(player.y - 24, 0);

  if (arduboy.pressed(A_BUTTON) && arduboy.pressed(B_BUTTON)) gameState = STATE_GAME_INVENTORY;
  else if (arduboy.justPressed(A_BUTTON))
  {
    // Testing
    level++;
    Serial.println(" ");
    Serial.println(" ");
    Serial.println(" ");
    Serial.print("---Level ");
    Serial.print(level);
    Serial.println("---");
    levelGenerate(levelArray, level);
    for (int y = 0; y <= 16; ++y)
    {
      for (int x = 0; x <= 16; ++x)
      {
        Serial.print((getChunkBit(levelArray, x, y)) ? " " : "#");
      }
      Serial.println(" ");
    }
  }
}


#endif
