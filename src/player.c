#include "player.h"

Player PlayerInit(Rectangle bounds, float speed)
{
    return (Player){ .bounds = bounds, .speed = speed };
}
