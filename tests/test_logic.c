/*
 * Testes da logica pura (sem janela e sem Raylib em execucao).
 * Rodar com: make test
 */
#include "collectible.h"
#include "logic.h"
#include <math.h>
#include <stdio.h>

static int failures = 0;

#define CHECK(cond)                                                     \
    do {                                                                \
        if (!(cond)) {                                                  \
            printf("FALHOU (linha %d): %s\n", __LINE__, #cond);         \
            failures++;                                                 \
        }                                                               \
    } while (0)

static void test_logic(void)
{
    CHECK(TileOf(0.0f) == 0);
    CHECK(TileOf(79.9f) == 1);
    CHECK(TileOf(80.0f) == 2);

    Vector2 p = NextPosition((Vector2){ 100, 100 }, (Vector2){ 1, 0 }, 200.0f, 0.5f);
    CHECK(fabsf(p.x - 200.0f) < 0.001f && fabsf(p.y - 100.0f) < 0.001f);

    CHECK(EnemyCountFor(1, MODE_WAVES) == 2);
    CHECK(EnemyCountFor(3, MODE_WAVES) == 4);
    CHECK(EnemyCountFor(99, MODE_WAVES) == MAX_ENEMIES);
    CHECK(EnemyCountFor(1, MODE_TIME) == 4);

    CHECK(EnemySpeedFor(2) > EnemySpeedFor(1));
    CHECK(ItemsTargetFor(1) == 5);
    CHECK(ItemsTargetFor(3) == 9);

    CHECK(WaveCompleted(MODE_WAVES, 5, 5));
    CHECK(!WaveCompleted(MODE_WAVES, 4, 5));
    CHECK(!WaveCompleted(MODE_TIME, 99, 5));

    CHECK(TimeAfter(1.0f, 0.4f) > 0.59f && TimeAfter(1.0f, 0.4f) < 0.61f);
    CHECK(TimeAfter(0.1f, 1.0f) == 0.0f);

    CHECK(IsRoundOver(MODE_WAVES, 45.0f, 0));
    CHECK(!IsRoundOver(MODE_WAVES, 0.0f, 3));
    CHECK(IsRoundOver(MODE_TIME, 0.0f, 3));
    CHECK(!IsRoundOver(MODE_TIME, 10.0f, 3));

    CHECK(ScoreAfter(7, 2) == 9);
}

static void test_collectibles(void)
{
    Collectible cs[3] = {
        CollectibleInit(1, 1, true),
        CollectibleInit(2, 2, true),
        CollectibleInit(3, 3, true)
    };

    /* TryCollect e pura: nao altera o original */
    Collectible c = CollectibleInit(5, 5, true);
    Collectible d = CollectibleTryCollect(c, 5, 5);
    CHECK(c.active && !d.active);
    CHECK(CollectibleTryCollect(c, 4, 5).active);

    CHECK(CollectiblesCountActive(cs, 3) == 3);
    CHECK(CollectiblesHasAt(cs, 3, 2, 2));
    CHECK(!CollectiblesHasAt(cs, 3, 9, 9));

    CHECK(CollectiblesCollectAt(cs, 3, 9, 9) == 0);
    CHECK(CollectiblesCollectAt(cs, 3, 2, 2) == 1);
    CHECK(CollectiblesCollectAt(cs, 3, 2, 2) == 0); /* ja coletada */
    CHECK(CollectiblesCountActive(cs, 3) == 2);

    int n = CollectiblesFilterActive(cs, 3);
    CHECK(n == 2);
    CHECK(cs[0].col == 1 && cs[1].col == 3);

    CHECK(CollectiblesFilterActive(cs, 0) == 0); /* lista vazia */
}

int main(void)
{
    test_logic();
    test_collectibles();

    if (failures == 0) printf("Todos os testes passaram.\n");
    else printf("%d teste(s) falharam.\n", failures);
    return failures == 0 ? 0 : 1;
}
