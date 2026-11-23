#ifndef ENEMY_H
#define ENEMY_H

#include "config.h"

typedef struct
{
    Vector2 pos;
    Vector2 dir;
    float radius;
    float speed;
    bool active;
    Vector2 history[HISTORY_SIZE];
    int historyIndex;
} Enemy;

Enemy EnemyInit(Vector2 pos, float speed, bool active);

/* Devolvem um novo Enemy (nao alteram o original). */
Enemy EnemyStep(Enemy e, Vector2 playerPos, float dt);
Enemy EnemyRewind(Enemy e);

/* map: aplica a mesma transformacao a cada inimigo. */
void EnemiesUpdate(Enemy *es, int n, Vector2 playerPos, float dt);
void EnemiesRewind(Enemy *es, int n);

/* fold: reduzem a colecao a um unico valor. */
bool EnemiesHitPlayer(const Enemy *es, int n, Vector2 playerPos, float playerRadius);
int  EnemiesCountActive(const Enemy *es, int n);

void EnemiesDraw(const Enemy *es, int n);

#endif
