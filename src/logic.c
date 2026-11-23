#include "logic.h"

int TileOf(float coord)
{
    return (int)(coord / TILE_SIZE);
}

Vector2 NextPosition(Vector2 pos, Vector2 dir, float speed, float dt)
{
    return (Vector2){ pos.x + dir.x * speed * dt, pos.y + dir.y * speed * dt };
}

int EnemyCountFor(int wave, GameMode mode)
{
    if (mode == MODE_TIME) return 4;

    int count = 2 + (wave - 1);
    return count > MAX_ENEMIES ? MAX_ENEMIES : count;
}

float EnemySpeedFor(int wave)
{
    return 160.0f + wave * 25.0f;
}

int ItemsTargetFor(int wave)
{
    return 5 + 2 * (wave - 1);
}

bool WaveCompleted(GameMode mode, int collectedInWave, int target)
{
    return mode == MODE_WAVES && collectedInWave >= target;
}

float TimeAfter(float timeLeft, float dt)
{
    float t = timeLeft - dt;
    return t < 0.0f ? 0.0f : t;
}

bool IsRoundOver(GameMode mode, float timeLeft, int lives)
{
    return lives <= 0 || (mode == MODE_TIME && timeLeft <= 0.0f);
}

int ScoreAfter(int score, int collected)
{
    return score + collected;
}
