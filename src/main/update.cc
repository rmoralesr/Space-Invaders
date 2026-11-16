#ifndef UPDATE_CC
#define UPDATE_CC 1

#include <esat/draw.h>
#include "../core/types.cc"
#include "../entities/body.cc"
#include "../tools/ui-element.cc"
#include "../tools/asset-loader.cc"
#include "../tools/input.cc"
#include "../tools/menus.cc"
#include "../entities/aliens_army.cc"
#include "../entities/UFO.cc"
#include "../entities/player.cc"
#include "../entities/collision-handler.cc"
#include "../core/score.cc"

// FUNCIONES DE ACTUALIZACIÓN DE VALORES DEL JUEGO

void MatarAlien(AlienArmy_t *alien_army) { // TODO quitar
	Dead(&alien_army->aliens[g_tick%55]);
}

void MatarPlayer(Player_t *player, Game_t *game) { // TODO quitar
	Dead(player, 1);
}
void ClearQueuedActions(){
	for (int i = 0; i < Update_kQueueCapacity; i++){
		Update_queue[i].active = false;
	}
	g_wipe_screen.current_step = -1;
	g_wipe_screen.queued_tick = 0;
}

void PreviewingInit(Game_t *game){
	if (game->game_state != kGameState_Previewing){
		game->game_state = kGameState_Previewing;

		g_wipe_screen.current_step = -1;
		ClearUIContent();
		LoadPreviewingMenu();
	}
}

void DemoInit(Game_t *game){
	if (game->game_state != kGameState_Demo){
		game->game_state = kGameState_Demo;

		g_wipe_screen.current_step = -1;
		ClearUIContent();

		WipeScreenToContent(&g_wipe_screen);
		g_wipe_screen.queued_tick = g_tick + 120;

		GenerateText("*demo gameplay*", {50,200});

		QueueUpdate(kUpdateID_InsertCoin, g_tick + 120 + 22);
	}
}

void InsertCoinInit(Game_t *game){
	if (game->game_state != kGameState_InsertCoin){
		game->game_state = kGameState_InsertCoin;

		g_wipe_screen.current_step = -1;
		ClearUIContent();
		LoadInsertCoinMenu();

		g_alt_title_anim = !g_alt_title_anim;
	}
}

void ChoosingPlayerInit(Game_t *game){
	game->game_state = kGameState_ChoosingPlayers;

	g_wipe_screen.current_step = -1;
	ClearUIContent();
	if (game->credits > 1){
		LoadChoosingPlayerTwoMenu();
	} else {
		LoadChoosingPlayerOneMenu();
	}
}

void AboutToPlayInit(Game_t *game){
	ClearUIContent();
	EraseText(kTaitoCopPosition, 9);

	for (int i = 0; i < 4; i++) {
		Bullet_registry[i].active = false;
	}

	if (game->multiplayer) {
		if(game->multiplayer && !game->player[!game->player_playing].dead) game->player_playing = !game->player_playing;
	} else {
		game->player_playing = 0;
	}
	LoadAboutToPlayMenu(game);
}

void PlayingInit(Game_t *game){
	GenerateBottomLine();

	GenerateNumber(game->player[0].score,4, kScore1NumberPosition);
	if(game->multiplayer) GenerateNumber(game->player[1].score,4, kScore2NumberPosition);
	
	if(game->player[game->player_playing].lives == 0) AlienArmy_Init(&game->player[game->player_playing].alien_army);
	else if(game->multiplayer) AlienArmy_SetRegeneration(&game->player[game->player_playing].alien_army);
	UFO_Init();
	Player_Reset(&game->player[game->player_playing]);

}

