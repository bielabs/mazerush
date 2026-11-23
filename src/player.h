#ifndef PLAYER_H
#define PLAYER_H

#include "config.h"

typedef struct
{
    Vector2 pos;
    float radius;
    float speed;
    float invincibilityTimer;
} Player;

Player PlayerInit(Vector2 pos);

/* Camada impura: le o teclado. */
Vector2 PlayerReadInput(void);

/* Camada pura (sobre os argumentos): devolvem um novo Player. */
Player PlayerMove(Player p, Vector2 input, float dt);
Player PlayerTick(Player p, float dt);
Player PlayerHit(Player p);

void PlayerDraw(const Player *p);

#endif
