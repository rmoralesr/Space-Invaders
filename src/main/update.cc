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
#include "../entities/collision-handler.cc"
#include "../core/score.cc"

enum kUpdateID {
	kUpdateID_Previewing,
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
	long long int anim_tick = g_tick;
	if (!g_alt_title_anim) {
		GenerateText("play",{97,65}, anim_tick, ProgressiveTextTickRate);
	} else {
		GenerateText("pla",{97,65}, anim_tick, ProgressiveTextTickRate);
		anim_tick += 3*ProgressiveTextTickRate;
		GenerateUIElement({false, {120,64}, g_assets.YAnimMasks[0]}, anim_tick);
	}
	anim_tick += 4*ProgressiveTextTickRate;
	GenerateText("space  invaders",{57,89}, anim_tick, ProgressiveTextTickRate);
	anim_tick += 15*ProgressiveTextTickRate;
	GenerateText("*score advance table*",{33,121},anim_tick);
	GenerateUIElement({false, {64,136}, g_assets.UFOMasks[0]},anim_tick);
	GenerateUIElement({false, {66,152}, g_assets.AlienMasks[2][0]},anim_tick);
	GenerateUIElement({false, {66,168}, g_assets.AlienMasks[1][0]},anim_tick);
	GenerateUIElement({false, {66,184}, g_assets.AlienMasks[0][0]},anim_tick);

	GenerateText("=? mystery",{81,137},anim_tick, ProgressiveTextTickRate);
	anim_tick += 10*ProgressiveTextTickRate;
	GenerateText("=30 points",{81,153},anim_tick, ProgressiveTextTickRate);
	anim_tick += 10*ProgressiveTextTickRate;
	GenerateText("=20 points",{81,169},anim_tick, ProgressiveTextTickRate);
	anim_tick += 10*ProgressiveTextTickRate;
	GenerateText("=10 points",{81,185},anim_tick, ProgressiveTextTickRate);
	anim_tick += 10*ProgressiveTextTickRate;

	if (!g_alt_title_anim) {
		anim_tick += 60;
	} else {

		Mask_t anim1[kMaxAnimationExtraMasks] = {0};
		anim1[0] = g_assets.AlienMasks[2][1];
		GenerateAnimatedUIElement(
			{false, {kWindowSize.x,64}, g_assets.AlienMasks[2][0]},
			anim_tick, anim_tick + (kWindowSize.x-120 - 7),
			{-1, 0},
			anim1, 2
		);
		anim_tick += (kWindowSize.x-120 - 7);

		GenerateUIElement({false, {120,64}, {0}}, anim_tick);

		Mask_t anim2[kMaxAnimationExtraMasks] = {0};
		anim2[0] = g_assets.YAnimMasks[2];
		GenerateAnimatedUIElement(
			{false, {120,64}, g_assets.YAnimMasks[1]},
			anim_tick, anim_tick + (kWindowSize.x-120 - 5),
			{1, 0},
			anim2, 2
		);
		anim_tick += (kWindowSize.x-120 - 5);

		GenerateUIElement({false, {kWindowSize.x-3, 64}, g_assets.YAnimMasks[1]}, anim_tick);
		anim_tick += 60;
		GenerateUIElement({false, {kWindowSize.x-3, 64}, {0}}, anim_tick);

		Mask_t anim3[kMaxAnimationExtraMasks] = {0};
		anim3[0] = g_assets.YAnimMasks[4];
		GenerateAnimatedUIElement(
			{false, {kWindowSize.x-3,64}, g_assets.YAnimMasks[3]},
			anim_tick, anim_tick + (kWindowSize.x-3 - 120),
			{-1, 0},
			anim3, 2
		);
		anim_tick += (kWindowSize.x-3 - 120);

		GenerateUIElement({false, {120, 64}, g_assets.YAnimMasks[3]}, anim_tick);

		anim_tick += 60;
		GenerateUIElement({false, {120, 64}, {0}}, anim_tick);
		GenerateText("y", {121,65},  anim_tick);
	}

	anim_tick += 60;
	WipeScreenToContent(&g_wipe_screen);
	g_wipe_screen.queued_tick = anim_tick;

	anim_tick += 22;
	QueueUpdate(kUpdateID_InsertCoin, anim_tick);

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
	if (!g_alt_title_anim) GenerateText("insert  coin",{65,113});
	else  GenerateText("insert ccoin",{65,113});

	long long int anim_tick = g_tick;
	GenerateText("<1 or 2 players>",{49,143}, anim_tick, ProgressiveTextTickRate);
	anim_tick += 16*ProgressiveTextTickRate;
	GenerateText("*1 player  1 coin",{49,169}, anim_tick, ProgressiveTextTickRate);
	anim_tick += 17*ProgressiveTextTickRate;
	GenerateText("*2 players 2 coins",{49,195}, anim_tick, ProgressiveTextTickRate);
	anim_tick += 18*ProgressiveTextTickRate;

	if (g_alt_title_anim){
		anim_tick += 60;

		Mask_t anim1[kMaxAnimationExtraMasks] = {0};
		anim1[0] = g_assets.AlienMasks[2][1];
		GenerateAnimatedUIElement(
			{false, {0,38}, g_assets.AlienMasks[2][0]},
			anim_tick, anim_tick + 119,
			{1, 0},
			anim1, 2
		);
		anim_tick += 119;

		GenerateUIElement({false, {119, 38}, g_assets.AlienMasks[2][1]}, anim_tick);
		anim_tick += 6;

		Mask_t anim2[kMaxAnimationExtraMasks] = {0};
		for (int i = 0; i < 3; i++) anim2[i] = g_assets.AlienBulletsMasks[2][1+i];
		GenerateAnimatedUIElement(
			{false, {123,42}, g_assets.AlienBulletsMasks[2][0]},
			anim_tick, anim_tick + 113-42-8,
			{0, 1},
			anim2, 4, 1
		);
		anim_tick += 113-42-8;
		GenerateUIElement({false, {122, 109}, g_assets.AlienBulletExplosionMask}, anim_tick);

		anim_tick += 8;
		GenerateUIElement({false, {122, 109}, {0}}, anim_tick); // Borrar Explosion
		GenerateUIElement({false, {65 + 8*7, 113}, {0}}, anim_tick); // Borrar C

		anim_tick += 20;
	}

