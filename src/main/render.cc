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

void GameRender(const Game_t *game) {

    for (int i = 0; i < UIElement_kRegistryCapacity; i++){
        if ( !UIElement_registry[i].flashing || g_tick%4 > 1) 
            Draw(&UIElement_registry[i].body);
    }
    if (g_wipe_screen.current_step != -1){
        unsigned char RGBA[4] = {0,0,0,255};
        DrawRectangle(
            g_wipe_screen.position, 
            {g_wipe_screen.current_step*g_wipe_screen.step_size,g_wipe_screen.size.y},
            RGBA
        );
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