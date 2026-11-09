#ifndef GAME_VECTOR2_CC
#define GAME_VECTOR2_CC 1

#include <math.h>
#include <stdio.h>

struct Vector2_t {
  int x,y;
};

namespace vector2 {

  const Vector2_t kZero = {0,0};

  Vector2_t Sum(Vector2_t v1, Vector2_t v2) {
    return { 
      v1.x + v2.x,
      v1.y + v2.y
    };
  }

  Vector2_t Sub(Vector2_t v1, Vector2_t v2) {
    return { 
      v1.x - v2.x,
      v1.y - v2.y
    };
  }

  Vector2_t Mult(Vector2_t v1, Vector2_t v2) {
    return { 
      v1.x * v2.x,
      v1.y * v2.y
    };
  }

  Vector2_t Div(Vector2_t v1, Vector2_t v2) {
    return { 
      v1.x / v2.x,
      v1.y / v2.y
    };
  }

  Vector2_t Mult(Vector2_t v, int n) {
    return { 
      v.x * n,
      v.y * n
    };
  }

  Vector2_t Div(Vector2_t v, int n) {
    return { 
      v.x / n,
      v.y / n
    };
  }

  float Magnitude(Vector2_t v) {
    return sqrt(v.x*v.x + v.y*v.y);
  }

  void Print(Vector2_t v) {
    printf("{ %d, %d }\n", v.x, v.y);
  }

}

#endif // GAME_VECTOR2_CC