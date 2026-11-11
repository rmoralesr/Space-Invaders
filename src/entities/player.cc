#ifndef PLAYER_CC
#define PLAYER_CC 1

#include "../core/cirno.cc"
#include "../core/mask.cc"
#include "../core/draw.cc"
#include "body.cc"

struct Player_t {
  bool active = false;
  bool dying = false;

  Body_t body;
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

void Draw(Player_t* self, bool erase = false);

// Definiciones

Player_t* Player_Construct() {
//   int n = -1;
//   for (int i = 0; i < Player_kRegistryCapacity; i++) {
//     if (!Player_registry[i].active) {
//       n = i;
//       break;
//     }
//   }

  Player_t *self = nullptr;

  if (!Player_registry.active) { 
    Player_registry = {};
    self = &Player_registry;
    self->active = true;

    self->body = {
        true,
        {500, 500}, {}
    };

    FromFile(&self->body.mask, "../assets/test.png");

    // Player_registry_active_count++;
  }

  return self;
}

void Destruct(Player_t* self) {
  self->active = false;

//   Player_registry_active_count--;
}


void MoveTo(Player_t* self, Vector2_t pos) {
  MoveTo(&self->body, pos);
}

void Move(Player_t* self, Vector2_t delta) {
  Move(&self->body, delta);
}

void Draw(Player_t* self, bool erase) {
  Draw(&self->body, erase);
}

#endif // PLAYER_CC