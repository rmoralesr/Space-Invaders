#include "bullet.hh"

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
  
  if (self->type == kBulletType_Player) {
    self->body.mask = g_assets.PlayerBulletExplosion;
    self->body.position = Sum(self->body.position, {-4, 3}); // El sprite de la explosion del jugador es par

    self->step = 9;// Countdown para destruirse
  } else {
    self->body.mask = g_assets.AlienBulletExplosionMask;
    self->body.position = Sum(self->body.position, {-1, 0}); // Pequeño offset para que la bala penetre
    
    self->step = 3;
  }
}

void Tick(Bullet_t *self, bool quick) {
  if (!self->dying) {
    
    if (self->type == kBulletType_Player) {
      Move(self, {0, -4});
    } else {
      Move(self, {0, (quick ? 5 : 4)});
    }
    self->step++;

    if (self->type != kBulletType_Player) { // Cambia cada 3 frames
      self->body.mask = g_assets.AlienBulletsMasks[self->type-1][self->step % 4];
    }

    if (self->body.position.y != clamp(self->body.position.y, 32, 240)) { // TODO modificar valores para que sean accurate TIENEN QUE SER MULTIPLO DE 4
      self->body.position.y = clamp(self->body.position.y, 32, 240);
      Dead(self);
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