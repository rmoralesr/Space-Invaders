#ifndef GAME_VECTOR2_CC
#define GAME_VECTOR2_CC 1

#include <math.h>
#include <stdio.h>

namespace vector2 {

  struct Type {
    int x,y;
  };

  const Type kZero = {0,0};

  Type Sum(Type v1, Type v2) {
    return { 
      v1.x + v2.x,
      v1.y + v2.y
    };
  }

  Type Sub(Type v1, Type v2) {
    return { 
      v1.x - v2.x,
      v1.y - v2.y
    };
  }

  Type Mult(Type v1, Type v2) {
    return { 
      v1.x * v2.x,
      v1.y * v2.y
    };
  }

  Type Div(Type v1, Type v2) {
    return { 
      v1.x / v2.x,
      v1.y / v2.y
    };
  }

  Type Mult(Type v, int n) {
    return { 
      v.x * n,
      v.y * n
    };
  }

  Type Div(Type v, int n) {
    return { 
      v.x / n,
      v.y / n
    };
  }

  float Magnitude(Type v) {
    return sqrt(v.x*v.x + v.y*v.y);
  }

  void Print(Type v) {
    printf("{ %d, %d }\n", v.x, v.y);
  }

}

#endif // GAME_VECTOR2_CC