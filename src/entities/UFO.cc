#include "UFO.hh"
#include "..\core\game.hh"

UFO_t g_UFO;

void UFO_Init(){
    g_UFO.active = false;
    g_UFO.anim_tick_progress = 0;
    g_UFO.points_index = 0;
    g_UFO.start_tick = g_tick + 1200;
}

void GenerateUFO(){
    g_UFO.active = true;
    int x;
    if (g_UFO.dir) x = 8;
    else x = 196;
    g_UFO.anim_tick_progress = 0;
    g_UFO.body = {true, {x,42}, g_assets.UFOMasks[0]};
}
void EscapeUFO(){
    g_UFO.active = false;
    g_UFO.body.can_collide = false;
    g_UFO.start_tick = g_tick + 1200;
}

void DestroyUFO(){
    g_UFO.body.can_collide = false;
    g_UFO.body.mask = g_assets.UFOMasks[1];
    g_UFO.anim_tick_progress = 16;
    AddScore(kUFOpoints[g_UFO.points_index], false);

    GenerateNumber(kUFOpoints[g_UFO.points_index], 3, g_UFO.body.position, g_tick + 32);
    QueueUpdate(kUpdateID_UpdateScore, g_tick + 32);

    QueuedEraseText(g_UFO.body.position, 3, g_tick + 92);
}

void TickUFO(){
    if (g_tick % 2 == 0 && g_UFO.active) {
        if (g_UFO.anim_tick_progress == 0){
            if (g_UFO.dir) {
                g_UFO.body.position.x += 1;
                if (g_UFO.body.position.x > 196) EscapeUFO();
            } else {
                g_UFO.body.position.x -= 1;
                if (g_UFO.body.position.x < 8) EscapeUFO();
            }
        } else {
            g_UFO.anim_tick_progress -= 1;
            if (g_UFO.anim_tick_progress == 0) {
                EscapeUFO();
            }
        }
    }
}
void AdvanceUFOPointsIndex(){
    g_UFO.points_index++;
    if (g_UFO.points_index > 15) g_UFO.points_index = 0;
}