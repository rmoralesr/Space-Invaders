#ifndef CIRNO_PERFECT_MATH_CC
#define CIRNO_PERFECT_MATH_CC 1

#include <math.h>
#include <stdio.h>


int clamp(int x, int lower, int upper) {
  if (x > upper) return upper;
  if (x < lower) return lower;
  return x;
}

int min(int a, int b) {
  return a < b ? a : b;
}

int max(int a, int b) {
  return a > b ? a : b;
}

int sign(int x) {
  return x < 0 ? -1 : 1;
}


namespace vector2 {

  struct Type {
    int x,y;
  };

  const Type kZero = {0,0};

  bool isZero(Type v) {
    return v.x == 0 && v.y == 0;
  }

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

#endif // CIRNO_PERFECT_MATH_CC