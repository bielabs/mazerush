#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef struct
{
    Rectangle bounds;
    float speed;
} Player;

Player PlayerInit(Rectangle bounds, float speed);

#endif
