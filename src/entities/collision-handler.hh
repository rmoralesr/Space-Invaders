#ifndef COLLISION_HANDLER_HH
#define COLLISION_HANDLER_HH 1

#include "../core/cirno.hh"
#include "body.hh"
#include "bullet.hh"
#include "player.hh"
#include "../tools/ui-element.hh"
#include "aliens_army.hh"
#include "UFO.hh"


/** @brief Devuelve el AABB overlap de los dos bodies */
Vector2_t GetOverlap(Body_t *A, Body_t *B);

bool QuickCollide(Body_t *A, Body_t *B);

/** @brief Devuelve el numero de pixeles tocados y monta out_body para que tenga la mascara de los pixeles tocados y la posicion del a colision */
unsigned int Collide(
  Body_t *A, Body_t *B, 
  Body_t *out_body = nullptr
);

void VisualCollide(Body_t *A, Body_t *B);

// Devuelve si han colisionado y ejecutan la logica por si mismos
bool Collide(Bullet_t *A, Body_t *B);
bool Collide(Bullet_t *A, Bullet_t *B);
bool Collide(Bullet_t *A, Alien_t *B);
bool Collide(Bullet_t *A, UFO_t *B);
bool Collide(Bullet_t *A, Player_t *B);
void Collide(Alien_t *A, Bunker_t *B);
void Collide(Alien_t *A, UFO_t *B);


#endif // COLLISION_HANDLER_HH