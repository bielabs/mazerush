#ifndef MAP_H
#define MAP_H

#include "config.h"

bool    MapIsWall(int row, int col);
bool    CheckWallCollision(Vector2 pos, float radius);
Vector2 TileCenter(int col, int row);
Vector2 GetRandomValidPosition(void);
void    DrawMap(void);

#endif
