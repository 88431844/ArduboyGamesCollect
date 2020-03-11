#ifndef ENEMIES_H
#define ENEMIES_H

#include <Arduino.h>
#include "globals.h"

#define TOTAL_SPAWNS    16
#define TYPE_ENEMY      0
#define TYPE_ENTITY     0x80
#define ENEMY_BLOB      0
#define ENTITY_BLOCK    0


struct Spawner;

byte entity_frame = 0;

struct Spawnable {
  int x;
  int y;
  byte type;
  Spawner *parent;

  bool insideView () {
    //Point p = { .x = x, .y = y };
    //Rect r = { .x = cam.x - 128, .y = cam.y - 128, .width = 384, .height = 320};
    //return (arduboy.collide(p, r));
    return (abs(x - player.x) < 128 && abs(y - player.y) < 128);
  }

  virtual void update ();
};

struct Enemy : public Spawnable {
  byte hp;
  byte attack;
  byte defense;
  
  Enemy (int xpos, int ypos, byte enemy_type, byte level, Spawner *p) {
    x = xpos;
    y = ypos;
    type = enemy_type;
    parent = p;
  }

  void update () final {
    switch (type) {
      case ENEMY_BLOB:
      {
        // Blob logic
        sprites.drawOverwrite(x - cam.x, y - cam.y, enemyBlob, entity_frame);
      }
    }
  }
};

struct Entity : public Spawnable {
  byte can_move;
  byte direction;
  
  Entity (int xpos, int ypos, byte entity_type, Spawner *p) {
    x = xpos;
    y = ypos;
    type = entity_type;
    parent = p;
  }

  void update () final {
    switch (type) {
      case ENTITY_BLOCK:
      {
        sprites.drawOverwrite(x - cam.x, y - cam.y, entityBlock, 0);
      }
    }
  }
};

Spawnable *spawns[TOTAL_SPAWNS];

struct Spawner {
  Rect spawn_area;
  byte type;
  byte spawns_left;
  bool active;

  bool withinSpawn () const {
    Point p = { .x = player.x, .y = player.y};
    return (arduboy.collide(p, spawn_area));
  }

  void spawnEntities () {
    if (withinSpawn()) {
      for (byte i = 0; i < TOTAL_SPAWNS && spawns_left > 0; ++i) {
        if (!spawns[i]) {
          --spawns_left;
          if (type <= 0x7F) {
            Serial.println("Creating an Enemy");
            spawns[i] = new Enemy(spawn_area.x + 72 + (i % 3) * 16, spawn_area.y + 72 + (i / 3) * 16, type & 0x7F, 1, this);
          }
          else {
            Serial.println("Creating an Entity");
            spawns[i] = new Entity(spawn_area.x + 64 + (i % 4) * 16, spawn_area.y + 64 + (i / 4) * 16, type & 0x7F, this);
          }
        }
      }
    }
  }

  static void updateEntities () {
    if (arduboy.everyXFrames(5)) entity_frame = (++entity_frame) % 4;

    for (byte i = 0; i < TOTAL_SPAWNS; ++i) {
      if (spawns[i]) {
        if (!spawns[i]->insideView()) {
          ++spawns[i]->parent->spawns_left;
          delete spawns[i];
          spawns[i] = NULL;
          Serial.println("Destroyed an Object or Entity");
        }
        else
        {
          spawns[i]->update();
        }
      }
    }
  }
};

Spawner spawners[TOTAL_ROOMS - 2];

void deactivateSpawners () {
  for (byte i = 0; i < TOTAL_ROOMS - 2; ++i) {
    spawners[i].active = false;
  }
  for (byte b = 0; b < TOTAL_SPAWNS; ++b) {
    if (spawns[b]) delete spawns[b];
    spawns[b] = NULL;
  }
}

void createSpawner (int x, int y, byte type, byte number_of_spawns) {
  for (byte i = 0; i < TOTAL_ROOMS - 2; ++i) {
    if (!spawners[i].active) {
      spawners[i].active = true;
      spawners[i].spawn_area.x = x;
      spawners[i].spawn_area.y = y;
      spawners[i].spawn_area.width = 192;
      spawners[i].spawn_area.height = 192;
      spawners[i].type = type;
      spawners[i].spawns_left = number_of_spawns;
      break;
    }
  }
}

void updateSpawners () {
  spawners[0].updateEntities();
  for (byte i = 0; i < TOTAL_ROOMS - 2; ++i) {
    spawners[i].spawnEntities();
  }
}




#endif
