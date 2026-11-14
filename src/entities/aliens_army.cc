#ifndef ALIENS_ARMY_CC
#define ALIENS_ARMY_CC 1

#include "body.cc"
#include "../core/types.cc"
#include "../tools/asset-loader.cc"

void AlienArmy_Init();
void Destruct(Alien_t* self);

// Definiciones

void AsignAlienMask(Alien_t *alien) {
    alien->body.mask = g_assets.AlienMasks[alien->type][alien->sprite_frame];
}

AlienType GetAlienType(int row) {
    if (row < 2) {
        return kAlienType_octopus;
    }
    if (row < 4) {
        return kAlienType_crab;
    }
    return kAlienType_squid;
}

void Alien_Init(Alien_t *alien, int row, int col) {
    alien->type = GetAlienType(row);
    alien->active = false;
    alien->body.can_collide = true;
    alien->body.mask = {};
    alien->body.position.x = col * 16 + 24;
    alien->body.position.y = 136 - 8 - row * 16;
    alien->sprite_frame = 0;
    AsignAlienMask(alien);
}

void AlienArmy_Init(AlienArmy_t *alien_army) {
    alien_army->aliens_alive = 55;
    alien_army->last_alien_moved = 54;
    alien_army->alien_died = -1;
    alien_army->alien_explosion_tick = 0;
    alien_army->direction = 1;
    alien_army->descending = kAlienArmyState_generating;
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 11; col++) {
            Alien_Init(&alien_army->aliens[row*11+col], row, col);
        }
    }
}

void Alien_Move(Alien_t *alien, Vector2_t delta) {
    Move(&alien->body, delta);
    alien->sprite_frame = (alien->sprite_frame+1)%2;
    AsignAlienMask(alien);
}

AlienArmyFlag AlienArmy_Move(AlienArmy_t *alien_army) {
    int index = (alien_army->last_alien_moved + 1) % 55;
    bool finished = false;
    if (alien_army->aliens_alive <= 0) return kAlienArmyFlag_exterminated;
    if (alien_army->descending != kAlienArmyState_generating) {
        do {
            if (alien_army->descending == kAlienArmyState_descendingWaiting && index == 0) {
                alien_army->descending = kAlienArmyState_descendingTrue;
                alien_army->direction = alien_army->direction * -1;
            }
            if (alien_army->aliens[index].active) {
                if (alien_army->descending == kAlienArmyState_descendingTrue) {
                    Alien_Move(&alien_army->aliens[index], {0, 8});
                    if (alien_army->aliens[index].body.position.y >= 216) return kAlienArmyFlag_invaded;
                }
                Alien_Move(&alien_army->aliens[index], {alien_army->direction*2, 0});
                if (alien_army->aliens[index].body.position.x <= 8 || alien_army->aliens[index].body.position.x >= 208) {
                    alien_army->descending = kAlienArmyState_descendingWaiting;
                }
            }
            if (alien_army->descending == kAlienArmyState_descendingTrue && index == 54) {
                alien_army->descending = kAlienArmyState_descendingFalse;
            }
            alien_army->last_alien_moved++;
            if (alien_army->aliens[index].active) finished = true;
            else index = (index + 1) % 55;
        } while (!finished && alien_army->aliens_alive > 0);
    } else {
        alien_army->aliens[index].active = true;
        alien_army->last_alien_moved++;
        if (index == 54) alien_army->descending = kAlienArmyState_descendingFalse;
    }
    return kAlienArmyFlag_none;
}

AlienArmyFlag AlienArmy_Control(AlienArmy_t *alien_army) {
    if (alien_army->alien_explosion_tick <= 0) {
        return AlienArmy_Move(alien_army);
    }
    alien_army->alien_explosion_tick--;
    return kAlienArmyFlag_none;
}

void AliensRender(AlienArmy_t *alien_army) {
    for (int i = 0; i < 55; i++) {
        if (alien_army->aliens[i].active) Draw(&alien_army->aliens[i].body);
    }
    if (alien_army->alien_explosion_tick > 0) Draw(&alien_army->aliens[alien_army->alien_died].body);
}

void Destruct(AlienArmy_t* self, int index) {
    if (self->aliens[index].active) {
        self->aliens[index].active = false;
        self->aliens_alive--;
        self->alien_explosion_tick = 16;
        self->alien_died = index;
        self->aliens[index].body.mask = g_assets.AlienExplosionMask;
    }
}

#endif // ALIENS_ARMY_CC