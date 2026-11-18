#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include "raylib.h"

typedef struct
{
    Rectangle bounds;
    bool active;
} Collectible;

Collectible CollectibleInit(Rectangle bounds, bool active);

#endif
