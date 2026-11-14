#ifndef COLLISION_HANDLER_CC
#define COLLISION_HANDLER_CC 1

#include "../core/cirno.cc"
#include "body.cc"
#include "bullet.cc"
#include "player.cc"
#include "aliens_army.cc"

// Prototipaje

Body_t g_dummy_body; // Usado para no construir y deconstruir un body por colision
Mask_t g_dummy_mask;

/** @brief Devuelve el AABB overlap de los dos bodies */
Vector2_t GetOverlap(Body_t *A, Body_t *B);

bool QuickCollide(Body_t *A, Body_t *B);

/** @brief Devuelve el numero de pixeles tocados y monta out_body para que tenga la mascara de los pixeles tocados y la posicion del a colision */
unsigned int Collide(
  Body_t *A, Body_t *B, 
  Body_t *out_body = nullptr
);
// Devuelve si han colisionado y ejecutan la logica por si mismos
bool Collide(Bullet_t *A, Bullet_t *B);
bool Collide(Bullet_t *A, Alien_t *B);
bool Collide(Bullet_t *A);


// Definiciones

bool QuickCollide(Body_t *A, Body_t *B) {

  Vector2_t overlap = GetOverlap(A, B);

  if (overlap.x > 0 && overlap.y > 0) {

    Vector2_t corner = {
      max(A->position.x, B->position.x),
      max(A->position.y, B->position.y)
    };

    Vector2_t local_corner_A = Sub(corner, A->position);
    Vector2_t local_corner_B = Sub(corner, B->position);

    Vector2_t local_pos_A; // En release refactorizar todo esto porque será basicamente innecesario
    Vector2_t local_pos_B;

    for (int y = 0; y < overlap.y; y++) {

      local_pos_A.y = local_corner_A.y + y;
      local_pos_B.y = local_corner_B.y + y;

      for (int x = 0; x < overlap.x; x++) {

        local_pos_A.x = local_corner_A.x + x;
        local_pos_B.x = local_corner_B.x + x;

        if ( // Borrar en release
          IsValidPosition(&A->mask,
            local_pos_A.x, local_pos_A.y
          )
          && IsValidPosition(&B->mask, 
            local_pos_B.x, local_pos_B.y
          )
        ) {

          if (ReadBit(&A->mask, local_pos_A.x, local_pos_A.y) // And bitwise operator
          & ReadBit(&B->mask, local_pos_B.x, local_pos_B.y) ) {

            return true; // Retornamos directamente

          }

        } else {
          printf("La cosa que no deberia pasar ha pasado");
        }

      }
    }

    return false;

  } else 
    return false;
  
}

unsigned int Collide(Body_t *A, Body_t *B, Body_t *out_body) {

  Vector2_t overlap = GetOverlap(A, B);

  if (overlap.x > 0 && overlap.y > 0) {

    unsigned int count = 0;

    Vector2_t corner = {
      max(A->position.x, B->position.x),
      max(A->position.y, B->position.y)
    };

    if (out_body != nullptr) {
      out_body->position = corner;
      out_body->mask.size = overlap;
    }

    Vector2_t local_corner_A = Sub(corner, A->position);
    Vector2_t local_corner_B = Sub(corner, B->position);

    Vector2_t local_pos_A; // En release refactorizar todo esto porque será basicamente innecesario
    Vector2_t local_pos_B;

    for (int y = 0; y < overlap.y; y++) {

      local_pos_A.y = local_corner_A.y + y;
      local_pos_B.y = local_corner_B.y + y;

      for (int x = 0; x < overlap.x; x++) {

        local_pos_A.x = local_corner_A.x + x;
        local_pos_B.x = local_corner_B.x + x;

        if ( // Borrar en release
          IsValidPosition(&A->mask,
            local_pos_A.x, local_pos_A.y
          )
          && IsValidPosition(&B->mask, 
            local_pos_B.x, local_pos_B.y
          )
        ) {

          if (ReadBit(&A->mask, local_pos_A.x, local_pos_A.y) // And bitwise operator
          & ReadBit(&B->mask, local_pos_B.x, local_pos_B.y) ) {

            if (out_body != nullptr)
              WriteBit(&out_body->mask, x,y, 1);

            count++;

          }

        } else {
          printf("La cosa que no deberia pasar ha pasado");
        }

      }
    }

    return count;

  } else {
    return 0;
  }

}


bool Collide(Bullet_t *A, Bullet_t *B) {
  if (A->body.can_collide && B->body.can_collide) {

    unsigned int res = Collide(&A->body, &B->body, &g_dummy_body);

    if (res > 0) { // Ha habido colision

      Vector2_t delta = Sub(A->body.position, B->body.position);
      
      g_dummy_mask = A->body.mask;

      Dead(A); // <- Actualizar Sprite de A AQUI
      Sub(&A->body.mask, &B->body.mask, delta);

      Dead(B); // <- Actualizar Sprite de B AQUI
      Sub(&B->body.mask, &g_dummy_mask, Neg(delta)); 

      return true;

    }
  }

  return false;
}



Vector2_t GetOverlap(Body_t *A, Body_t *B) {
  // min(aMax, bMax) - max(aMin, bMin)

  return {
    min(A->position.x + A->mask.size.x, B->position.x + B->mask.size.x) 
    - max(A->position.x, B->position.x),

    min(A->position.y + A->mask.size.y, B->position.y + B->mask.size.y) 
    - max(A->position.y, B->position.y)
  };
}


#endif // COLLISION_HANDLER_CC