#ifndef CONFIG_H
#define CONFIG_H

#include "raylib.h"

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#define TARGET_FPS    60

#define ROWS 21
#define COLS 31
#define TILE_SIZE 40

#define MAX_ENEMIES      6
#define MAX_COLLECTIBLES 5
#define HISTORY_SIZE     360   /* 360 quadros a 60 FPS = ~6 s de historico */

#define ROUND_TIME         45.0f
#define START_LIVES        3
#define INVINCIBILITY_TIME 4.0f

typedef enum
{
    MENU,
    PLAYING,
    GAME_OVER
} GameState;

typedef enum
{
    MODE_WAVES = 0,
    MODE_TIME  = 1
} GameMode;

#endif
