#include "map.h"

static const int map[ROWS][COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1},
    {1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,0,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1},
    {1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,0,1,1,1,0,1,0,1,1,1,1,0,1,1,1,1,0,1,1,1,0,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1},
    {1,1,1,0,1,1,1,0,1,0,1,1,1,1,0,1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1},
    {1,0,1,1,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
    {1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

bool MapIsWall(int row, int col)
{
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) return true;
    return map[row][col] == 1;
}

Vector2 TileCenter(int col, int row)
{
    return (Vector2){ (float)(col * TILE_SIZE + TILE_SIZE / 2), (float)(row * TILE_SIZE + TILE_SIZE / 2) };
}

bool CheckWallCollision(Vector2 pos, float radius)
{
    int startCol = (int)((pos.x - radius) / TILE_SIZE);
    int endCol   = (int)((pos.x + radius) / TILE_SIZE);
    int startRow = (int)((pos.y - radius) / TILE_SIZE);
    int endRow   = (int)((pos.y + radius) / TILE_SIZE);

    for (int r = startRow; r <= endRow; r++)
    {
        for (int c = startCol; c <= endCol; c++)
        {
            if (MapIsWall(r, c))
            {
                Rectangle tile = { (float)(c * TILE_SIZE), (float)(r * TILE_SIZE), (float)TILE_SIZE, (float)TILE_SIZE };
                if (CheckCollisionCircleRec(pos, radius, tile)) return true;
            }
        }
    }
    return false;
}

Vector2 GetRandomValidPosition(void)
{
    int col, row;
    do
    {
        col = GetRandomValue(1, COLS - 2);
        row = GetRandomValue(1, ROWS - 2);
    } while (MapIsWall(row, col));

    return TileCenter(col, row);
}

void DrawMap(void)
{
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            Rectangle tile = { (float)(c * TILE_SIZE), (float)(r * TILE_SIZE), (float)TILE_SIZE, (float)TILE_SIZE };
            if (map[r][c] == 1)
            {
                DrawRectangleRounded(tile, 0.15f, 4, (Color){ 45, 50, 70, 255 });
            }
            else
            {
                DrawRectangleRec(tile, (Color){ 235, 240, 245, 255 });
                DrawRectangleLinesEx(tile, 1, (Color){ 215, 220, 230, 255 });
            }
        }
    }
}
