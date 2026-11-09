#ifndef BODY_CC
#define BODY_CC 1

#include "vector2.cc"
#include "mask.cc"

namespace body {

  struct Type {
    vector2::Type position = {0, 0},
    mask::Type mask = {0, 0}
  };

  


}

#endif //BODY_CC