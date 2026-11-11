#ifndef ALIENS_ARMY_CC
#define ALIENS_ARMY_CC 1

#include "body.cc"

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
    int alien_died = -1;
    int last_alien_moved = 0;
};

void AlienArmyInit();
void Destruct(Alien* self);

void MoveTo(Alien* self, Vector2_t pos);
void Move(Alien* self, Vector2_t delta);

void Tick(Alien* self); // Actualiza toda la logica. Se mueve hacia adelante y/o intenta colisionar (por ver), en este caso.

void Draw(Alien* self, bool erase = false);

// Definiciones

void AlienArmyInit() {

}

void Destruct(Alien* self) {
    self->active = false;
}


void MoveTo(Alien* self, Vector2_t pos) {
    MoveTo(&self->body, pos);
}

void Move(Alien* self, Vector2_t delta) {
    Move(&self->body, delta);
}

void Draw(Alien* self, bool erase) {
    Draw(&self->body, erase);
}

#endif // ALIENS_ARMY_CC