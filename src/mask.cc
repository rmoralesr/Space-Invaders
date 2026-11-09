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


  unsigned int GetWordIndex(Type *mask, unsigned int x, unsigned int y) {
    return x / 8 + y * mask->size.x / 8;
  }

  unsigned char GetBitIndex(Type *mask, unsigned int x, unsigned int y) {
    return x + y * mask->size.x;
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
    return ReadBitFromWord(
      mask->matrix[ GetWordIndex(mask, x,y) ],
      GetBitIndex(mask, x,y) % 8
    );
  }

  void WriteBit(Type *mask, unsigned int x, unsigned int y, unsigned int v) {
    unsigned char word = mask->matrix[x + y * 8];

    WriteBitToWord(&word, x%8);
  }


}

#endif //GAME_MASK_CC