#ifndef BULLET_HH
#define BULLET_HH 1

#include "../core/cirno.hh"
#include "../core/mask.hh"
#include "../core/draw.hh"
#include "body.hh"

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
  int current_frame = 0;

  BulletType type = kBulletType_Player;

  Body_t body;
}; 

unsigned int Bullet_registry_active_count = 0;
Bullet_t Bullet_registry[4] = {0};


Bullet_t* Bullet_Construct(BulletType type);
void Destruct(Bullet_t* self);

void Tick(Bullet_t *self, bool quick = false); // Actualiza toda la logica. Se mueve hacia adelante y/o intenta colisionar (por ver), en este caso.
void Dead(Bullet_t *self);

void MoveTo(Bullet_t *self, Vector2_t pos);
void Move(Bullet_t *self, Vector2_t delta);

void Draw(Bullet_t *self);


#endif // BULLET_HH