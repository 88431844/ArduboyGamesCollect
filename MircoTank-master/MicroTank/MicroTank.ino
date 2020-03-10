/*
  Copyright 2019 Thomas Hartmann (@hartmann1301)

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This is MicroTank, a game created for Arduboy.

  For lots of information and how to play see: https://github.com/hartmann1301/MicroTank

  Version: 1.0: 31.12.2018
*/

#include "Globals.h"
#include "Draw.h"
#include "Sprites.h"

#include "Items.h"
itemsManager iM;

#include "Animations.h"
animationManager aM;

#include "Player.h"
playerTank pT;

#include "Projectiles.h"
projectileManager pM;

#include "Enemys.h"
enemyManager eM;

#include "Map.h"
mapManager mM;

#include "Spawn.h"
spawnManager sM;


#include "Buttons.h"
#include "Checks.h"

void setup() {

#ifdef USE_SERIAL  
  Serial.begin(57600);
  Serial.println(F("MicroTank!"));
#endif

#ifdef ESP8266
  ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, false, false);

  // this function must be called or the esp will crash with the first call
  arduboy.setExternalButtonsHandler(getButtons);
#endif

  arduboy.begin();
  arduboy.setFrameRate(GAME_FRAMES);

  // some start tone
  sound.tone(200, 200, 300, 200);

  arduboy.fillScreen(WHITE);

  sM.init();
  pT.init();

  //eM.add(100, 30, ENEMY_MINE, 0);
  //eM.add(100, 30, ENEMY_ROCKET, 0);

  //arduboy.display();
  //delay(60000);
}


void playMode() {

  checkItemsCatch();
  checkItemsSpawn();
  checkEnemyChrush();
  checkProjectiles();

  mM.update();
  sM.update();
  pM.update();
  iM.update();
  eM.update();
  aM.update();
  pT.update();

  checkSlowmo();
  checkSpecialAttack();
}

void menuMode() {

  mM.drawMap();

  pT.drawTankShop();
}

void playGame() {
#ifdef DEBUG_FRAME_TIME
  uint32_t start = millis();
#endif

  prepareThings();
  updateGameButtons();

  if (isPlayMode) {
    playMode();
  } else {
    menuMode();
  }

  // needs to be after the drawMap
  pT.drawCornerInfo();

#ifdef DEBUG_FRAME_TIME
  // pirnt time for a frame to screen
  arduboy.fillRect(0, 0, 26, 7, BLACK);
  //drawNumbers(0, 0, millis() - start);
  //drawNumbers(0, 0, eM.count());
  //drawNumbers(0, 0, sM.difficulty);
#endif
}

void loop() {
  if (!(arduboy.nextFrame()))
    return;

  arduboy.pollButtons();

  arduboy.fillScreen(WHITE);

  if (isStartAnimation) {
    playAnimation();

  } else {
    playGame();
  }

  arduboy.display();

  gameFrames++;
}
