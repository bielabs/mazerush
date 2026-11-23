#include "collectible.h"

Collectible CollectibleInit(int col, int row, bool active)
{
    return (Collectible){ .col = col, .row = row, .active = active };
}

Collectible CollectibleTryCollect(Collectible c, int col, int row)
{
    if (c.active && c.col == col && c.row == row) c.active = false;
    return c;
}

int CollectiblesCollectAt(Collectible *cs, int n, int col, int row)
{
    int collected = 0; /* acumulador do fold */

    for (int i = 0; i < n; i++)
    {
        Collectible next = CollectibleTryCollect(cs[i], col, row); /* map */
        if (cs[i].active && !next.active) collected++;             /* fold */
        cs[i] = next;
    }
    return collected;
}

int CollectiblesCountActive(const Collectible *cs, int n)
{
    int active = 0;
    for (int i = 0; i < n; i++)
        if (cs[i].active) active++;
    return active;
}

int CollectiblesFilterActive(Collectible *cs, int n)
{
    int j = 0;
    for (int i = 0; i < n; i++)
    {
        if (cs[i].active)
        {
            cs[j] = cs[i];
            j++;
        }
    }
    return j;
}

bool CollectiblesHasAt(const Collectible *cs, int n, int col, int row)
{
    bool found = false;
    for (int i = 0; i < n; i++)
        found = found || (cs[i].active && cs[i].col == col && cs[i].row == row);
    return found;
}
