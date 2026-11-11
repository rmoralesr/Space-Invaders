#include <esat/window.h>
#include <esat/draw.h>
#include <esat/input.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <stdio.h>

// LOS INCLUDES DEBEN ESTAR EN ORDEN, LOS QUE NECESITAN OTROS QUE SE PONGAN DEBAJO DE ELLOS
#include "core/cirno.cc"
#include "core/types.cc"
#include "core/mask.cc"
#include "entities/body.cc"
#include "tools/ui-element.cc"
#include "update.cc"
#include "core/render.cc"


/** @brief Controla el maximo de frames por segundo */
void FrameWait(double last_time);


/** @brief Inicializa las variables necesarias para el juego */
void GameInit(Game *game) {
    // TODO
    game->game_state = kGameState_Previewing;

    LoadAssets();
}


int esat::main(int argc, char **argv) {
    esat::WindowInit(kWindowSize.x, kWindowSize.y);
    WindowSetMouseVisibility(true);

    Game game = {};

    GameInit(&game);


    while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
        double last_time = esat::Time();

        esat::DrawBegin();
        esat::DrawSetStrokeColor(0,0,0,0);

        GameUpdate(&game);

        esat::DrawEnd();
        esat::WindowFrame();

        FrameWait(last_time);
    }


    esat::WindowDestroy();
    return 0;
}


void FrameWait(double last_time) {
    double current_time;
    do{
        current_time = esat::Time();
    } while(current_time - last_time <= 1000.0 / kTargetFps);
}