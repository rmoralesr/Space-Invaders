#include "menus.hh"
#include "..\core\game.hh"

bool g_alt_title_anim = false;

void LoadPreviewingMenu(){
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
    GenerateUIElement({false, {60,136}, g_assets.UFOMasks[0]},anim_tick);
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
    QueueUpdate(kUpdateID_Demo, anim_tick);
}

void LoadInsertCoinMenu(){
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
            {false, {0,40}, g_assets.AlienMasks[2][0]},
            anim_tick, anim_tick + 119,
            {1, 0},
            anim1, 2
        );
        anim_tick += 119;

        GenerateUIElement({false, {119, 40}, g_assets.AlienMasks[2][1]}, anim_tick);
        anim_tick += 6;

        Mask_t anim2[kMaxAnimationExtraMasks] = {0};
        for (int i = 0; i < 3; i++) anim2[i] = g_assets.AlienBulletsMasks[kBulletType_Squiggly-1][1+i];
        GenerateAnimatedUIElement(
            {false, {123,42}, g_assets.AlienBulletsMasks[kBulletType_Squiggly-1][0]},
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
}

void LoadChoosingPlayerOneMenu(){
	GenerateText("push",{98,91});
	GenerateText("only 1 player button",{33,123});
}
void LoadChoosingPlayerTwoMenu(){
	GenerateText("push",{98,91});
	GenerateText("1 or 2players button",{33,123});
}

void LoadAboutToPlayMenu(Game_t *game){
	if (!game->player_playing){
		GenerateText("play player<1>",{59,120});
		GenerateNumber(game->player[0].score,4, kScore1NumberPosition, 0, true);
	} else {
		GenerateText("play player<2>",{59,120});
		GenerateNumber(game->player[1].score,4, kScore2NumberPosition, 0, true);
	}
	if (!game->multiplayer) EraseText(kScore2NumberPosition, 4);

	int lives = game->player[game->player_playing].lives;
	if (lives == 0) lives = 3;
	GenerateLives(lives);

	WipeScreenToContent(&g_wipe_screen);
	g_wipe_screen.queued_tick = g_tick + 120;
	
	QueueUpdate(kUpdateID_Playing, g_tick + 120 + 22);
}

void LoadGameOverMenu(Game_t *game){
    if (game->game_state == kGameState_DemoDeath){

        WipeScreenToContent(&g_wipe_screen);
        g_wipe_screen.queued_tick = g_tick;
        QueueUpdate(kUpdateID_InsertCoin, g_tick + 22);

    } else {
        if (game->multiplayer && game->player[game->player_playing].dead) {
            GenerateText("game over  player< >",{33,224},g_tick, ProgressiveTextTickRate);
            GenerateText(game->player_playing ? "2" : "1",{177,224},g_tick + 140, ProgressiveTextTickRate);
            
            if(!game->player[!game->player_playing].dead){
                WipeScreenToContent(&g_wipe_screen);
                g_wipe_screen.queued_tick = g_tick + 180;
                QueueUpdate(kUpdateID_Afterdeath, g_tick + 180 + 22);
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
            WipeScreenToContent(&g_wipe_screen);
            g_wipe_screen.queued_tick = g_tick + 40;
            QueueUpdate(kUpdateID_Afterdeath, g_tick + 80);
        }
    }
}