#ifndef PLAYER_CC
#define PLAYER_CC 1

#include "../core/cirno.cc"
#include "../core/mask.cc"
#include "../core/draw.cc"
#include "../core/types.cc"
#include "../tools/asset-loader.cc"
#include "body.cc"
#include "bullet.cc"

// Prototipaje

void Player_Construct(Player_t *self);
void Player_Reset(Player_t *self);
void Destruct(Player_t* self);

void MoveTo(Player_t* self, Vector2_t pos);
void Move(Player_t* self, Vector2_t delta);

void Tick(Player_t* self); // Actualiza toda la logica. Se mueve hacia adelante y/o intenta colisionar (por ver), en este caso.

void Draw(Player_t* self);

// Definiciones

void Player_Assets(Player_t *self_assets) {
  for(int i = 0; i < kBunkerLimit; i++) {
    self_assets->bunker[i].body = {
      true,
      {32 + (g_assets.ShieldMask.size.x*2*i), 192}, 
      g_assets.ShieldMask
    };
  }

  self_assets->body = {
    true,
    {16, 224}, 
    g_assets.PlayerMasks
  };
}

void Player_Construct(Player_t *self) {
    self->active = false;
    self->dying = false;

    self->score = 0;
    self->lives = 0;

    Player_Assets(self);
}

void Player_Reset(Player_t *self) {
  if (!self->active) {
    self->active = true;

    if (self->lives == 0) {
      self->score = 0;
      self->lives = 3;
    }

    Player_Assets(self);
  }
}

void Dead(Player_t *self, int damage) {
  if (self->active) {
    self->active = false;
    self->dying = true;
    self->lives -= damage;
  }
}

void Dead_Animation(Player_t *self) {
  self->body.mask = g_assets.PlayerExplosion[0];
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
  if (
    (self->body.position.x > 16 && self->body.position.x < (208 - self->body.mask.size.x)) || 
    ((self->body.position.x == 16 && delta.x > 0) || (self->body.position.x == (208 - self->body.mask.size.x) && delta.x < 0))
  ) {
    Move(&self->body, delta);
  }
}

void Draw(Player_t *self) {
  if(self->active) {
    for (int i = 0; i < kBunkerLimit; i++) {
      Draw(&self->bunker[i].body);
    }
    Draw(&self->body);
  }
}

#endif // PLAYER_CC