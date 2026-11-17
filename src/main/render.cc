#include "render.hh"

#include "../core/game.hh"
#include "../core/cirno.hh"
#include "../core/mask.hh"
#include "../core/draw.hh"
#include "../entities/body.hh"
#include "../entities/aliens_army.hh"
#include "../entities/UFO.hh"
#include "../entities/player.hh"
#include "../tools/ui-element.hh"

// FUNCIONES DE RENDERIZADO

void BulletRender() {
    for (int i = 0; i < 4; i++) {
        if (Bullet_registry[i].active) {
            Draw(&Bullet_registry[i]);
        }
    }
}

void GameRender(Game_t *game) {

    //UI
    for (int i = 0; i < UIElement_kRegistryCapacity; i++){
        if ( !UIElement_registry[i].flashing || g_tick%8 > 3) 
            if (UIElement_registry[i].active && UIElement_registry[i].queued_tick <= g_tick) 
                Draw(&UIElement_registry[i].body);
    }
    for (int i = 0; i < AnimatedUIElement_kRegistryCapacity; i++){
        
        if ( !AnimatedUIElement_registry[i].element.flashing || g_tick%8 > 3){
            if (AnimatedUIElement_registry[i].active && AnimatedUIElement_registry[i].element.queued_tick <= g_tick){

                Body_t b = AnimatedUIElement_registry[i].element.body;

                int frame_rate = AnimatedUIElement_registry[i].frame_rate;
                int frame_count = AnimatedUIElement_registry[i].frame_count;

                if (g_tick%(frame_rate*frame_count) > frame_rate - 1) {
                    b.mask = AnimatedUIElement_registry[i].extra_masks[ (g_tick%(frame_rate*frame_count))/frame_rate - 1 ];
                } 
                Draw(&b);
            }
        }
    }

    switch (game->game_state) {
        case kGameState_Previewing: {
            break;
        }
        case kGameState_ChoosingPlayers: {
            break;
        }
        case kGameState_AboutToPlay: {
            break;
        }
        case kGameState_Demo:
        case kGameState_Playing: {
            Draw(&g_bottomline);
            AliensRender(&game->player[game->player_playing].alien_army);
            if (g_UFO.active) Draw(&g_UFO.body);
            Draw(&game->player[game->player_playing]);
            BulletRender();
            break;
        }
        case kGameState_DemoDeath:
        case kGameState_Death:
            Draw(&g_bottomline);
            AliensRender(&game->player[game->player_playing].alien_army);
            if (g_UFO.active) Draw(&g_UFO.body);
            Draw(&game->player[game->player_playing]);
        break;
        case kGameState_GameOver:
            Draw(&g_bottomline);
            AliensRender(&game->player[game->player_playing].alien_army);
            if (g_UFO.active) Draw(&g_UFO.body);
            Draw_Bunkers(&game->player[game->player_playing]);
        break;
    }

    if (g_wipe_screen.current_step != -1){
        unsigned char RGBA[4] = {0,0,0,255};
        DrawRectangle(
            g_wipe_screen.position, 
            {g_wipe_screen.current_step*g_wipe_screen.step_size,g_wipe_screen.size.y},
            RGBA
        );
    }

}