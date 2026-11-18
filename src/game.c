#include "game.h"

Game GameInit(void)
{
    Game game = {
        .state = MENU,
        .score = 0,
        .elapsed = 0.0f
    };

    game.player = PlayerInit((Rectangle){ 0 }, 0.0f);

    for (int i = 0; i < MAX_COLLECTIBLES; i++)
    {
        game.collectibles[i] = CollectibleInit((Rectangle){ 0 }, false);
    }

    return game;
}

void GameUpdate(Game *game, float dt)
{
    game->elapsed += dt;
}

void GameDraw(const Game *game, float dt)
{
    ClearBackground(RAYWHITE);
    DrawText("Maze Rush", 20, 20, 40, DARKGRAY);
    DrawText(TextFormat("Tempo: %.2f s", game->elapsed), 20, 80, 20, DARKGRAY);
    DrawText(TextFormat("dt: %.4f s", dt), 20, 110, 20, DARKGRAY);
    DrawFPS(20, 140);
}