void GameOverInit(Game_t *game) {
	if (game->game_state != kGameState_GameOver){
		game->game_state = kGameState_GameOver;

		ClearUIContent();

		if (game->multiplayer && game->player[game->player_playing].dead) {
			GenerateText("game over  player< >",{33,224},g_tick, ProgressiveTextTickRate);
			GenerateText(game->player_playing ? "2" : "1",{177,224},g_tick + 140, ProgressiveTextTickRate);
			
			if(!game->player[!game->player_playing].dead){
				WipeScreenToContent(&g_wipe_screen);
				g_wipe_screen.queued_tick = g_tick + 180;
				QueueUpdate(kUpdateID_AboutToPlay, g_tick + 180 + 22);
			} else {
				GenerateText("game over",{81,59},g_tick + 180, ProgressiveTextTickRate);

				WipeScreenToContent(&g_wipe_screen);
				g_wipe_screen.queued_tick = g_tick + 180 + 180;

				QueueUpdate(kUpdateID_OnGameOverEnd, g_tick + 180 + 180 + 22);
			}
		} else if(game->player[game->player_playing].dead) {
			GenerateText("game over",{81,59},g_tick, ProgressiveTextTickRate);
			WipeScreenToContent(&g_wipe_screen);
			g_wipe_screen.queued_tick = g_tick + 180;
	
			QueueUpdate(kUpdateID_OnGameOverEnd, g_tick + 180 + 22);
		} else {
			QueueUpdate(kUpdateID_AboutToPlay, g_tick + 40);
		}


	}
}

