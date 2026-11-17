#include "mask.hh"

bool IsValidPosition(Mask_t *mask, unsigned int x, unsigned int y) {
  return (
    x < (unsigned int)mask->size.x
    && y < (unsigned int)mask->size.y
    && (y * mask->size.x + x) / 8 < kMask_WordCount
  );
}

unsigned int GetIndex(Mask_t *mask, unsigned int x, unsigned int y) {
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

unsigned char ReadBit(Mask_t *mask, unsigned int x, unsigned int y) {
  unsigned int i = GetIndex(mask, x,y);

  return ReadBitFromWord(
    mask->matrix[ i / 8 ],
    i % 8
  );
}

void WriteBit(Mask_t *mask, unsigned int x, unsigned int y, unsigned char v) {
  unsigned int i = GetIndex(mask, x,y);

  WriteBitToWord(
    &mask->matrix[ i / 8 ],
    i % 8,
    v
  );
}

void FromFile(Mask_t *mask, const char *path, 
  Vector2_t sub_position,
  Vector2_t sub_size
) {
  esat::SpriteHandle sprite = esat::SpriteFromFile(path);

  if (!isZero(sub_size)) {
    printf("sub ");
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

  if (mx*my > kMask_WordCount*8) {
    printf("FromFile: %s es demasiado grande (%d x %d)", path, mx, my);
    assert(false); // Abortamos porque probablemente escribiriamos en buffers ajenos
  }
  printf("%d, %d\n", mx, my);

  mask->size = {mx, my};

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

void Sub(Mask_t *A, Mask_t *B, Vector2_t offset) {
  int ox = (int)offset.x;
  int oy = (int)offset.y;

  // Rango de cordenadas B locales dentro de A
  int x0 = max(0, -ox);
  int y0 = max(0, -oy);
  int x1 = min((int)B->size.x, (int)A->size.x - ox);
  int y1 = min((int)B->size.y, (int)A->size.y - oy);

  for (int y = y0; y < y1; y++) {
    for (int x = x0; x < x1; x++) {
      if (ReadBit(B, x, y))
        WriteBit(A, ox + x, oy + y, 0);
    }
  }
}

void Print(Mask_t *mask) {
  unsigned char c = 0;

  printf("size: "); Print(mask->size);

  for (int y = 0; y < mask->size.y; y++) {
    for (int x = 0; x < mask->size.x; x++) {
      c = ReadBit(mask, x,y);
      if (c) printf("#");
      else printf(" ");
    }
    printf("\n");
  }

}