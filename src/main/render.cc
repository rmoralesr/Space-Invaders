#ifndef RENDER_CC
#define RENDER_CC 1

#include <esat/draw.h>

#include "../core/types.cc"
#include "../core/cirno.cc"
#include "../core/mask.cc"
#include "../entities/body.cc"
#include "../core/draw.cc"

// FUNCIONES DE RENDERIZADO

void PreviewRender() {
    Body_t buddy = {
        true,
        {20,20}, {}
    };

    FromFile(&buddy.mask, "../../assets/test.png");

    Draw(&buddy, false);
}

void GameRender(const Game *game) {
    PreviewRender();
    switch (game->game_state) {
        case kGameState_Previewing: {
            break;
        }
        case kGameState_ChoosingPlayers: {
            break;
        }
        case kGameState_Playing: {
            break;
        }
    }
}

#endif // RENDER_CC