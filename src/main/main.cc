#include <esat/window.h>
#include <esat/draw.h>
#include <esat/input.h>
#include <esat/sprite.h>
#include <esat/time.h>

// LOS INCLUDES DEBEN ESTAR EN ORDEN, LOS QUE NECESITAN OTROS QUE SE PONGAN DEBAJO DE ELLOS
#include "../core/cirno.hh"
#include "../core/mask.hh"
#include "../entities/body.hh"
#include "../tools/ui-element.hh"
#include "../core/game.hh"
#include "update.hh"
#include "render.hh"
#include "../tools/sound.cc"

/** @brief Controla el maximo de frames por segundo */
void FrameWait(double last_time);


/** @brief Inicializa las variables necesarias para el juego */
void GameInit(Game_t *game) {
    game->credits = 0;
    game->player_playing = 0;

    LoadAssets();
    GenerateText("score<1> hi-score score<2>", {10,9});
    GenerateNumber(game->player[0].score,4, kScore1NumberPosition);
    GenerateNumber(game->player[1].score,4, kScore2NumberPosition);
    GenerateNumber(game->hiscore,4,kHiScoreNumberPosition);
    GenerateText("credit", {140,240});
    GenerateNumber(game->credits,2, kCreditsNumberPosition);

    for (int i = 0; i < 2; i++){
        Player_Construct(&game->player[i]);
    }

    game->game_state = kGameState_InsertCoin;
    PreviewingInit(game);
}


int esat::main(int argc, char **argv) {
    esat::WindowInit(kWindowSize.x*kPixelScale, kWindowSize.y*kPixelScale);
    WindowSetMouseVisibility(true);

    GameInit(&g_game);
    g_Soloud.init();
    g_AlienExplosion.load("../../assets/sounds/alienkilled.ogg");
    g_PlayerExplosion.load("../../assets/sounds/explosion.ogg");
    g_shoot.load("../../assets/sounds/shoot.ogg");
    g_UFOflying.load("../../assets/sounds/ufo_highpitch.ogg");
    g_UFOdead.load("../../assets/sounds/ufo_lowpitch.ogg");
    g_ExtraLife.load("../../assets/sounds/extendedPlay.ogg");
    g_AlienSounds[0].load("../../assets/sounds/fastinvader1.ogg");
    g_AlienSounds[1].load("../../assets/sounds/fastinvader2.ogg");
    g_AlienSounds[2].load("../../assets/sounds/fastinvader3.ogg");
    g_AlienSounds[3].load("../../assets/sounds/fastinvader4.ogg");
    


    while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
        double last_time = esat::Time();

        GameUpdate(&g_game);

        esat::DrawBegin();
        esat::DrawClear(0,0,0);
        esat::DrawSetStrokeColor(0,0,0,0);

        GameRender(&g_game);

        esat::DrawEnd();
        esat::WindowFrame();

        FrameWait(last_time);
    }

    esat::SpriteRelease(g_assets.ColorMask);
    g_Soloud.deinit();
    esat::WindowDestroy();
    return 0;
}


void FrameWait(double last_time) {
    double current_time;
    do{
        current_time = esat::Time();
    } while(current_time - last_time <= 1000.0 / kTargetFps);
}