void PlayerLogic(Game_t *game) {
	Player_t *current_player = &game->player[game->player_playing];

	AlienArmyFlag alien_army_flag = AlienArmy_Control(
		&current_player->alien_army,
		current_player->score, 
		current_player->body.position.x
	);
	
	// UFO
	TickUFO();
	if (g_tick == g_UFO.start_tick) GenerateUFO();

	if (esat::IsKeyDown('M')) MatarAlien(&current_player->alien_army); //TODO Quitar
	if (esat::IsKeyDown('K')) MatarPlayer(current_player, game); //TODO Quitar
	if (esat::IsKeyDown('V')) current_player->lives++; //TODO Quitar
	if (esat::IsKeyDown('U')) DestroyUFO(); //TODO Quitar

	if (game->input.two_player_button) {
		Destruct(&Bullet_registry[1+g_tick%3]); //TODO Quitar
	}

	// Jugador
	if(game->input.action_button) Shoot(current_player);

	if (game->input.left_button) {
		Move(current_player, {-1, 0});
	} else if (game->input.right_button) {
		Move(current_player, {+1, 0});
	}


	// Update Objetos
	int i, j;

	Body_t *body;
	UIElement_t *uielement;
	Bunker_t *bunker;
	Alien_t *alien;
	Bullet_t *bullet;

	// Todo Bala jugador
	Bullet_t *p_bullet = &Bullet_registry[kBulletType_Player];
	if (p_bullet->active) {
		Tick(p_bullet);
		
		Collide(p_bullet, &g_UFO);

		// Bunkers
		for (j = 0; j < kBunkerLimit; j++) {
			body = &current_player->bunker[j].body;
			Collide(p_bullet, body);
		}

		// Aliens
		for (j = 0; j < 55; j++) {
			alien = &current_player->alien_army.aliens[j];
			if(alien->active) Collide(p_bullet, alien);
		}
	}

	// Otras balas
	for (i = 0; i < 3; i++) {
		bullet = &Bullet_registry[i+1];
		if (bullet->active) {
			if (i == g_tick % 3) // Cada disparo (de alien) toma turnos para ser simulado
				Tick(bullet, current_player->alien_army.aliens_alive <= 8);

			Collide(p_bullet, bullet);

			for (j = 0; j < kBunkerLimit; j++) {
				body = &current_player->bunker[j].body;
				Collide(bullet, body);
			}

			Collide(bullet, current_player);

			if (!bullet->dying && bullet->body.position.y > 228) {
				Dead(bullet);

				if (bullet->dying) {
					VisualCollide(&bullet->body, &g_bottomline);

					for (j = 0; j < UIElement_kRegistryCapacity; j++) {
						uielement = &UIElement_registry[j];
						if (uielement->active)
							VisualCollide(&bullet->body, &uielement->body);
					}
				}
			}

		}
	}

	// Aliens con barreras
	for (i = 0; i < 11; i++) {
		alien = AlienArmy_GetLowestAlienInColumn(&current_player->alien_army, i);
		if (alien != nullptr) {
			for (j = 0; j < kBunkerLimit; j++) {
				bunker = &current_player->bunker[j];
				Collide(alien, bunker);
			}
		}
	}



	if (alien_army_flag == kAlienArmyFlag_invaded) Dead(current_player, 4);
	if (alien_army_flag == kAlienArmyFlag_exterminated && current_player->lives > 0) {
		AlienArmy_Init(&game->player[game->player_playing].alien_army);
		UFO_Init();
	}
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

	// Queued Updates
	for (int i = 0; i < Update_kQueueCapacity; i++){
		if (Update_queue[i].active && Update_queue[i].queued_tick == g_tick){
			Update_queue[i].active = false;
			switch(Update_queue[i].id){
				case kUpdateID_Previewing :
					PreviewingInit(game);
					break;
				case kUpdateID_Demo :
					DemoInit(game);
					break;
				case kUpdateID_InsertCoin :
					InsertCoinInit(game);
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
				case kUpdateID_GameOver :
					// g_wipe_screen.current_step = -1;
					// ClearUIContent();
					GameOverInit(game);
					break;
				case kUpdateID_OnGameOverEnd :
					if (game->credits > 0) {
						ChoosingPlayerInit(game);
					} else {
						PreviewingInit(game);
					}
				case kUpdateID_UpdateScore:
					UpdateScore();
					break;
			}
		}
	}

	switch (game->game_state) {
		case kGameState_Previewing: 
		case kGameState_Demo: 
		case kGameState_InsertCoin: {
			if (game->input.left_button && game->input.right_button){
				GenerateText("taito cop", kTaitoCopPosition);
			}

			if (game->credits > 0) {
				ClearQueuedActions();
				game->game_state = kGameState_ChoosingPlayers;

				WipeScreenToContent(&g_wipe_screen);
				g_wipe_screen.queued_tick = g_tick;

				QueueUpdate(kUpdateID_ChoosingPlayer, g_tick + 22);
			}
			break;
		}
		case kGameState_ChoosingPlayers: {
			if (game->input.insert_credit && game->credits == 2) {
				WipeScreenToContent(&g_wipe_screen);
				g_wipe_screen.queued_tick = g_tick;
				QueueUpdate(kUpdateID_ChoosingPlayer, g_tick + 22);
			}
			if (game->input.one_player_button && game->credits >= 1) { // TODO por ahora así, luego cambiar
				game->multiplayer = false;
				UpdateCredits(game, -1);
			} else if (game->input.two_player_button && game->credits >= 2) { // TODO por ahora así, luego cambiar
				game->multiplayer = true;
				game->player_playing = 1; // Todo modificar logica de muerte para no depende de poner el player 2 por defecto
				UpdateCredits(game, -2);
			}
			break;
		}
		case kGameState_AboutToPlay: break;
		case kGameState_Playing: {

			if(game->player[game->player_playing].active) {
				PlayerLogic(game);
			} else {
				PlayingInit(game);
			}


			break;
			case kGameState_Death:
				Dead_Animation(&game->player[game->player_playing]);
			break;
			case kGameState_GameOver:
			break;
		}
	}

	// UI Timing checks
	if (g_tick == g_wipe_screen.queued_tick) g_wipe_screen.current_step = 0;
	AdvanceWipeScreen(&g_wipe_screen);

	for (int i = 0; i < UIElement_kRegistryCapacity; i++){
		if (UIElement_registry[i].active && UIElement_registry[i].queued_tick == g_tick) 
			EraseUIElement(UIElement_registry[i].body.position, i);
    }
}

#endif // UPDATE_CC