#ifndef GAME_H
#define GAME_H

#include "collectible.h"
#include "player.h"

#define MAX_COLLECTIBLES 5

typedef enum
{
    MENU,
    PLAYING,
    GAME_OVER
} GameState;

typedef struct
{
    GameState state;
    Player player;
    Collectible collectibles[MAX_COLLECTIBLES];
    int score;
    float elapsed;
} Game;

Game GameInit(void);
void GameUpdate(Game *game, float dt);
void GameDraw(const Game *game, float dt);

#endif
