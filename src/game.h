#ifndef GAME_H
#define GAME_H

#include "collectible.h"
#include "config.h"
#include "enemy.h"
#include "player.h"

typedef struct
{
    GameState state;
    GameMode mode;
    int screenWidth;
    int screenHeight;

    Player player;
    Enemy enemies[MAX_ENEMIES];
    Collectible collectibles[MAX_COLLECTIBLES];
    int collectibleCount;

    int score;
    int lives;
    int wave;
    int waveItems;
    int itemsTarget;
    float timeLeft;

    Camera2D camera;
} Game;

Game GameInit(int screenWidth, int screenHeight);

/* Atualiza o estado. Nunca desenha. */
void GameUpdate(Game *game, float dt);

/* Desenha o estado. Nunca o altera. */
void GameDraw(const Game *game);

#endif
