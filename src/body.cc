#ifndef BODY_CC
#define BODY_CC 1

#include <math.h>

#include "cirno.cc"
#include "mask.cc"


// Los tipos body no necesitan un registro puesto que todos los body tienen un padre (un struct que los contenga)

struct Body_t {
  bool can_collide = true;
  Vector2_t position = {0, 0};
  Mask_t mask = {};
};


Vector2_t GetOverlap(Body_t *A, Body_t *B) {
  // min(aMax, bMax) - max(aMin, bMin)

  return {
    min(A->position.x + A->mask.size.x, B->position.x + B->mask.size.x) 
    - max(A->position.x, B->position.x),

    min(A->position.y + A->mask.size.y, B->position.y + B->mask.size.y) 
    - max(A->position.y, B->position.y)
  };
}

/** @brief Retorna si dos bodies estan tocandose */
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

/** @brief Retorna la cantidad de pixeles que se tocan entre dos bodies 
  Admite el pointer a un body dummy al que se le escribe informacion sobre la colision
*/
unsigned int Collide(Body_t *A, Body_t *B,
  Body_t *out_body = nullptr
) {

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


#endif //BODY_CC