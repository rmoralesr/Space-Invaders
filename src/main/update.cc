#ifndef UPDATE_CC
#define UPDATE_CC 1

#include <esat/draw.h>
#include "../core/types.cc"
#include "../entities/body.cc"
#include "../tools/ui-element.cc"
#include "../tools/input.cc"


// FUNCIONES DE ACTUALIZACIÓN DE VALORES DEL JUEGO

void PreviewUpdate(Game *game) {

}

void PreviewingInit(){
	ClearCharacterGenerateQueue();
	GenerateProgressiveText("play",4,{100,80}, 0);
	GenerateProgressiveText("space  invaders",15,{50,100}, 4);
}

/** @brief Actualiza los datos del juego y pinta los necesarios */
void GameUpdate(Game *game) {

	g_tick++;
	// Player Inputs

	InputUpdate(&game->input);
	if (game->input.insert_credit) game->credits++;

	// Text
	QueuedCharactersUpdate();

	switch (game->game_state) {
		case kGameState_Previewing: {
			PreviewUpdate(game);
			if (game->credits > 0) game->game_state = kGameState_ChoosingPlayers;
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