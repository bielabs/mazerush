#include "game.h"

Game GameInit(void)
{
    return (Game){ .state = MENU };
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
