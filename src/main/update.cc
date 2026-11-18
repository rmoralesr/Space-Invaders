#include "update.hh"

#include "../core/game.hh"
#include "../entities/body.hh"
#include "../tools/ui-element.hh"
#include "../tools/asset-loader.hh"
#include "../tools/input.hh"
#include "../tools/menus.hh"
#include "../entities/aliens_army.hh"
#include "../entities/UFO.hh"
#include "../entities/player.hh"
#include "../entities/collision-handler.hh"
#include "../core/score.hh"

// FUNCIONES DE ACTUALIZACIÓN DE VALORES DEL JUEGO

void AboutToPlayInit(Game_t *game){
	ClearUIContent();
	EraseText(kTaitoCopPosition, 9);

	for (int i = 0; i < 4; i++) {
		Bullet_registry[i].active = false;
	}
	if(game->player[game->player_playing].alien_army.active) 
		AlienArmy_SetRegeneration(&game->player[game->player_playing].alien_army);

	LoadAboutToPlayMenu(game);
}

void PlayingInit(Game_t *game){
	GenerateBottomLine();

	GenerateNumber(game->player[0].score,4, kScore1NumberPosition);
	if(game->multiplayer) GenerateNumber(game->player[1].score,4, kScore2NumberPosition);

	UFO_Init();
	Player_Reset(&game->player[game->player_playing]);

}

void GameOverInit(Game_t *game) {
	if (game->game_state != kGameState_GameOver){
		ClearUIContent();

    	if (game->game_state == kGameState_DemoDeath) EscapeUFO();

		LoadGameOverMenu(game);
		game->game_state = kGameState_GameOver;

	}
}

void PlayerInput(Game_t *game){
	if(game->player[game->player_playing].pause_tick % 140 == 0) game->player[game->player_playing].pause = false;
	else game->player[game->player_playing].pause_tick++;

	if (!game->player[game->player_playing].pause){
		if (game->game_state == kGameState_Playing){
			if(game->input.action_button) Shoot(&game->player[game->player_playing]);

			if (game->input.left_button) {
				Move(&game->player[game->player_playing], {-1, 0});
			} else if (game->input.right_button) {
				Move(&game->player[game->player_playing], {+1, 0});
			}

		} else if (game->game_state == kGameState_Demo){
			if ( g_tick % 180 < 140){
				if ( g_tick%360 < 260) Move(&game->player[game->player_playing], {+1, 0});
				else Move(&game->player[game->player_playing], {-1, 0});
			}
			if (  g_tick % 90 == 0 ) Shoot(&game->player[game->player_playing]);
		}
	}
}

void NextRound(Game_t *game) {
	g_wipe_screen.current_step = -1;
	Player_Bunker_Reset();
	game->player[game->player_playing].pause = true;
	game->player[game->player_playing].rounds++;
	if (game->player[game->player_playing].rounds >= 9) game->player[game->player_playing].rounds = 1;
	AlienArmy_Init(&game->player[game->player_playing].alien_army);
	UFO_Init();
}

