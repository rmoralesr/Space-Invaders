#ifndef UPDATE_CC
#define UPDATE_CC 1

#include <esat/draw.h>
#include "../core/types.cc"
#include "../entities/body.cc"
#include "../tools/ui-element.cc"
#include "../tools/asset-loader.cc"
#include "../tools/input.cc"


struct QueuedUpdate{
	bool active = false;
	int id;
	long long int queued_tick;
};

// FUNCIONES DE ACTUALIZACIÓN DE VALORES DEL JUEGO

void PreviewUpdate(Game_t *game) {

}

void PreviewingInit(){
	ClearUIElementGenerateQueue();
	GenerateProgressiveText("play",4,{97,65}, 0);
	GenerateProgressiveText("space  invaders",15,{57,89}, 4*ProgressiveTextTickRate);

	GenerateText("*score advance table*",21,{33,121},(4+4+15)*ProgressiveTextTickRate);
	QueueUIElement({false, {64,136}, g_assets.UFOMasks[0]},(4+4+15)*ProgressiveTextTickRate);
	QueueUIElement({false, {66,152}, g_assets.AlienMasks[2][0]},(4+4+15)*ProgressiveTextTickRate);
	QueueUIElement({false, {66,168}, g_assets.AlienMasks[1][0]},(4+4+15)*ProgressiveTextTickRate);
	QueueUIElement({false, {66,184}, g_assets.AlienMasks[0][0]},(4+4+15)*ProgressiveTextTickRate);

	GenerateProgressiveText("=? mystery",10,{81,137},(4+4+15)*ProgressiveTextTickRate);
	GenerateProgressiveText("=30 points",10,{81,153},(4+4+15+10)*ProgressiveTextTickRate);
	GenerateProgressiveText("=20 points",10,{81,169},(4+4+15+20)*ProgressiveTextTickRate);
	GenerateProgressiveText("=10 points",10,{81,185},(4+4+15+30)*ProgressiveTextTickRate);

	WipeScreenToContent(&g_wipe_screen);
	g_wipe_screen.queued_tick = g_tick + (4+4+15+40)*ProgressiveTextTickRate + 8;
}

void InsertCoinInit(){
	ClearUIElementGenerateQueue();
	GenerateText("insert  coin",12,{65,113});

	GenerateProgressiveText("<1 or 2 players>",16,{49,143}, 0);
	GenerateProgressiveText("*1 player  1 coin",17,{49,169}, 16*ProgressiveTextTickRate);
	GenerateProgressiveText("*2 players 2 coins",18,{49,195}, (16+17)*ProgressiveTextTickRate);

}

/** @brief Actualiza los datos del juego y pinta los necesarios */
void GameUpdate(Game_t *game) {

	g_tick++;

	// Player Inputs
	InputUpdate(&game->input);
	if (game->input.insert_credit) game->credits++;

	// Text
	QueuedUIElementsUpdate();
	if (g_tick == g_wipe_screen.queued_tick) g_wipe_screen.current_step = 0;
	AdvanceWipeScreen(&g_wipe_screen);

	// Queued Updates

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