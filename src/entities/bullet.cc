#ifndef BULLET_CC
#define BULLET_CC 1

#include "../core/cirno.cc"
#include "../core/mask.cc"
#include "../core/draw.cc"
#include "body.cc"

enum BulletType {
  kBulletType_Player
};

struct Bullet_t {
  bool active = false;
  bool dying = false;

  BulletType type = kBulletType_Player;

  Body_t body;
}; 

constexpr unsigned int Bullet_kRegistryCapacity = 20;
unsigned int Bullet_registry_active_count = 0;
Bullet_t Bullet_registry[Bullet_kRegistryCapacity] = {0};

// Prototipaje

Bullet_t* Bullet_Construct();
void Destruct(Bullet_t* self);

void MoveTo(Bullet_t* self, Vector2_t pos);
void Move(Bullet_t* self, Vector2_t delta);

void Tick(Bullet_t* self); // Actualiza toda la logica. Se mueve hacia adelante y/o intenta colisionar (por ver), en este caso.

void Draw(Bullet_t* self, bool erase = false);

// Definiciones

Bullet_t* Bullet_Construct() {
  int n = -1;
  for (int i = 0; i < Bullet_kRegistryCapacity; i++) {
    if (!Bullet_registry[i].active) {
      n = i;
      break;
    }
  }

  Bullet_t *self = nullptr;

  if (n != -1) { 
    Bullet_registry[n] = {};
    self = &Bullet_registry[n];
    self->active = true;

    Bullet_registry_active_count++;
  }

  return self;
}

void Destruct(Bullet_t* self) {
  self->active = false;

  Bullet_registry_active_count--;
}


void MoveTo(Bullet_t* self, Vector2_t pos) {
  MoveTo(&self->body, pos);
}

void Move(Bullet_t* self, Vector2_t delta) {
  Move(&self->body, delta);
}

void Draw(Bullet_t* self, bool erase) {
  Draw(&self->body, erase);
}

#endif // BULLET_CC