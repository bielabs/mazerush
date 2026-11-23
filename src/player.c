#include "player.h"
#include "logic.h"
#include "map.h"

Player PlayerInit(Vector2 pos)
{
    return (Player){ .pos = pos, .radius = 12.0f, .speed = 210.0f, .invincibilityTimer = 0.0f };
}

Vector2 PlayerReadInput(void)
{
    Vector2 input = { 0.0f, 0.0f };
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    input.y -= 1.0f;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  input.y += 1.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  input.x -= 1.0f;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) input.x += 1.0f;
    return input;
}

Player PlayerMove(Player p, Vector2 input, float dt)
{
    if (input.x != 0.0f && input.y != 0.0f)
    {
        input.x *= 0.7071f;
        input.y *= 0.7071f;
    }

    /* eixo X e eixo Y separados para deslizar nas paredes */
    Vector2 next = NextPosition(p.pos, (Vector2){ input.x, 0.0f }, p.speed, dt);
    if (!CheckWallCollision(next, p.radius)) p.pos = next;

    next = NextPosition(p.pos, (Vector2){ 0.0f, input.y }, p.speed, dt);
    if (!CheckWallCollision(next, p.radius)) p.pos = next;

    return p;
}

Player PlayerTick(Player p, float dt)
{
    if (p.invincibilityTimer > 0.0f) p.invincibilityTimer -= dt;
    return p;
}

Player PlayerHit(Player p)
{
    p.invincibilityTimer = INVINCIBILITY_TIME;
    return p;
}

void PlayerDraw(const Player *p)
{
    /* pisca enquanto invencivel */
    if (p->invincibilityTimer > 0.0f && (int)(p->invincibilityTimer * 15) % 2 == 0) return;

    DrawCircleV(p->pos, p->radius + 2, MAROON);
    DrawCircleV(p->pos, p->radius, RED);
}
