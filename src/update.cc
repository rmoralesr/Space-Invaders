#ifndef UPDATE_CC
#define UPDATE_CC 1

#include <esat/draw.h>
#include "types.cc"
#include "body.cc"

// FUNCIONES DE ACTUALIZACIÓN DE VALORES DEL JUEGO

/** @brief Actualiza los datos del juego */
void GameUpdate(Game *game) {
    esat::DrawSetStrokeColor(0,0,0,0);
    // TODO

    Body_t buddy = {
        true,
        {20,20}, {}
    };

    FromFile(&buddy.mask, "../assets/test.png");

    DrawBody(&buddy);
}

#endif // UPDATE_CC