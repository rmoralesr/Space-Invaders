#ifndef GAME_MASK_CC
#define GAME_MASK_CC 1

#define MASK_WORD_COUNT 100


#include <stdio.h>
#include <sprite.h>
#include <assert.h>

#include <vector2.cc>


namespace mask {

  struct Type {
    Vector2_t size = {0,0};
    unsigned char matrix[MASK_WORD_COUNT] = {0}; // Uso unsigned char porque no necesitamos el signo y el size de char NO varia entre compiladores
  };

  unsigned char ReadWord(Type *mask, unsigned int x, unsigned int y) {
    assert(x < mask.size.x && y < mask.size.y, "Attempted to read word outside of bounds");
    return mask.matrix[x + y * size];
  }

  void WriteWord(Type *mask, unsigned int x, unsigned int y, unsigned char word) {
    assert(x < mask.size.x && y < mask.size.y, "Attempted to read word outside of bounds");
    mask.matrix[x + y * size] = word;
  }

  unsigned char ReadBitFromWord(unsigned char word, unsigned char i) {
    return (word >> i) & 0b00000001;
  }

  void WriteBitToWord(unsigned char *word, unsigned char i, unsigned char v) {

  }

  unsigned char ReadBit(Type *mask, unsigned int x, unsigned int y) {
    return 0;
  }

  void WriteBit(Type *mask, unsigned int x, unsigned int y, unsigned int v) {
    unsigned char word = ReadWord(mask, x/8, y/8);

    WriteBitToWord(&word, );
  }


}

#endif //GAME_MASK_CC