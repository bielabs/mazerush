#include "collectible.h"

Collectible CollectibleInit(Rectangle bounds, bool active)
{
    return (Collectible){ .bounds = bounds, .active = active };
}
