#ifndef PLAYER_CC
#define PLAYER_CC 1

#include "../core/cirno.cc"
#include "../core/mask.cc"
#include "../core/draw.cc"
#include "../core/types.cc"
#include "../tools/asset-loader.cc"
#include "../tools/ui-element.cc"
#include "body.cc"
#include "bullet.cc"
#include "UFO.cc"

// Prototipaje

void Player_Construct(Player_t *self);
void Player_Reset(Player_t *self);

void Dead(Player_t *self, int damage);
void Dead_Animation(Player_t *self);
void Destruct(Player_t* self);

void MoveTo(Player_t* self, Vector2_t pos);
void Move(Player_t* self, Vector2_t delta);

void Tick(Player_t* self); // Actualiza toda la logica. Se mueve hacia adelante y/o intenta colisionar (por ver), en este caso.

void Draw(Player_t* self);

// Definiciones

void Player_Assets(Player_t *self_assets) {
  self_assets->body = {
    true,
    {16, 216}, 
    g_assets.PlayerMask
  };
}

void Player_Bunker_Assets(Player_t *self_assets) {
  for(int i = 0; i < kBunkerLimit; i++) {
    self_assets->bunker[i].body = {
      true,
      {32 + (g_assets.ShieldMask.size.x*2*i), 192}, 
      g_assets.ShieldMask
    };
  }
}

void Player_Construct(Player_t *self) {
    self->active = false;
    self->pause = true; //TODO
    self->dead = false;
    self->dying = false;
    self->dying_tick = 0;
    
    self->score = 0;
    self->lives = 0;

    Player_Assets(self);
}

void Player_Reset(Player_t *self) {
  if (!self->active) {
    self->active = true;
    self->dead = false;
    self->dying = false;
    self->dying_tick = 0;

    if (self->lives == 0) {
      self->score = 0;
      self->lives = 3;
      Player_Bunker_Assets(self);
    }
    GenerateLives(self->lives);

    Player_Assets(self);
  }
}

void Dead(Player_t *self, int damage) {
  if (self->active) {
    self->active = false;
    self->dying_tick = 60;
    self->lives -= damage;

    GenerateLives(self->lives);
    g_game.game_state = kGameState_Death;

    if(self->lives <= 0) {
      self->lives = 0;
      self->dead = true;
    }

    if(g_game.multiplayer && !g_game.player[!g_game.player_playing].dead) { // Si es multiplayer comprueba que el otro player aun siga vivo
      // WipeScreenToContent(&g_wipe_screen);
			// g_wipe_screen.queued_tick = g_tick + 60;
      QueueUpdate(kUpdateID_GameOver, g_tick + 60 + 22);
    } else if(!self->dead) {
      QueueUpdate(kUpdateID_Playing, g_tick + 60);
    } else {
      QueueUpdate(kUpdateID_GameOver, g_tick + 60);
    }

  }
}

// 2s de animación alternando frame a frame los dos sprites. 
void Dead_Animation(Player_t *self) {
  self->body.mask = g_assets.PlayerExplosion[self->dying];
  self->dying_tick--;
  if(self->dying_tick%4 == 0) self->dying = !self->dying;
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

// Obsoleta
void Bullet_Move() {
  if (Bullet_registry[kBulletType_Player].active) {
    Tick(&Bullet_registry[kBulletType_Player]);
  }
}

void Shoot(Player_t *self) {
  if (!Bullet_registry[kBulletType_Player].active) {
    Bullet_registry[kBulletType_Player] = *Bullet_Construct(kBulletType_Player);
    Bullet_registry[kBulletType_Player].body.mask = g_assets.PlayerBullet;
    MoveTo(&Bullet_registry[kBulletType_Player], {self->body.position.x + self->body.mask.size.x/2 - g_assets.PlayerBullet.size.x/2, self->body.position.y});
  
    if (!g_UFO.active) g_UFO.dir = !g_UFO.dir;
  }
}

void Draw_Bunkers(Player_t *self_bunkers) {
  for (int i = 0; i < kBunkerLimit; i++) {
    Draw(&self_bunkers->bunker[i].body);
  }
}

void Draw(Player_t *self) {
  Draw_Bunkers(self);
  if(!self->pause) Draw(&self->body); //TODO
}

#endif // PLAYER_CC