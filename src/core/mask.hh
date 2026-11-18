#ifndef MASK_HH
#define MASK_HH 1

#include <windows.h>
#include "cirno.hh"

constexpr unsigned int kMask_WordCount = 70;

struct Mask_t {
  Vector2_t size = {0,0};
  unsigned char matrix[kMask_WordCount] = {0}; // Uso unsigned char porque no necesitamos el signo y el size de char NO varia entre compiladores
};

/**  @brief Crea una mask dado el path de un sprite
  Opcionalmente se le puede dar una sub posicion y un sub tamaño,
  entonces utilizará el sub Sprite dado por esas dimensiones
*/
void FromFile(Mask_t *mask, const char *path, 
  Vector2_t sub_position = {0,0},
  Vector2_t sub_size = {0,0}
);

/** @brief Substrae mask B de mask A */
void Sub(Mask_t *A, Mask_t *B, Vector2_t offset = {0,0});

void Print(Mask_t *mask);

bool IsValidPosition(Mask_t *mask, unsigned int x, unsigned int y);

unsigned int GetIndex(Mask_t *mask, unsigned int x, unsigned int y);

unsigned char ReadBitFromWord(unsigned char word, unsigned char i);
void WriteBitToWord(unsigned char *word, unsigned char i, unsigned char v);

unsigned char ReadBit(Mask_t *mask, unsigned int x, unsigned int y);
void WriteBit(Mask_t *mask, unsigned int x, unsigned int y, unsigned char v);

#endif // MASK_CC