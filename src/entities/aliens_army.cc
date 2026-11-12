#ifndef ALIENS_ARMY_CC
#define ALIENS_ARMY_CC 1

#include "body.cc"
#include "../core/types.cc"
#include "../tools/asset-loader.cc"

enum AlienType {
    kAlienType_squid = 1,
    kAlienType_crab,
    kAlienType_octopus
};

struct Alien {
    Body_t body;
    bool active;
    AlienType type;
};

struct AlienArmy {
    Alien aliens[55];
    int aliens_alive;
    int alien_died;
    int alien_explosion_tick;
    int last_alien_moved;
    int direction;
    int descending;
};

void AlienArmy_Init();
void Destruct(Alien* self);

void Tick(Alien* self); // Actualiza toda la logica. Se mueve hacia adelante y/o intenta colisionar (por ver), en este caso.

// Definiciones

void G(Alien *alien) { // TODO por ahora
    alien->body.mask = g_assets.CharacterMasks[0];
}

void AlienArmy_Init(AlienArmy *alien_army) {
    alien_army->aliens_alive = 55;
    alien_army->last_alien_moved = 54;
    alien_army->alien_died = -1;
    alien_army->alien_explosion_tick = 0;
    alien_army->direction = 1;
    alien_army->descending = 0;
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 11; col++) {
            alien_army->aliens[row * 11 + col].type = kAlienType_crab; // TODO
            alien_army->aliens[row * 11 + col].active = true;
            alien_army->aliens[row * 11 + col].body.can_collide = true;
            alien_army->aliens[row * 11 + col].body.mask = {};
            alien_army->aliens[row * 11 + col].body.position.x = col * 16 + 24;
            alien_army->aliens[row * 11 + col].body.position.y = 136 - 8 - row * 16;
            G(&alien_army->aliens[row*11+col]);
        }
    }
}

void AlienArmy_Move(AlienArmy *alien_army) {
    int index = (alien_army->last_alien_moved + 1) % 55;
    bool finished = false;
    do {
        if (alien_army->descending == 1 && index == 0) {
            alien_army->descending = 2;
            alien_army->direction = alien_army->direction * -1;
        }
        if (alien_army->aliens[index].active) {
            if (alien_army->descending == 2) {
                Move(&alien_army->aliens[index].body, {0, 8});
            }
            Move(&alien_army->aliens[index].body, {alien_army->direction*2, 0});
            if (alien_army->aliens[index].body.position.x <= 8 || alien_army->aliens[index].body.position.x >= 208) {
                alien_army->descending = 1;
            }
        }
        if (alien_army->descending == 2 && index == 54) {
            alien_army->descending = 0;
        }
        alien_army->last_alien_moved++;
        if (alien_army->aliens[index].active) finished = true;
        else index = (index + 1) % 55;
    } while (!finished && alien_army->aliens_alive > 0);
}

void AlienArmy_Control(AlienArmy *alien_army) {
    if (alien_army->alien_explosion_tick <= 0) {
        AlienArmy_Move(alien_army);
    } else {
        alien_army->alien_explosion_tick--;
    }
}

void Destruct(AlienArmy* self, int index) {
    if (self->aliens[index].active) {
        self->aliens[index].active = false;
        self->aliens_alive--;
        self->alien_explosion_tick = 16;
        self->alien_died = index;
    }
}

#endif // ALIENS_ARMY_CC