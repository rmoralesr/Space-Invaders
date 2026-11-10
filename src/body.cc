#ifndef BODY_CC
#define BODY_CC 1

#include <math.h>

#include "cirno.cc"
#include "mask.cc"

namespace body {

  // Los tipos body no necesitan un registro puesto que todos los body tienen un padre (un struct que los contenga)

  struct Type {
    bool can_collide = true;
    ::vector2::Type position = {0, 0};
    ::mask::Type mask = {};
  };


  vector2::Type GetOverlap(Type *A, Type *B) {
    // min(aMax, bMax) - max(aMin, bMin)

    return {
      min(A->position.x + A->mask.size.x, B->position.x + B->mask.size.x) 
      - max(A->position.x, B->position.x),

      min(A->position.y + A->mask.size.y, B->position.y + B->mask.size.y) 
      - max(A->position.y, B->position.y)
    };
  }
  
  /** @brief Retorna si dos bodies estan tocandose */
  bool QuickCollide(Type *A, Type *B) {

    vector2::Type overlap = GetOverlap(A, B);

    if (overlap.x > 0 && overlap.y > 0) {

      vector2::Type corner = {
        max(A->position.x, B->position.x),
        max(A->position.y, B->position.y)
      };

      vector2::Type local_corner_A = vector2::Sub(corner, A->position);
      vector2::Type local_corner_B = vector2::Sub(corner, B->position);

      vector2::Type local_pos_A; // En release refactorizar todo esto porque será basicamente innecesario
      vector2::Type local_pos_B;

      for (int y = 0; y < overlap.y; y++) {

        local_pos_A.y = local_corner_A.y + y;
        local_pos_B.y = local_corner_B.y + y;

        for (int x = 0; x < overlap.x; x++) {

          local_pos_A.x = local_corner_A.x + x;
          local_pos_B.x = local_corner_B.x + x;

          if ( // Borrar en release
            mask::IsValidPosition(&A->mask,
              local_pos_A.x, local_pos_A.y
            )
            && mask::IsValidPosition(&B->mask, 
              local_pos_B.x, local_pos_B.y
            )
          ) {

            if (mask::ReadBit(&A->mask, local_pos_A.x, local_pos_A.y) // And bitwise operator
            & mask::ReadBit(&B->mask, local_pos_B.x, local_pos_B.y) ) {

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
  unsigned int Collide(Type *A, Type *B,
    Type *out_body = NULL
  ) {

    vector2::Type overlap = GetOverlap(A, B);

    if (overlap.x > 0 && overlap.y > 0) {

      unsigned int count = 0;

      vector2::Type corner = {
        max(A->position.x, B->position.x),
        max(A->position.y, B->position.y)
      };

      if (out_body != NULL) {
        out_body->position = corner;
        out_body->mask.size = overlap;
      }

      vector2::Type local_corner_A = Sub(corner, A->position);
      vector2::Type local_corner_B = Sub(corner, B->position);

      vector2::Type local_pos_A; // En release refactorizar todo esto porque será basicamente innecesario
      vector2::Type local_pos_B;

      for (int y = 0; y < overlap.y; y++) {

        local_pos_A.y = local_corner_A.y + y;
        local_pos_B.y = local_corner_B.y + y;

        for (int x = 0; x < overlap.x; x++) {

          local_pos_A.x = local_corner_A.x + x;
          local_pos_B.x = local_corner_B.x + x;

          if ( // Borrar en release
            mask::IsValidPosition(&A->mask,
              local_pos_A.x, local_pos_A.y
            )
            && mask::IsValidPosition(&B->mask, 
              local_pos_B.x, local_pos_B.y
            )
          ) {

            if (mask::ReadBit(&A->mask, local_pos_A.x, local_pos_A.y) // And bitwise operator
            & mask::ReadBit(&B->mask, local_pos_B.x, local_pos_B.y) ) {

              if (out_body != NULL)
                mask::WriteBit(&out_body->mask, x,y, 1);

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

}

#endif //BODY_CC