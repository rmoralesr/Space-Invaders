#ifndef CIRNO_PERFECT_MATH_HH
#define CIRNO_PERFECT_MATH_HH 1

int clamp(int x, int lower, int upper);
int sign(int x);
int pow(int base, int exp);
int log(unsigned int n, unsigned int base = 2);

struct Vector2_t {
  int x,y;
};

const Vector2_t Vector2_kZero = {0,0};

bool isZero(Vector2_t v);

Vector2_t Sum(Vector2_t v1, Vector2_t v2);
Vector2_t Sub(Vector2_t v1, Vector2_t v2);

Vector2_t Mult(Vector2_t v1, Vector2_t v2);
Vector2_t Div(Vector2_t v1, Vector2_t v2);

Vector2_t Mult(Vector2_t v, int n);
Vector2_t Div(Vector2_t v, int n);

Vector2_t Neg(Vector2_t v);

void Print(Vector2_t v);

#endif // CIRNO_PERFECT_MATH_HH