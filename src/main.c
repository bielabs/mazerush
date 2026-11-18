#include "game.h"

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define TARGET_FPS    60

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Rush");
    SetTargetFPS(TARGET_FPS);

    Game game = GameInit();

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        GameUpdate(&game, dt);

        BeginDrawing();
        GameDraw(&game, dt);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
