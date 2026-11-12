#ifndef RENDER_CC
#define RENDER_CC 1

#include <esat/draw.h>

#include "../core/types.cc"
#include "../core/cirno.cc"
#include "../core/mask.cc"
#include "../core/draw.cc"
#include "../entities/body.cc"
#include "../entities/aliens_army.cc"
#include "../entities/player.cc"
#include "../tools/ui-element.cc"

// FUNCIONES DE RENDERIZADO

void PreviewRender() {

}

void GameRender(Game_t *game) {

    for (int i = 0; i < UIElement_kRegistryCapacity; i++){
        if ( !UIElement_registry[i].flashing || g_tick%4 > 1) 
            if (UIElement_registry[i].active && UIElement_registry[i].queued_tick <= g_tick) 
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
        case kGameState_AboutToPlay: break;
        case kGameState_ChoosingPlayers: {
            break;
        }
        case kGameState_Playing: {
            AliensRender(&game->player[0].alien_army);
            Draw(&game->player[0]);
            break;
        }
    }
}

#endif // RENDER_CC