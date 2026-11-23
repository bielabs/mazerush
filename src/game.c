#include "game.h"
#include "logic.h"
#include "map.h"

/* ------------------------------------------------------------------ */
/* Inicializacao e helpers de estado                                   */
/* ------------------------------------------------------------------ */

Game GameInit(int screenWidth, int screenHeight)
{
    Game game = { 0 };
    game.state = MENU;
    game.mode = MODE_WAVES;
    game.screenWidth = screenWidth;
    game.screenHeight = screenHeight;
    game.camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    game.camera.zoom = 1.0f;
    return game;
}

/* Sorteia uma posicao valida longe de "from" (tenta algumas vezes). */
static Vector2 RandomPositionAwayFrom(Vector2 from, float minDist)
{
    Vector2 pos = GetRandomValidPosition();
    for (int attempt = 0; attempt < 50; attempt++)
    {
        float dx = pos.x - from.x;
        float dy = pos.y - from.y;
        if (dx * dx + dy * dy >= minDist * minDist) break;
        pos = GetRandomValidPosition();
    }
    return pos;
}

static void SpawnEnemies(Game *g)
{
    int active = EnemyCountFor(g->wave, g->mode);
    float speed = EnemySpeedFor(g->wave);

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        Vector2 pos = RandomPositionAwayFrom(g->player.pos, 6.0f * TILE_SIZE);
        g->enemies[i] = EnemyInit(pos, speed, i < active);
    }
}

/* Repoe moedas ate MAX_COLLECTIBLES, em celulas livres e distintas. */
static void RefillCollectibles(Game *g)
{
    int playerCol = TileOf(g->player.pos.x);
    int playerRow = TileOf(g->player.pos.y);

    while (g->collectibleCount < MAX_COLLECTIBLES)
    {
        Vector2 p = GetRandomValidPosition();
        int col = TileOf(p.x);
        int row = TileOf(p.y);

        if (CollectiblesHasAt(g->collectibles, g->collectibleCount, col, row)) continue;
        if (col == playerCol && row == playerRow) continue;

        g->collectibles[g->collectibleCount++] = CollectibleInit(col, row, true);
    }
}

static void GameStart(Game *g)
{
    g->player = PlayerInit(GetRandomValidPosition());
    g->score = 0;
    g->lives = START_LIVES;
    g->wave = 1;
    g->waveItems = 0;
    g->itemsTarget = ItemsTargetFor(1);
    g->timeLeft = ROUND_TIME;
    g->collectibleCount = 0;

    RefillCollectibles(g);
    SpawnEnemies(g);
    g->camera.target = g->player.pos;
    g->state = PLAYING;
}

/* ------------------------------------------------------------------ */
/* Update                                                              */
/* ------------------------------------------------------------------ */

static Rectangle MenuButton(const Game *g, int index)
{
    float centerX = g->screenWidth / 2.0f;
    float startY = g->screenHeight / 2.0f - 50;
    return (Rectangle){ centerX - 160, startY + 65.0f * index, 320, 45 };
}

static void UpdateMenu(Game *g)
{
    bool start = false;

    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        g->mode = (g->mode == MODE_WAVES) ? MODE_TIME : MODE_WAVES;

    Vector2 mouse = GetMousePosition();
    for (int i = 0; i < 2; i++)
    {
        if (CheckCollisionPointRec(mouse, MenuButton(g, i)))
        {
            g->mode = (GameMode)i;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) start = true;
        }
    }

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) start = true;
    if (start) GameStart(g);
}

static void UpdatePlaying(Game *g, float dt)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        g->state = MENU;
        return;
    }

    /* jogador */
    g->player = PlayerTick(g->player, dt);
    g->player = PlayerMove(g->player, PlayerReadInput(), dt);

    /* inimigos (map) */
    EnemiesUpdate(g->enemies, MAX_ENEMIES, g->player.pos, dt);

    /* dano (fold: algum inimigo tocou o jogador?) */
    if (g->player.invincibilityTimer <= 0.0f &&
        EnemiesHitPlayer(g->enemies, MAX_ENEMIES, g->player.pos, g->player.radius))
    {
        g->lives--;
        g->player = PlayerHit(g->player);
        EnemiesRewind(g->enemies, MAX_ENEMIES);
    }

    /* moedas: coleta (map+fold), compacta (filter), repoe */
    int collected = CollectiblesCollectAt(g->collectibles, g->collectibleCount,
                                          TileOf(g->player.pos.x), TileOf(g->player.pos.y));
    if (collected > 0)
    {
        g->score = ScoreAfter(g->score, collected);
        g->waveItems += collected;
        g->collectibleCount = CollectiblesFilterActive(g->collectibles, g->collectibleCount);

        if (WaveCompleted(g->mode, g->waveItems, g->itemsTarget))
        {
            g->wave++;
            g->waveItems = 0;
            g->itemsTarget = ItemsTargetFor(g->wave);
            SpawnEnemies(g);
        }
        RefillCollectibles(g);
    }

    /* tempo e fim de partida */
    if (g->mode == MODE_TIME) g->timeLeft = TimeAfter(g->timeLeft, dt);
    if (IsRoundOver(g->mode, g->timeLeft, g->lives)) g->state = GAME_OVER;

    g->camera.target = g->player.pos;
}

