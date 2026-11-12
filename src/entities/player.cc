#ifndef PLAYER_CC
#define PLAYER_CC 1

#include "../core/cirno.cc"
#include "../core/mask.cc"
#include "../core/draw.cc"
#include "body.cc"
#include "bullet.cc"

struct Player_t {
  bool active = false;
  bool dying = false;

  Body_t body;
  Bullet_t bullet;
};

// constexpr unsigned int Player_kRegistryCapacity = 20;
// unsigned int Player_registry_active_count = 0;
// Player_t Player_registry[Player_kRegistryCapacity] = {0};
Player_t Player_registry = {0};

// Prototipaje

Player_t* Player_Construct();
void Destruct(Player_t* self);

void MoveTo(Player_t* self, Vector2_t pos);
void Move(Player_t* self, Vector2_t delta);

void Tick(Player_t* self); // Actualiza toda la logica. Se mueve hacia adelante y/o intenta colisionar (por ver), en este caso.

void Draw(Player_t* self);

// Definiciones

void Player_Construct(Player_t *self) {
  if (!Player_registry.active) {
    self->active = true;

    self->body = {
        true,
        {16, 224}, 
        {}
    };

    FromFile(&self->body.mask, "../../assets/test.png");

    Print(&self->body.mask);
  }
}

void Destruct(Player_t *self) {
  if (self->active) {
    self->active = false;
  }
}


void MoveTo(Player_t *self, Vector2_t pos) {
  MoveTo(&self->body, pos);
}

void Move(Player_t *self, Vector2_t delta) {
  Move(&self->body, delta);
}

void Draw(Player_t *self) {
  if(self->active) {
    Draw(&self->body);
  }
}

#endif // PLAYER_CC