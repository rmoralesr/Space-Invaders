#ifndef BODY_CC
#define BODY_CC 1

#include "body.hh"

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