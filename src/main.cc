#include <esat/window.h>
#include <esat/draw.h>
#include <esat/input.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <stdio.h>

// LOS INCLUDES DEBEN ESTAR EN ORDEN, LOS QUE NECESITAN OTROS QUE SE PONGAN DEBAJO DE ELLOS
#include "cirno.cc"
#include "mask.cc"
#include "body.cc"
#include "ui.cc"
#include "update.cc"
#include "render.cc"


/** @brief Inicializa las variables necesarias para el juego */
void GameInit(Game *game) {
    // TODO
}

/** @brief Controla el maximo de frames por segundo */
void FrameWait(double last_time) {
    double current_time;
    do{
        current_time = esat::Time();
    } while(current_time - last_time <= 1000.0 / kTargetFps);
}

/** @brief Bucle principal del juego, hasta que se cierra la ventana */
void GameLoop(Game *game) {
    while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
        double last_time = esat::Time();

        GameUpdate(game);

        esat::DrawBegin();
        esat::DrawClear(0,0,0);
        GameRender(game);
        esat::DrawEnd();

        FrameWait(last_time);
        esat::WindowFrame();
    }
}

int esat::main(int argc, char **argv) {
    esat::WindowInit(kWindowSize.x, kWindowSize.y);
    WindowSetMouseVisibility(true);
    Game game = {};

    GameInit(&game);

    GameLoop(&game);

    esat::WindowDestroy();
    return 0;
}