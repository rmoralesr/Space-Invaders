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
const int kCharacterWidth = 5;

// ---------------------------------------------------------------------------------------------------------------------
// TYPES ---------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

struct Game {
    // TODO
};

struct Assets {
    // Sprite TextSprites[42]; Todos los posibles caracteres del juego, sacar un spritesheet de ellos y asignarlos TODO
};

Game g_game = {};
Assets g_assets = {};

// ---------------------------------------------------------------------------------------------------------------------
// TEXT FUNCTIONS ------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

void GenerateCharacter(unsigned char c, vector2:Vector2_t position){
    if (c >= '0' && c <= '9') {
        c -= '0';
    } else {
        if (c >= 'a' && c <= 'z') c -= 'a';
    }
    // body = GenerateBody(TextSprites[c], position) //FUNCION SIN IMPLEMENTAR
    // Añadir body a la array de objetos "character"
    }

void GenerateNumberCharacters(int value, int digits, vector2:Vector2_t position){
    if (value > pow(10, digits)-1) value = pow(10, digits)-1;
    for (int i = 0; i<digits; i++){
        int number = value/pow(10, digits-1-i);
        GenerateCharacter('0'+number, position);
        position.x += kCharacterWidth;
        value %= pow(10,digits-1-i);
    }
    }


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