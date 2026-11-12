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

const unsigned int Update_kQueueCapacity = 10;
QueuedUpdate Update_queue[Update_kQueueCapacity] = {0};

// FUNCIONES DE ACTUALIZACIÓN DE VALORES DEL JUEGO

void QueueUpdate(int id, long long int tick){
    int n = 0;
    bool free_space = false;
    while(n < Update_kQueueCapacity && !free_space){
        if (!Update_queue[n].active) free_space = true;
        else n++;
    }
	if (free_space){
		Update_queue[n].active = true;
		Update_queue[n].id = id;
		Update_queue[n].queued_tick = tick;
	}
}

void PreviewUpdate(Game *game) {

}

void PreviewingInit(){
	GenerateText("play",4,{97,65}, 0, ProgressiveTextTickRate);
	GenerateText("space  invaders",15,{57,89}, 4*ProgressiveTextTickRate, ProgressiveTextTickRate);

	GenerateText("*score advance table*",21,{33,121},(4+4+15)*ProgressiveTextTickRate);
	GenerateUIElement({false, {64,136}, g_assets.UFOMasks[0]},(4+4+15)*ProgressiveTextTickRate);
	GenerateUIElement({false, {66,152}, g_assets.AlienMasks[2][0]},(4+4+15)*ProgressiveTextTickRate);
	GenerateUIElement({false, {66,168}, g_assets.AlienMasks[1][0]},(4+4+15)*ProgressiveTextTickRate);
	GenerateUIElement({false, {66,184}, g_assets.AlienMasks[0][0]},(4+4+15)*ProgressiveTextTickRate);

	GenerateText("=? mystery",10,{81,137},(4+4+15)*ProgressiveTextTickRate, ProgressiveTextTickRate);
	GenerateText("=30 points",10,{81,153},(4+4+15+10)*ProgressiveTextTickRate, ProgressiveTextTickRate);
	GenerateText("=20 points",10,{81,169},(4+4+15+20)*ProgressiveTextTickRate, ProgressiveTextTickRate);
	GenerateText("=10 points",10,{81,185},(4+4+15+30)*ProgressiveTextTickRate, ProgressiveTextTickRate);

	WipeScreenToContent(&g_wipe_screen);
	g_wipe_screen.queued_tick = g_tick + (4+4+15+40)*ProgressiveTextTickRate + 8;

	QueueUpdate(0, g_tick + (4+4+15+40)*ProgressiveTextTickRate + 20);
}

void InsertCoinInit(){
	GenerateText("insert  coin",12,{65,113});

	GenerateText("<1 or 2 players>",16,{49,143}, 0, ProgressiveTextTickRate);
	GenerateText("*1 player  1 coin",17,{49,169}, 16*ProgressiveTextTickRate, ProgressiveTextTickRate);
	GenerateText("*2 players 2 coins",18,{49,195}, (16+17)*ProgressiveTextTickRate, ProgressiveTextTickRate);

}

/** @brief Actualiza los datos del juego y pinta los necesarios */
void GameUpdate(Game *game) {

	g_tick++;

	// Player Inputs
	InputUpdate(&game->input);
	if (game->input.insert_credit) game->credits++;

	// Text
	if (g_tick == g_wipe_screen.queued_tick) g_wipe_screen.current_step = 0;
	AdvanceWipeScreen(&g_wipe_screen);

	// Queued Updates
	for (int i = 0; i < Update_kQueueCapacity; i++){
		if (Update_queue[i].active && Update_queue[i].queued_tick == g_tick){
			Update_queue[i].active = false;
			switch(Update_queue[i].id){
				case 0 : 
					ClearUIContent();
					InsertCoinInit();
					break;
			}
		}
	}

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