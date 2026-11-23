#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include <stdbool.h>

/* Moeda posicionada em uma celula (col, row) do labirinto. */
typedef struct
{
    int col;
    int row;
    bool active;
} Collectible;

Collectible CollectibleInit(int col, int row, bool active);

/* Pura: devolve a moeda coletada (inativa) se estiver na celula, senao a mesma. */
Collectible CollectibleTryCollect(Collectible c, int col, int row);

/* map + fold: aplica TryCollect em todas e retorna quantas foram coletadas. */
int CollectiblesCollectAt(Collectible *cs, int n, int col, int row);

/* fold: conta as moedas ativas. */
int CollectiblesCountActive(const Collectible *cs, int n);

/* filter: compacta mantendo so as ativas e retorna o novo tamanho. */
int CollectiblesFilterActive(Collectible *cs, int n);

/* fold (any): existe moeda ativa nessa celula? */
bool CollectiblesHasAt(const Collectible *cs, int n, int col, int row);

#endif