void GameUpdate(Game *g, float dt)
{
    switch (g->state)
    {
        case MENU:      UpdateMenu(g);          break;
        case PLAYING:   UpdatePlaying(g, dt);   break;
        case GAME_OVER:
            if (IsKeyPressed(KEY_ENTER)) g->state = MENU;
            break;
    }
}

/* ------------------------------------------------------------------ */
/* Draw                                                                */
/* ------------------------------------------------------------------ */

static void DrawTextCentered(const char *text, int centerX, int y, int size, Color color)
{
    DrawText(text, centerX - MeasureText(text, size) / 2, y, size, color);
}

static void DrawMenu(const Game *g)
{
    int cx = g->screenWidth / 2;
    int startY = g->screenHeight / 2 - 50;
    const char *labels[2] = { "1. MODO ONDAS (Progressivo)", "2. MODO TEMPO (45 Segundos)" };

    DrawTextCentered("MAZE RUSH", cx, startY - 130, 44, (Color){ 255, 100, 100, 255 });
    DrawTextCentered("Escolha o modo (setas/W-S ou clique):", cx, startY - 70, 18, LIGHTGRAY);

    for (int i = 0; i < 2; i++)
    {
        Color color = ((int)g->mode == i) ? (Color){ 70, 130, 240, 255 } : (Color){ 45, 50, 70, 255 };
        DrawRectangleRounded(MenuButton(g, i), 0.3f, 4, color);
        DrawTextCentered(labels[i], cx, startY + 13 + 65 * i, 18, RAYWHITE);
    }

    DrawTextCentered("Pressione [ENTER] para iniciar", cx, startY + 150, 20, GOLD);
    DrawTextCentered("Controles: WASD ou setas | ESC volta ao menu", cx, startY + 195, 16, GRAY);
}

static void DrawHud(const Game *g)
{
    DrawRectangle(0, 0, g->screenWidth, 50, (Color){ 15, 15, 20, 230 });

    if (g->mode == MODE_WAVES)
    {
        DrawText(TextFormat("ONDA: %d", g->wave), 40, 15, 20, (Color){ 100, 200, 255, 255 });
        DrawText(TextFormat("RESTAM: %d", g->itemsTarget - g->waveItems), 200, 15, 20, (Color){ 255, 200, 100, 255 });
    }
    else
    {
        DrawText("MODO: TEMPO", 40, 15, 20, (Color){ 100, 200, 255, 255 });
        DrawText(TextFormat("TEMPO: %.1fs", g->timeLeft), 220, 15, 20, (Color){ 255, 200, 100, 255 });
    }
    DrawText(TextFormat("VIDAS: %d", g->lives), 420, 15, 20, (Color){ 255, 100, 100, 255 });
    DrawText(TextFormat("TOTAL: %d", g->score), g->screenWidth - 180, 15, 20, (Color){ 100, 255, 100, 255 });
}

static void DrawGameOver(const Game *g)
{
    int cx = g->screenWidth / 2;
    int cy = g->screenHeight / 2;

    DrawRectangle(0, 0, g->screenWidth, g->screenHeight, Fade(BLACK, 0.85f));
    DrawTextCentered("FIM DE JOGO!", cx, cy - 100, 44, RAYWHITE);
    DrawTextCentered(TextFormat("Total de itens coletados: %d", g->score), cx, cy - 30, 20, GOLD);
    if (g->mode == MODE_WAVES)
        DrawTextCentered(TextFormat("Onda alcancada: %d", g->wave), cx, cy, 20, GOLD);
    DrawTextCentered("Pressione [ENTER] para voltar ao menu", cx, cy + 60, 18, LIGHTGRAY);
}

void GameDraw(const Game *g)
{
    ClearBackground((Color){ 20, 20, 30, 255 });

    if (g->state == MENU)
    {
        DrawMenu(g);
        return;
    }

    BeginMode2D(g->camera);
        DrawMap();

        for (int i = 0; i < g->collectibleCount; i++)
        {
            if (!g->collectibles[i].active) continue;
            Vector2 c = TileCenter(g->collectibles[i].col, g->collectibles[i].row);
            DrawCircleV(c, 9.0f, GOLD);
            DrawCircleV(c, 5.0f, YELLOW);
        }

        EnemiesDraw(g->enemies, MAX_ENEMIES);
        PlayerDraw(&g->player);
    EndMode2D();

    DrawHud(g);
    if (g->state == GAME_OVER) DrawGameOver(g);
}