void PlayerLogic(Game_t *game){

	AlienArmyFlag alien_army_flag = AlienArmy_Control(
		&game->player[game->player_playing].alien_army,
		game->player[game->player_playing].score,
		game->player[game->player_playing].body.position.x
	);
	
	// UFO
	TickUFO();
	if (g_tick == g_UFO.start_tick) GenerateUFO();

	// Jugador
	PlayerInput(game);

	// Update Objetos
	int i, j;

	Body_t *body;
	UIElement_t *uielement;
	Bunker_t *bunker;
	Alien_t *alien;
	Bullet_t *bullet;

	Bullet_t *p_bullet = &Bullet_registry[kBulletType_Player];
	if (p_bullet->active) {
		Tick(p_bullet);
		
		Collide(p_bullet, &g_UFO);

		// Bunkers
		for (j = 0; j < kBunkerLimit; j++) {
			body = &game->player[game->player_playing].bunker[j].body;
			Collide(p_bullet, body);
		}

		// Aliens
		for (j = 0; j < 55; j++) {
			alien = &game->player[game->player_playing].alien_army.aliens[j];
			if(alien->active) Collide(p_bullet, alien);
		}
	}

	// Otras balas
	for (i = 0; i < 3; i++) {
		bullet = &Bullet_registry[i+1];
		if (bullet->active) {
			if (i == g_tick % 3) // Cada disparo (de alien) toma turnos para ser simulado
				Tick(bullet, game->player[game->player_playing].alien_army.aliens_alive <= 8);

			Collide(p_bullet, bullet);

			for (j = 0; j < kBunkerLimit; j++) {
				body = &game->player[game->player_playing].bunker[j].body;
				Collide(bullet, body);
			}

			Collide(bullet, &game->player[game->player_playing]);

			if (!bullet->dying && bullet->body.position.y > 226) {
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
		alien = AlienArmy_GetLowestAlienInColumn(&game->player[game->player_playing].alien_army, i);
		if (alien != nullptr) {
			for (j = 0; j < kBunkerLimit; j++) {
				bunker = &game->player[game->player_playing].bunker[j];
				Collide(alien, bunker);
			}
		}
	}

	if (alien_army_flag == kAlienArmyFlag_invaded) Dead(&game->player[game->player_playing], game->player[game->player_playing].lives);
	if (alien_army_flag == kAlienArmyFlag_exterminated &&
		game->player[game->player_playing].lives > 0 && !IsActionQueued(kUpdateID_NextRound)) {


		WipeScreenToContent(&g_wipe_screen);
		g_wipe_screen.queued_tick = g_tick + 1;

		QueueUpdate(kUpdateID_NextRound, g_tick+22);
	}
}

void ResetGame(Game_t *game) {
	for (int i = 0; i < 4; i++) {
		Bullet_registry[i].active = false;
	}
	for(int i = 0; i < 2; i++) {
		PlayerSetUp(&game->player[i]);
		AlienArmy_Init(&game->player[i].alien_army);
	}
	game->player_playing = 0;
}

void CheckCredits(Game_t *game) {
	if (game->credits > 0) {
		ClearQueuedActions();
		game->game_state = kGameState_ChoosingPlayers;

		WipeScreenToContent(&g_wipe_screen);
		g_wipe_screen.queued_tick = g_tick;

		QueueUpdate(kUpdateID_ChoosingPlayer, g_tick + 22);
	}
}

void SpendCredits(Game_t *game, int number_credits) {
	game->credits -= number_credits;
	GenerateNumber(game->credits,2, kCreditsNumberPosition);
	WipeScreenToFull(&g_wipe_screen);
	g_wipe_screen.queued_tick = g_tick + 1;
	QueueUpdate(kUpdateID_AboutToPlay, g_tick + 22);
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
		ResetGame(game);

		GenerateLives(3);
		game->multiplayer = false;
		game->game_state = kGameState_Demo;

		g_wipe_screen.current_step = -1;
		ClearUIContent();

		PlayingInit(game);
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
				case kUpdateID_Afterdeath :
					if(game->multiplayer && !game->player[!game->player_playing].dead) {
						game->player_playing = !game->player_playing;
						QueueUpdate(kUpdateID_AboutToPlay, g_tick + 1);
					} else {
						QueueUpdate(kUpdateID_Playing, g_tick + 100);
					}

					break;
				case kUpdateID_GameOver :
					GameOverInit(game);
					break;
				case kUpdateID_OnGameOverEnd :
					EscapeUFO();

					if (game->credits > 0) {
						ChoosingPlayerInit(game);
					} else {
						PreviewingInit(game);
					}
					break;
				case kUpdateID_UpdateScore:
					UpdateScore();
					break;
				case kUpdateID_NextRound:
					NextRound(game);
					break;
			}
		}
	}

	switch (game->game_state) {
		case kGameState_Previewing: 
		case kGameState_InsertCoin: {
			if (game->input.left_button && game->input.right_button){
				GenerateText("taito cop", kTaitoCopPosition);
			}

			CheckCredits(game);
			break;
		}
		case kGameState_ChoosingPlayers: {
			if (game->input.insert_credit && game->credits == 2) {
				WipeScreenToContent(&g_wipe_screen);
				g_wipe_screen.queued_tick = g_tick;
				QueueUpdate(kUpdateID_ChoosingPlayer, g_tick + 22);
			}
			if (game->input.one_player_button && game->credits >= 1) {
				game->multiplayer = false;
				ResetGame(game);
				SpendCredits(game, 1);
			} else if (game->input.two_player_button && game->credits >= 2) {
				game->multiplayer = true;
				ResetGame(game);
				game->player[1].pause = false;
				SpendCredits(game, 2);
			}
			break;
		}
		case kGameState_AboutToPlay: break;
		case kGameState_Demo:
			CheckCredits(game);
		case kGameState_Playing: {

			if(game->player[game->player_playing].active) {
				PlayerLogic(game);
			} else {
				PlayingInit(game);
			}


			break;
			case kGameState_DemoDeath:
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