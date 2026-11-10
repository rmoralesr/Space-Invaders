#ifndef GAME_MASK_CC
#define GAME_MASK_CC 1


#include <stdio.h>
#include <esat/sprite.h>
#include <assert.h> // Borrar en release

#include "cirno.cc"

namespace mask {

  constexpr unsigned int kWordCount = 100;

  struct Type {
    ::vector2::Type size = {0,0};
    unsigned char matrix[kWordCount] = {0}; // Uso unsigned char porque no necesitamos el signo y el size de char NO varia entre compiladores
  };

  bool IsValidPosition(Type *mask, unsigned int x, unsigned int y) {
    return (
      x < (unsigned int)mask->size.x
      && y < (unsigned int)mask->size.y
      && (y * mask->size.x + x) / 8 < kWordCount
    );
  }

  unsigned int GetPosition(Type *mask, unsigned int x, unsigned int y) {
    assert(IsValidPosition(mask, x,y)); // Borrar en release

    return y * mask->size.x + x;
  }


  unsigned char ReadBitFromWord(unsigned char word, unsigned char i) {
    return (word >> i) & 0b00000001;
  }

  void WriteBitToWord(unsigned char *word, unsigned char i, unsigned char v) {
    unsigned char selector = 1 << i;

    if (v) { // bit a 1
      *word |= selector;
    } else { // bit a 0
      *word &= (unsigned char)~selector;
    }
  }

  unsigned char ReadBit(Type *mask, unsigned int x, unsigned int y) {
    unsigned int pos = GetPosition(mask, x,y);

    return ReadBitFromWord(
      mask->matrix[ pos / 8 ],
      pos % 8
    );
  }

  void WriteBit(Type *mask, unsigned int x, unsigned int y, unsigned char v) {
    unsigned int pos = GetPosition(mask, x,y);

    WriteBitToWord(
      &mask->matrix[ pos / 8 ],
      pos % 8,
      v
    );
  }

  /**  @brief Crea una mask dado el path de un sprite
    Opcionalmente se le puede dar una sub posicion y un sub tamaño,
    entonces utilizará el sub Sprite dado por esas dimensiones
  */
  void FromFile(Type *mask, const char *path, 
    vector2::Type sub_position = {0,0},
    vector2::Type sub_size = {0,0}
  ) {
    esat::SpriteHandle sprite = esat::SpriteFromFile(path);

    if (!vector2::isZero(sub_size)) {
      esat::SpriteHandle aux = esat::SubSprite(sprite, 
        sub_position.x, sub_position.y, 
        sub_size.x, sub_size.y
      );

      esat::SpriteRelease(sprite);

      sprite = aux;
    }

    unsigned char RGBA[4] = {0};

    int mx = esat::SpriteWidth(sprite);
    int my = esat::SpriteHeight(sprite);

    for (int y = 0; y < my; y++) {
      for (int x = 0; x < mx; x++) {

        esat::SpriteGetPixel(sprite, x,y, RGBA);

        if (RGBA[3] == 255) { // El pixel es opaco
          WriteBit(mask, x,y,1);
        }

      }
    }

    esat::SpriteRelease(sprite);
  }

  /** @brief Substrae mask B de mask A */
  void Substract(Type *A, Type *B, vector2::Type offset = {0,0}) {

    // min(aMax, bMax) - max(0, bMin)
    vector2::Type overlap = {
      min(A->size.x, B->size.x + offset.x) - offset.x,
      min(A->size.y, B->size.y + offset.y) - offset.y
    };

    if (overlap.x <= 0 && overlap.y <= 0) return; // Nada que hacer

    for (int y = 0; y < overlap.y; y++) { // x,y son locales de B
      for (int x = 0; x < overlap.x; x++) {

        if (ReadBit(B, x,y))
          WriteBit(A, offset.x + x, offset.y + y, 0);

      }
    }

  }

  void Print(Type *mask) {
    unsigned char c = 0;

    printf("size: "); vector2::Print(mask->size);

    for (int y = 0; y < mask->size.y; y++) {
      for (int x = 0; x < mask->size.x; x++) {
        c = ReadBit(mask, x,y);
        if (c) printf("#");
        else printf(" ");
      }
      printf("\n");
    }

  }

}

#endif //GAME_MASK_CC