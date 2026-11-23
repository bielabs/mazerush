#include "game.h"
#include <time.h>

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Rush");
    SetTargetFPS(TARGET_FPS);
    SetExitKey(KEY_NULL); /* ESC volta ao menu em vez de fechar a janela */
    SetRandomSeed((unsigned int)time(NULL));

    Game game = GameInit(SCREEN_WIDTH, SCREEN_HEIGHT);

    while (!WindowShouldClose())
    {
        GameUpdate(&game, GetFrameTime());

        BeginDrawing();
        GameDraw(&game);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
