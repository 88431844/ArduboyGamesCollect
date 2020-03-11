#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>
#include "globals.h"

struct Players {
  public:
    int x;
    int y;
    boolean walking;
    boolean slashing;
    byte direction;
    byte walkFrame;
    byte slashFrame;
    unsigned long score;
};

Players player = { .x = 20, .y = 20, .walking = false, .slashing = false, .direction = FACING_SOUTH, .walkFrame = 0 , .slashFrame = 0};

PROGMEM const unsigned char walkSeq[] = { 0, 1, 2, 1 };
PROGMEM const unsigned char slaySeq[] = { 0, 3, 3, 4, 4, 4, 4 };
PROGMEM const unsigned char tipSeq[] = { 0, 1, 1, 2, 2, 2, 2 };


void checkPlayer() {
  if (arduboy.everyXFrames(8)) {
    if (player.walking) player.walkFrame = (++player.walkFrame) % 4;
  }
  if (arduboy.everyXFrames(4)) {
    if (player.slashing) player.slashFrame++;
    if (player.slashFrame > 6) {
      player.slashFrame = 0;
      player.slashing = false;
    }
  }
}
void drawPlayer() {
  if (player.slashing) {
    sprites.drawPlusMask(player.x - cam.x - 1, player.y - cam.y - 1, playerSprite_plus_mask, pgm_read_byte(&slaySeq[player.slashFrame]) + 5 * player.direction);
    sprites.drawPlusMask(player.x + 16 - cam.x - 1, player.y + 7 - cam.y - 1,swordTip_plus_mask, pgm_read_byte(&tipSeq[player.slashFrame]));
  }
  else {
    sprites.drawPlusMask(player.x - cam.x - 1, player.y - cam.y - 1, playerSprite_plus_mask, pgm_read_byte(&walkSeq[player.walkFrame]) + 5 * player.direction);
  }
}
#endif
