#ifndef LOGIC_H
#define LOGIC_H

/*
 * Regras do jogo em FUNCOES PURAS: so dependem dos argumentos, nao alteram
 * nada fora do que retornam e nao chamam desenho/teclado/som.
 * Por isso podem ser testadas sem abrir a janela (veja tests/test_logic.c).
 */

#include "config.h"

int     TileOf(float coord);
Vector2 NextPosition(Vector2 pos, Vector2 dir, float speed, float dt);

int   EnemyCountFor(int wave, GameMode mode);
float EnemySpeedFor(int wave);
int   ItemsTargetFor(int wave);

bool  WaveCompleted(GameMode mode, int collectedInWave, int target);
float TimeAfter(float timeLeft, float dt);
bool  IsRoundOver(GameMode mode, float timeLeft, int lives);
int   ScoreAfter(int score, int collected);

#endif
