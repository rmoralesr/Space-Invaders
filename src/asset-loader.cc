#ifndef ASSET_LOADER_CC
#define ASSET_LOADER_CC 1

#include "cirno.cc"
#include "mask.cc"

namespace assets {
  constexpr unsigned int kNumCharacters = 36; // Numero de caracteres en la fuente
  
  constexpr unsigned int kCharacterWidth = 5; // Leer del sprite
  constexpr unsigned int kCharacterHeight = 8; // Leer del sprite

  Mask_t CharacterMasks[kNumCharacters] = {};

  void Load() {

  }

}

#endif // ASSET_LOADER_CC