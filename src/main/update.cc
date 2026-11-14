#ifndef UPDATE_CC
#define UPDATE_CC 1

#include <esat/draw.h>
#include "../core/types.cc"
#include "../entities/body.cc"
#include "../tools/ui-element.cc"
#include "../tools/asset-loader.cc"
#include "../tools/input.cc"
#include "../entities/aliens_army.cc"
#include "../entities/player.cc"

enum kUpdateID {
    kUpdateID_InsertCoin,
    kUpdateID_ChoosingPlayer,
    kUpdateID_AboutToPlay,
    kUpdateID_Playing
};

struct QueuedUpdate{
	bool active = false;
	int id;
	long long int queued_tick;
};

const unsigned int Update_kQueueCapacity = 10;
QueuedUpdate Update_queue[Update_kQueueCapacity] = {0};

// FUNCIONES DE ACTUALIZACIÓN DE VALORES DEL JUEGO

void MatarAlien(AlienArmy_t *alien_army) { // TODO quitar
	Destruct(alien_army, g_tick%55);
}

void MatarPlayer(Player_t *player) { // TODO quitar
	Dead(player, 1);
}

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
void ClearQueuedActions(){
	for (int i = 0; i < Update_kQueueCapacity; i++){
		Update_queue[i].active = false;
	}
	g_wipe_screen.current_step = -1;
	g_wipe_screen.queued_tick = 0;
}

void PreviewingInit(){
	GenerateText("play",{97,65}, 0, ProgressiveTextTickRate);
	GenerateText("space  invaders",{57,89}, 4*ProgressiveTextTickRate, ProgressiveTextTickRate);

	GenerateText("*score advance table*",{33,121},(4+4+15)*ProgressiveTextTickRate);
	GenerateUIElement({false, {64,136}, g_assets.UFOMasks[0]},(4+4+15)*ProgressiveTextTickRate);
	GenerateUIElement({false, {66,152}, g_assets.AlienMasks[2][0]},(4+4+15)*ProgressiveTextTickRate);
	GenerateUIElement({false, {66,168}, g_assets.AlienMasks[1][0]},(4+4+15)*ProgressiveTextTickRate);
	GenerateUIElement({false, {66,184}, g_assets.AlienMasks[0][0]},(4+4+15)*ProgressiveTextTickRate);

	GenerateText("=? mystery",{81,137},(4+4+15)*ProgressiveTextTickRate, ProgressiveTextTickRate);
	GenerateText("=30 points",{81,153},(4+4+15+10)*ProgressiveTextTickRate, ProgressiveTextTickRate);
	GenerateText("=20 points",{81,169},(4+4+15+20)*ProgressiveTextTickRate, ProgressiveTextTickRate);
	GenerateText("=10 points",{81,185},(4+4+15+30)*ProgressiveTextTickRate, ProgressiveTextTickRate);

	WipeScreenToContent(&g_wipe_screen);
	g_wipe_screen.queued_tick = g_tick + (4+4+15+40)*ProgressiveTextTickRate + 120;
	
	QueueUpdate(kUpdateID_InsertCoin, g_tick + (4+4+15+40)*ProgressiveTextTickRate + 120 + 22);
}


void ChoosingPlayerInit(Game_t *game){
	ClearUIContent();
	if (game->credits > 1){
		GenerateText("push",{98,91});
		GenerateText("1 or 2players button",{33,123});
	} else {
		GenerateText("push",{98,91});
		GenerateText("only 1 player button",{33,123});
	}
}

void AboutToPlayInit(Game_t *game){
	ClearUIContent();
	GenerateText("play player<1>",{59,120});
	GenerateNumber(game->player[0].score,4, kScore1NumberPosition, 0, true);
	EraseText(kScore2NumberPosition, 4);
	WipeScreenToContent(&g_wipe_screen);
	g_wipe_screen.queued_tick = g_tick + 120;
	
	QueueUpdate(kUpdateID_Playing, g_tick + 120 + 22);
}

void InsertCoinInit(){
	GenerateText("insert  coin",{65,113});

	GenerateText("<1 or 2 players>",{49,143}, 0, ProgressiveTextTickRate);
	GenerateText("*1 player  1 coin",{49,169}, 16*ProgressiveTextTickRate, ProgressiveTextTickRate);
	GenerateText("*2 players 2 coins",{49,195}, (16+17)*ProgressiveTextTickRate, ProgressiveTextTickRate);
}

