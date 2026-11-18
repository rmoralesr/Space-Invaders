#ifndef UPDATE_HH
#define UPDATE_HH 1
#include "../core/game.hh"

void AboutToPlayInit(Game_t *game);

void PlayingInit(Game_t *game);
void GameOverInit(Game_t *game);

void PlayerInput(Game_t *game);

void PlayerLogic(Game_t *game);

void SpendCredits(Game_t *game, int number_credits);

void PreviewingInit(Game_t *game);
void DemoInit(Game_t *game);

void InsertCoinInit(Game_t *game);

void ChoosingPlayerInit(Game_t *game);

/** @brief Actualiza los datos del juego y pinta los necesarios */
void GameUpdate(Game_t *game);

#endif // UPDATE_HH