#ifndef BODY_HH
#define BODY_HH 1

#include "../core/cirno.hh"
#include "../core/mask.hh"

struct Body_t {
  bool can_collide = true;
  
  Vector2_t position = {0, 0};
  Mask_t mask = {};
};

void MoveTo(Body_t* self, Vector2_t pos);
void Move(Body_t* self, Vector2_t delta);

#endif //BODY_HH