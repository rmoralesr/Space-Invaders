#include <esat/window.h>
#include <esat/draw.h>
#include <esat/input.h>
#include <esat/sprite.h>
#include <esat/time.h>
#include <stdio.h>

// ---------------------------------------------------------------------------------------------------------------------
// CONSTANTS -----------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

const unsigned char kFps = 25; //Control de frames por segundo
const int kScreenWidth = 800;
const int kScreenHeight = 600;
const int kPossibleCharacters = 42;

// ---------------------------------------------------------------------------------------------------------------------
// TYPES ---------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

struct Game {
    // TODO
};

struct Assets {
    // Sprite text[42]; Todos los posibles caracteres del juego, sacar un spritesheet de ellos y asignarlos TODO
};

Game g_game = {};
Assets g_assets = {};

// ---------------------------------------------------------------------------------------------------------------------
// MAIN FUNCTIONS ------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

/** @brief Inicializa las variables necesarias para el juego */
void GameInit() {
    // TODO
}

/** @brief Actualiza los datos del juego */
void GameUpdate() {
    // TODO
}

/** @brief Dibuja el juego en la ventana */
void GameRender() {
    // TODO
}

/** @brief Controla el maximo de frames por segundo */
void FrameWait(double last_time) {
    double current_time;
    do{
        current_time = esat::Time();
    }while(current_time-last_time<=1000.0/kFps);
}

/** @brief Bucle principal del juego, hasta que se cierra la ventana */
void GameLoop() {
    while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
        double last_time = esat::Time();

        GameUpdate();

        esat::DrawBegin();
        esat::DrawClear(0,0,0);
        GameRender();
        esat::DrawEnd();

        FrameWait(last_time);
        esat::WindowFrame();
    }
}

int esat::main(int argc, char **argv) {
    esat::WindowInit(800,600);
    WindowSetMouseVisibility(true);

    GameInit();

    GameLoop();

    esat::WindowDestroy();
    return 0;
}