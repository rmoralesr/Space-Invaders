#ifndef BODY_CC
#define BODY_CC 1

#include <math.h>

#include "cirno.cc"
#include "mask.cc"

namespace body {

  // Los tipos body no necesitan un registro puesto que todos los body tienen un padre (un struct que los contenga)

  struct Type {
    bool can_collide = true;
    vector2::Type position = {0, 0};
    mask::Type mask = {};
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
  
  /** @brief Retorna la cantidad de pixeles que se tocan entre dos bodies */
  unsigned int Collide(Type *A, Type *B,
    Type *out_body
  ) {

    vector2::Type overlap = GetOverlap(A, B);

    if (overlap.x > 0 && overlap.y > 0) {

      unsigned int 

      vector2::Type corner = {
        max(A->position.x, B->position.x),
        max(A->position.y, B->position.y)
      };

      
      for (int y = 0; y < overlap.y; y++) {
        for (int x = 0; x < overlap.x; x++) {

          // TODO

        }
      }


    } else {
      return false;
    }

  }

}

#endif //BODY_CC