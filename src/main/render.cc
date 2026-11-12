#ifndef RENDER_CC
#define RENDER_CC 1

#include <esat/draw.h>

#include "../core/types.cc"
#include "../core/cirno.cc"
#include "../core/mask.cc"
#include "../entities/body.cc"
#include "../core/draw.cc"
#include "../entities/aliens_army.cc"
#include "../tools/ui-element.cc"

// FUNCIONES DE RENDERIZADO

void AliensRender(AlienArmy *alien_army) {
    for (int i = 0; i < 55; i++) {
        if (alien_army->aliens[i].active) Draw(&alien_army->aliens[i].body, false);
    }
    if (alien_army->alien_explosion_tick > 0) Draw(&alien_army->aliens[alien_army->alien_died].body, false);
}

void PreviewRender() {

}

void GameRender(Game *game) {

    for (int i = 0; i < Character_kRegistryCapacity; i++){
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
            AliensRender(&game->alien_army);
            break;
        }
    }
}

#endif // RENDER_CC