#ifndef UPDATE_CC
#define UPDATE_CC 1

#include <esat/draw.h>
#include "tools/input.cc"
#include "core/types.cc"
#include "entities/body.cc"

// FUNCIONES DE ACTUALIZACIÓN DE VALORES DEL JUEGO

void PreviewUpdate(Game *game) {
	Body_t buddy = {
		true,
		{20,20}, {}
	};

	FromFile(&buddy.mask, "../assets/test.png");

	Draw(&buddy);
}

/** @brief Actualiza los datos del juego y pinta los necesarios */
void GameUpdate(Game *game) {

	// Player Inputs

	InputUpdate(&game->input);
	if (game->input.insert_credit) game->credits++;

	// 

	switch (game->game_state) {
		case kGameState_Previewing: {
			PreviewUpdate(game);
			if (game->credits > 0) game->game_state = kGameState_ChoosingPlayers;
			break;
		}
		case kGameState_ChoosingPlayers: {
			if (game->input.one_player_button || game->input.two_player_button) { // TODO por ahora así, luego cambiar
				game->player = Player_Construct();
				game->game_state = kGameState_Playing;
			}
			break;
		}
		case kGameState_Playing: {
			if (game->input.left_button) {
				Move(game->player, {-1, 0});
  				Draw(&game->player->body);
			}
			if (game->input.right_button) {
				Move(game->player, {+1, 0});
				Draw(&game->player->body);
			}
			break;
		}
	}
}

#endif // UPDATE_CC