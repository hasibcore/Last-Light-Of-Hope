#ifndef COMMON_H
#define COMMON_H

#define _CRT_SECURE_NO_WARNINGS
#include "iGraphics.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>

// Screen constants
const int screen_width = 1200;
const int screen_height = 700;

// Maze constants
const int TILE_SIZE = 50;
const int ROWS = 14;
const int COLS = 24;
const int INITIAL_TIME = 150;
const int LIGHT_RADIUS = 90;

// Player constants
const int charWidth = 55;
const int charHeight = 35;

// Sound states enum
enum MusicState {
  MUSIC_NONE,
  MUSIC_MAZE_BG,
  MUSIC_WALK,
  MUSIC_FIGHT,
  MUSIC_HEARTBEAT,
  MUSIC_GAMEOVER,
  SFX_MONSTER_ATTACK,
  SFX_MONSTER_RUN,
  SFX_SWORD
};

#endif // COMMON_H
