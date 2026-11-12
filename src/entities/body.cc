#ifndef BODY_CC
#define BODY_CC 1

#include <math.h>

#include "../core/cirno.cc"
#include "../core/mask.cc"

// Los tipos body no necesitan un registro puesto que todos los body tienen un padre (un struct que los contenga)

struct Body_t {
  bool can_collide = true;
  Vector2_t position = {0, 0};
  Mask_t mask = {};
};

// Prototipaje

void MoveTo(Body_t* self, Vector2_t pos);
void Move(Body_t* self, Vector2_t delta);

// Definiciones

void MoveTo(Body_t* self, Vector2_t pos) {
  self->position = pos;
}

void Move(Body_t* self, Vector2_t delta) {
  MoveTo(self,
    Sum(
      self->position,
      delta
    )
  );
}



#endif //BODY_CC