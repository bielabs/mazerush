# MazeRush

Jogo 2D em C com Raylib, desenvolvido para o Projeto Integrador de Programação Imperativa e Funcional (CESAR School, 2026.2).

## 1. Descrição do jogo

Você controla um personagem em um labirinto, coleta moedas e foge de fantasmas que perseguem o jogador. O labirinto é fixo; as moedas aparecem em posições aleatórias válidas.

## 2. Modos de jogo

| Modo | Como funciona | Fim de jogo |
|---|---|---|
| **Ondas** | Colete a meta de moedas da onda (5 na primeira, +2 a cada onda). Ao completar, a onda sobe: mais fantasmas e mais velocidade. | Quando as vidas chegam a zero. |
| **Tempo** | Rodada de 45 segundos com 4 fantasmas. Colete o máximo possível. | Quando o tempo chega a zero ou as vidas acabam. |

## 3. Regras

1. O jogador começa com 3 vidas.
2. Cada moeda coletada vale 1 ponto. Sempre há 5 moedas no mapa; ao coletar uma, outra surge em outra posição.
3. As paredes bloqueiam o jogador e os fantasmas.
4. Tocar em um fantasma tira uma vida, dá 4 s de invencibilidade (o personagem pisca) e todos os fantasmas voltam para a posição de ~6 s antes.
5. Ao fim da partida são mostrados o total de moedas (e a onda alcançada, no modo Ondas), e o jogador pode voltar ao menu e jogar de novo quantas vezes quiser.

## 4. Controles

| Tecla / ação | Efeito |
|---|---|
| `W A S D` ou setas | Mover |
| `↑ ↓` / `W S` no menu, ou mouse | Escolher modo |
| `Enter` / `Espaço` / clique | Iniciar partida |
| `Esc` (durante o jogo) | Voltar ao menu |
| `Enter` (tela final) | Voltar ao menu |

## 5. Como compilar e executar

Pré-requisitos: `gcc`, `make` e a [Raylib](https://www.raylib.com/) instalada.

**Linux** (Raylib instalada no sistema, com `libGL` e X11):

```sh
make        # compila
make run    # compila e executa
make test   # testes da lógica pura (não abre janela)
make clean
```

**macOS**: instale com `brew install raylib` e use os mesmos comandos.

**Windows**: instale o pacote Raylib com w64devkit (padrão `C:/raylib/w64devkit`). Se estiver em outro lugar, use `make RAYLIB=D:/caminho/raylib/w64devkit`.

## 6. Aplicação dos paradigmas

**Imperativo.** O `main.c` contém só o game loop (`GameUpdate` → `GameDraw` a cada quadro, com `GetFrameTime()`). O estado fica no struct `Game`; a entrada de teclado e o desenho ficam isolados.

**Funcional.** A lógica sobre coleções e as regras estão em funções puras, que podem ser testadas sem a janela (`tests/test_logic.c`):

| Padrão | Onde |
|---|---|
| **map** | `EnemiesUpdate`, `EnemiesRewind` (`enemy.c`): aplicam a mesma transformação a cada inimigo. `CollectibleTryCollect` aplicada a todas as moedas em `CollectiblesCollectAt` (`collectible.c`). |
| **filter** | `CollectiblesFilterActive` (`collectible.c`): compacta mantendo só as moedas ativas. |
| **fold** | `CollectiblesCountActive`, `CollectiblesHasAt` (`collectible.c`); `EnemiesHitPlayer`, `EnemiesCountActive` (`enemy.c`); acumulador de pontuação em `CollectiblesCollectAt`. |
| **Funções puras** | `logic.c` (`NextPosition`, `EnemyCountFor`, `EnemySpeedFor`, `ItemsTargetFor`, `WaveCompleted`, `TimeAfter`, `IsRoundOver`, `ScoreAfter`) e `PlayerMove`, `PlayerTick`, `PlayerHit`, `EnemyStep`, `EnemyRewind`, que recebem uma struct e devolvem uma nova. |

Os efeitos (teclado, desenho, números aleatórios) ficam em camadas separadas: `PlayerReadInput`, `GameDraw`, `GameStart`/`RefillCollectibles`.

## 7. Estrutura do código

- `main.c`: abre a janela e roda o game loop.
- `config.h`: constantes (tela, mapa, limites) e enums `GameState` e `GameMode`.
- `game.h` / `game.c`: struct `Game`, máquina de estados (menu / jogo / fim), regras da partida, HUD e telas.
- `map.h` / `map.c`: matriz do labirinto, colisão com paredes e desenho.
- `player.h` / `player.c`: jogador (movimento, invencibilidade, desenho).
- `enemy.h` / `enemy.c`: fantasmas (IA em grade, histórico de posições) e operações sobre o vetor.
- `collectible.h` / `collectible.c`: moedas e operações map/filter/fold sobre o vetor.
- `logic.h` / `logic.c`: regras do jogo em funções puras.
- `tests/test_logic.c`: testes da lógica pura.

## Equipe
- Gabriel Araujo
- Heitor Torres
- Isabelly Ribeiro
- Rafael de Andrade
- Rubens Sousa