	anim_tick += 60;
	WipeScreenToContent(&g_wipe_screen);
	g_wipe_screen.queued_tick = anim_tick;

	anim_tick += 22;
	QueueUpdate(kUpdateID_Previewing, anim_tick);

	g_alt_title_anim = !g_alt_title_anim;
}

void PlayingInit(Game_t *game){
	GenerateNumber(game->player[game->player_playing].score,4, kScore1NumberPosition);
	if(game->multiplayer) GenerateNumber(game->player[game->player_playing].score,4, kScore2NumberPosition);
	if(game->player[game->player_playing].lives == 0) AlienArmy_Init(&game->player[game->player_playing].alien_army);
	Player_Reset(&game->player[game->player_playing]);
}

void PlayerLogic(Game_t *game) {
	AlienArmyFlag alien_army_flag = AlienArmy_Control(&game->player[game->player_playing].alien_army);


	if (esat::IsKeyDown('M')) MatarAlien(&game->player[game->player_playing].alien_army); //TODO Quitar
	if (game->input.action_button) MatarPlayer(&game->player[game->player_playing]); //TODO Quitar
	if (esat::IsKeyDown('V')) game->player[game->player_playing].lives++; //TODO Quitar


	// if (alien_army_flag == kAlienArmyFlag_invaded) Instant_Dead(&game->player[player]);
	if (alien_army_flag == kAlienArmyFlag_exterminated) PlayingInit(game);
	if (game->input.left_button) {
		Move(&game->player[game->player_playing], {-1, 0});
	} else if (game->input.right_button) {
		Move(&game->player[game->player_playing], {+1, 0});
	}
}

void DoGameCollisions() {
	// Balas
	Bullet_t *bulletA;
	Bullet_t *bulletB;

}

void UpdateCredits(Game_t *game, int number_credits) {
	game->credits+=number_credits;
	GenerateNumber(game->credits,2, kCreditsNumberPosition);
	WipeScreenToFull(&g_wipe_screen);
	g_wipe_screen.queued_tick = g_tick + 1;
	QueueUpdate(kUpdateID_AboutToPlay, g_tick + 22);
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
    for (int i = 0; i < AnimatedUIElement_kRegistryCapacity; i++){
		if (AnimatedUIElement_registry[i].active && AnimatedUIElement_registry[i].element.queued_tick <= g_tick){
			if (AnimatedUIElement_registry[i].end_tick == g_tick) AnimatedUIElement_registry[i].active = false;
			Move(&AnimatedUIElement_registry[i].element.body, AnimatedUIElement_registry[i].delta);
		}
	}

	if (g_tick == g_wipe_screen.queued_tick) g_wipe_screen.current_step = 0;
	AdvanceWipeScreen(&g_wipe_screen);

	// Queued Updates
	for (int i = 0; i < Update_kQueueCapacity; i++){
		if (Update_queue[i].active && Update_queue[i].queued_tick == g_tick){
			Update_queue[i].active = false;
			switch(Update_queue[i].id){
				case kUpdateID_Previewing :
					g_wipe_screen.current_step = -1;
					ClearUIContent();
					PreviewingInit();
					break;
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
				Player_Construct(&game->player[0]);
				UpdateCredits(game, -1);
			}
			if (game->input.two_player_button && game->credits >= 2) { // TODO por ahora así, luego cambiar
				game->multiplayer = true;
				for (int i = 0; i < 2; i++){
					Player_Construct(&game->player[i]);
				}
				UpdateCredits(game, -2);
			}
			break;
		}
		case kGameState_AboutToPlay: break;
		case kGameState_Playing: {
			if (game->multiplayer) {
				if(game->player[game->player_playing].active) {
					PlayerLogic(game);
				} else if(!game->player[!game->player_playing].dead) {
						game->player_playing = !game->player_playing;
						PlayingInit(game);
				} else if(game->player[game->player_playing].dead) {
					if (game->credits > 0) {
						game->game_state = kGameState_ChoosingPlayers;
						ClearQueuedActions();
						ChoosingPlayerInit(game);
					} else {
						game->game_state = kGameState_Previewing;
						PreviewingInit();
					}
				} else {
					PlayingInit(game);
				}
			} else {
				if(game->player[game->player_playing].active) {
					PlayerLogic(game);
				} else if(game->player[game->player_playing].dead) {
					if (game->credits > 0) {
						game->game_state = kGameState_ChoosingPlayers;
						ClearQueuedActions();
						ChoosingPlayerInit(game);
					} else {
						game->game_state = kGameState_Previewing;
						PreviewingInit();
					}
				} else {
					PlayingInit(game);
				}
			}

			
			// Jugador Disparo



			break;
		}
	}

	for (int i = 0; i < UIElement_kRegistryCapacity; i++){
		if (UIElement_registry[i].active && UIElement_registry[i].queued_tick == g_tick) 
			EraseUIElement(UIElement_registry[i].body.position, i);
    }

}

#endif // UPDATE_CC