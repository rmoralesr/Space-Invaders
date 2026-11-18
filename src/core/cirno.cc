#include <stdio.h>

#include "cirno.hh"

int clamp(int x, int lower, int upper) {
  if (x > upper) return upper;
  if (x < lower) return lower;
  return x;
}

int sign(int x) {
  return x < 0 ? -1 : 1;
}

int pow(int base, int exp) {
  int value = 1;
  for (int i = 0; i<exp; i++) value *= base;
  return value;
}

int log(unsigned int n, unsigned int base) {
  if (base < 2) return 0;

  int c = 0;
  while (n >= base) {
    n /= base;
    c++;
  }

  return c;
}

bool isZero(Vector2_t v) {
  return v.x == 0 && v.y == 0;
}

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

Vector2_t Neg(Vector2_t v) {
  return {-v.x, -v.y};
}

void Print(Vector2_t v) {
  printf("{ %d, %d }\n", v.x, v.y);
}