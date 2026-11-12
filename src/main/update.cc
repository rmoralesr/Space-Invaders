#ifndef UPDATE_CC
#define UPDATE_CC 1

#include <esat/draw.h>
#include "../tools/input.cc"
#include "../core/types.cc"
#include "../entities/body.cc"

// FUNCIONES DE ACTUALIZACIÓN DE VALORES DEL JUEGO

void MatarAlien(AlienArmy *alien_army) { // TODO quitar
	Destruct(alien_army, g_tick%55);
}

void PreviewUpdate(Game *game) {

}

/** @brief Actualiza los datos del juego y pinta los necesarios */
void GameUpdate(Game *game) {

	g_tick++;
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
				game->game_state = kGameState_Playing;
				AlienArmy_Init(&game->alien_army);
			}
			break;
		}
		case kGameState_Playing: {
			AlienArmy_Control(&game->alien_army);
			if (game->input.insert_credit) MatarAlien(&game->alien_army);
			break;
		}
	}
}

#endif // UPDATE_CC