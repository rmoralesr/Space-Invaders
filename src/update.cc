#ifndef UPDATE_CC
#define UPDATE_CC 1

#include <esat/draw.h>
#include "input.cc"
#include "types.cc"
#include "body.cc"

// FUNCIONES DE ACTUALIZACIÓN DE VALORES DEL JUEGO




/** @brief Actualiza los datos del juego */
void GameUpdate(Game *game) {
    InputUpdate(&game->input);
    // TODO

    Body_t buddy = {
        true,
        {20,20}, {}
    };

    FromFile(&buddy.mask, "../assets/test.png");

    Draw(&buddy);
}

#endif // UPDATE_CC