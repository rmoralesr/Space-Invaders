#ifndef BULLET_CC
#define BULLET_CC 1

#include "../core/cirno.cc"
#include "../core/mask.cc"
#include "../core/draw.cc"
#include "body.cc"

enum BulletType {
  kBulletType_Player,

  kBulletType_Squiggly,
  kBulletType_Plunger,
  kBulletType_Rolling // Esta te apunta a ti!
};

struct Bullet_t {
  bool active = false;
  bool dying = false;

  unsigned int step = 0;

  BulletType type = kBulletType_Player;

  Body_t body;
}; 

unsigned int Bullet_registry_active_count = 0;
Bullet_t Bullet_registry[4] = {0};

// Prototipaje

Bullet_t* Bullet_Construct();
void Destruct(Bullet_t* self);

void Tick(Bullet_t *self); // Actualiza toda la logica. Se mueve hacia adelante y/o intenta colisionar (por ver), en este caso.
void Dead(Bullet_t *self);

void MoveTo(Bullet_t *self, Vector2_t pos);
void Move(Bullet_t *self, Vector2_t delta);

// Definiciones

Bullet_t* Bullet_Construct(BulletType type) { // Cambiar puntero por referencia
  Bullet_t *self = &Bullet_registry[type];

  if (!self->active) { 
    Bullet_registry[type] = {};
    self->active = true;
    self->type = type;

    Bullet_registry_active_count++;

    return self;
  } else {
    return nullptr;
  }
}

void Destruct(Bullet_t* self) {
  if (self->active) {
    self->active = false;
    Bullet_registry_active_count--;
  }
}

void Dead(Bullet_t *self) {
  // Cambiar sprite
  self->dying = true;
  self->body.can_collide = false;
  self->step = 3; // Countdown para destruirse
}

void Tick(Bullet_t *self) {
  if (!self->dying) {

    if (self->type == kBulletType_Player) {
      Move(self, {0, -1});
    } else {
      Move(self, {0, 1});
    }
    self->step++;

    // Cambiar sprite
    if (self->step % 3 == 0) {
      //self->body.mask = 
    }
  } else {
    
    self->step--;
    if (self->step <= 0) Destruct(self);

  }
}

void MoveTo(Bullet_t *self, Vector2_t pos) {
  MoveTo(&self->body, pos);
}

void Move(Bullet_t *self, Vector2_t delta) {
  Move(&self->body, delta);
}

void Draw(Bullet_t *self) {
  Draw(&self->body);
}

#endif // BULLET_CC