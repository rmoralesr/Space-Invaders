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
	InputUpdate(&game->input);

	switch (game->game_state) {
		case kGameState_Previewing: {
			PreviewUpdate(game);
			break;
		}
		case kGameState_ChoosingPlayers: {
			if (game->input.one_player_button || game->input.two_player_button) { // TODO por ahora así, luego cambiar
				game->game_state = kGameState_Playing;
			}
			break;
		}
		case kGameState_Playing: {
			break;
		}
	}
}

#endif // UPDATE_CC