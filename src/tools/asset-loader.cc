#ifndef ASSET_LOADER_CC
#define ASSET_LOADER_CC 1

#include "../core/cirno.cc"
#include "../core/mask.cc"


constexpr unsigned int kNumCharacters = 36; // Numero de caracteres en la fuente

constexpr unsigned int kCharacterWidth = 5; // Leer del sprite
constexpr unsigned int kCharacterHeight = 8; // Leer del sprite


struct Assets {
    Mask_t CharacterMasks[kNumCharacters] = {};
    Mask_t AlienMasks[3][2] = {}; // 3 tipos de alien y 2 sprites de cada uno
};

Assets g_assets;

void LoadAssets() {

}


#endif // ASSET_LOADER_CC