void PlayingInit(Game_t *game){
	GenerateNumber(game->player[game->player_playing].score,4, kScore1NumberPosition);
	if(game->player[game->player_playing].lives == 0) AlienArmy_Init(&game->player[game->player_playing].alien_army);
	Player_Reset(&game->player[game->player_playing]);
}

void PlayerLogic(Game_t *game) {
	AlienArmyFlag alien_army_flag = AlienArmy_Control(&game->player[game->player_playing].alien_army);
	if (game->input.insert_credit) MatarAlien(&game->player[game->player_playing].alien_army); //TODO Quitar
	if (game->input.action_button) MatarPlayer(&game->player[game->player_playing]); //TODO Quitar
	// if (alien_army_flag == kAlienArmyFlag_invaded) Instant_Dead(&game->player[player]);
	if (alien_army_flag == kAlienArmyFlag_exterminated) PlayingInit(game);
	if (game->input.left_button) {
		Move(&game->player[game->player_playing], {-2, 0});
	} else if (game->input.right_button) {
		Move(&game->player[game->player_playing], {+2, 0});
	}
}

/** @brief Actualiza los datos del juego y pinta los necesarios */
void GameUpdate(Game_t *game) {

	g_tick++;

	// Player Inputs
	InputUpdate(&game->input);
	if (game->input.insert_credit) {
		game->credits++;
		GenerateNumber(game->credits,2, kCreditsNumberPosition);
	}

	// Text
	if (g_tick == g_wipe_screen.queued_tick) g_wipe_screen.current_step = 0;
	AdvanceWipeScreen(&g_wipe_screen);

	// Queued Updates
	for (int i = 0; i < Update_kQueueCapacity; i++){
		if (Update_queue[i].active && Update_queue[i].queued_tick == g_tick){
			Update_queue[i].active = false;
			switch(Update_queue[i].id){
				case kUpdateID_InsertCoin : 
					g_wipe_screen.current_step = -1;
					ClearUIContent();
					InsertCoinInit();
					break;
				case kUpdateID_ChoosingPlayer : 
					g_wipe_screen.current_step = -1;
					ClearUIContent();
					ChoosingPlayerInit(game);
					break;
				case kUpdateID_AboutToPlay : 
					g_wipe_screen.current_step = -1;
					ClearUIContent();
					game->game_state = kGameState_AboutToPlay;
					AboutToPlayInit(game);
					break;
				case kUpdateID_Playing :
					g_wipe_screen.current_step = -1;
					ClearUIContent();
					game->game_state = kGameState_Playing;
					PlayingInit(game);
					break;
			}
		}
	}

	switch (game->game_state) {
		case kGameState_Previewing: {
			if (game->credits > 0) {
				game->game_state = kGameState_ChoosingPlayers;
				ClearQueuedActions();
				ChoosingPlayerInit(game);
			}
			break;
		}
		case kGameState_ChoosingPlayers: {
			if (game->input.insert_credit && game->credits == 2) {
				WipeScreenToContent(&g_wipe_screen);
				g_wipe_screen.queued_tick = g_tick + 1;
				QueueUpdate(kUpdateID_ChoosingPlayer, g_tick + 22);
			}
			if (game->input.one_player_button && game->credits >= 1) { // TODO por ahora así, luego cambiar
				game->multiplayer = false;
				WipeScreenToFull(&g_wipe_screen);
				g_wipe_screen.queued_tick = g_tick + 1;
				QueueUpdate(kUpdateID_AboutToPlay, g_tick + 22);
			}
			if (game->input.two_player_button && game->credits >= 2) { // TODO por ahora así, luego cambiar
				game->multiplayer = true;
				WipeScreenToFull(&g_wipe_screen);
				g_wipe_screen.queued_tick = g_tick + 1;
				QueueUpdate(kUpdateID_AboutToPlay, g_tick + 22);
			}
			break;
		}
		case kGameState_AboutToPlay: break;
		case kGameState_Playing: {
			if (game->multiplayer) {
				if(game->player[game->player_playing].active) {
					PlayerLogic(game);
				} else {
					game->player_playing = !game->player_playing;
					PlayingInit(game);
				}
			} else {
				if(game->player[game->player_playing].active) {
					PlayerLogic(game);
				} else {
					PlayingInit(game);
				}
			}
			break;
		}
	}
}

#endif // UPDATE_CC