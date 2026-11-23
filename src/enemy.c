#include "enemy.h"
#include "logic.h"
#include "map.h"
#include <math.h>

Enemy EnemyInit(Vector2 pos, float speed, bool active)
{
    Enemy e = { 0 };
    e.pos = pos;
    e.dir = (Vector2){ 0.0f, 0.0f };
    e.radius = 13.0f;
    e.speed = speed;
    e.active = active;
    for (int h = 0; h < HISTORY_SIZE; h++) e.history[h] = pos;
    e.historyIndex = 0;
    return e;
}

/* Escolhe, entre os corredores vizinhos, o que deixa o inimigo mais perto do jogador. */
static Vector2 ChooseDirection(const Enemy *e, int col, int row, Vector2 playerPos, bool forceDirection)
{
    static const Vector2 directions[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
    Vector2 best = e->dir;
    float minDist = 99999.0f;

    for (int i = 0; i < 4; i++)
    {
        int nextCol = col + (int)directions[i].x;
        int nextRow = row + (int)directions[i].y;
        if (MapIsWall(nextRow, nextCol)) continue;

        /* nao volta para tras, a menos que esteja preso */
        bool reverse = directions[i].x == -e->dir.x && directions[i].y == -e->dir.y;
        if (!forceDirection && reverse && (e->dir.x != 0 || e->dir.y != 0)) continue;

        Vector2 center = TileCenter(nextCol, nextRow);
        float dx = playerPos.x - center.x;
        float dy = playerPos.y - center.y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist < minDist)
        {
            minDist = dist;
            best = directions[i];
        }
    }

    /* sem saida (beco): aceita qualquer corredor, inclusive voltar */
    if (best.x == 0 && best.y == 0)
    {
        for (int i = 0; i < 4; i++)
        {
            if (!MapIsWall(row + (int)directions[i].y, col + (int)directions[i].x))
            {
                best = directions[i];
                break;
            }
        }
    }
    return best;
}

Enemy EnemyStep(Enemy e, Vector2 playerPos, float dt)
{
    if (!e.active) return e;

    int col = TileOf(e.pos.x);
    int row = TileOf(e.pos.y);
    Vector2 center = TileCenter(col, row);

    const float threshold = 3.0f;
    bool forceDirection = (e.dir.x == 0 && e.dir.y == 0);

    /* decide a direcao so quando chega ao centro de uma celula (interseccao) */
    if (forceDirection || (fabsf(e.pos.x - center.x) < threshold && fabsf(e.pos.y - center.y) < threshold))
    {
        e.pos = center;
        e.dir = ChooseDirection(&e, col, row, playerPos, forceDirection);
    }

    Vector2 next = NextPosition(e.pos, (Vector2){ e.dir.x, 0.0f }, e.speed, dt);
    if (CheckWallCollision(next, e.radius)) e.dir = (Vector2){ 0, 0 };
    else e.pos = next;

    next = NextPosition(e.pos, (Vector2){ 0.0f, e.dir.y }, e.speed, dt);
    if (CheckWallCollision(next, e.radius)) e.dir = (Vector2){ 0, 0 };
    else e.pos = next;

    e.history[e.historyIndex] = e.pos;
    e.historyIndex = (e.historyIndex + 1) % HISTORY_SIZE;
    return e;
}

Enemy EnemyRewind(Enemy e)
{
    if (!e.active) return e;
    e.pos = e.history[e.historyIndex]; /* posicao mais antiga do buffer (~6 s atras) */
    e.dir = (Vector2){ 0, 0 };
    return e;
}

void EnemiesUpdate(Enemy *es, int n, Vector2 playerPos, float dt)
{
    for (int i = 0; i < n; i++) es[i] = EnemyStep(es[i], playerPos, dt); /* map */
}

void EnemiesRewind(Enemy *es, int n)
{
    for (int i = 0; i < n; i++) es[i] = EnemyRewind(es[i]); /* map */
}

bool EnemiesHitPlayer(const Enemy *es, int n, Vector2 playerPos, float playerRadius)
{
    bool hit = false; /* fold (any) */
    for (int i = 0; i < n; i++)
        hit = hit || (es[i].active && CheckCollisionCircles(playerPos, playerRadius, es[i].pos, es[i].radius));
    return hit;
}

int EnemiesCountActive(const Enemy *es, int n)
{
    int count = 0; /* fold (count) */
    for (int i = 0; i < n; i++)
        if (es[i].active) count++;
    return count;
}

void EnemiesDraw(const Enemy *es, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (!es[i].active) continue;
        DrawCircleV(es[i].pos, es[i].radius + 2, PURPLE);
        DrawCircleV(es[i].pos, es[i].radius, (Color){ 180, 100, 220, 255 });
        DrawCircle((int)es[i].pos.x - 3, (int)es[i].pos.y - 2, 2, WHITE);
        DrawCircle((int)es[i].pos.x + 3, (int)es[i].pos.y - 2, 2, WHITE);
    }
}
