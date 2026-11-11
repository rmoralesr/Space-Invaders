#ifndef RENDER_CC
#define RENDER_CC 1

#include <esat/draw.h>

#include "../core/types.cc"
#include "../core/cirno.cc"
#include "../core/mask.cc"
#include "../entities/body.cc"
#include "../core/draw.cc"
#include "../tools/ui-element.cc"

// FUNCIONES DE RENDERIZADO

void PreviewRender() {
    /*Body_t buddy = {
        true,
        {0,0}, {}
    };
    FromFile(&buddy.mask, "../../assets/test.png");
    Draw(&buddy);*/
}

void GameRender(const Game *game) {

    for (int i = 0; i < Character_kRegistryCapacity; i++){
        if ( !Character_registry[i].flashing || g_tick%4 > 1) 
            Draw(&Character_registry[i].body);
    }

    switch (game->game_state) {
        case kGameState_Previewing: {
            PreviewRender();
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