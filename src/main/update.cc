#ifndef UPDATE_CC
#define UPDATE_CC 1

#include <esat/draw.h>
#include "../core/types.cc"
#include "../entities/body.cc"
#include "../tools/ui-element.cc"
#include "../tools/input.cc"


// FUNCIONES DE ACTUALIZACIÓN DE VALORES DEL JUEGO

void PreviewUpdate(Game_t *game) {

}

void PreviewingInit(){
	ClearText();

	GenerateProgressiveText(
		"play", 
		{ 100, 80 }, 
		0
	);

	GenerateProgressiveText(
		"space  invaders",
		{ 50, 100 },
		4*6
	);

    /*Body_t buddy = {
        true,
        {0,0}, {}
    };
    FromFile(&buddy.mask, "../../assets/test.png");
	QueueUIElement(buddy, 10);*/
}

/** @brief Actualiza los datos del juego y pinta los necesarios */
void GameUpdate(Game_t *game) {

	g_tick++;
	// Player Inputs

	InputUpdate(&game->input);
	if (game->input.insert_credit) game->credits++;

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