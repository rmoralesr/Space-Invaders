#ifndef ASSET_LOADER_CC
#define ASSET_LOADER_CC 1

#include "../core/cirno.cc"
#include "../core/mask.cc"


constexpr int kNumCharacters = 42; // Numero de caracteres en la fuente

constexpr int kCharacterWidth = 5; // Leer del sprite
constexpr int kCharacterHeight = 7; // Leer del sprite

constexpr int kAlienWidth = 12;
constexpr int kAlienHeight = 8;

constexpr int kNumAlienTypes = 3;
constexpr int kAliensPerType = 2;

constexpr int kAlienSeparationX = 3;
constexpr int kAlienSeparationY = 1;

struct Assets {
    Mask_t CharacterMasks[kNumCharacters] = {};
    Mask_t AlienMasks[3][2] = {}; // 3 tipos de alien y 2 sprites de cada uno
};

Assets g_assets;

void LoadAssets() {
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 8; j++){
            if (i*8+j < kNumCharacters) FromFile(
                &g_assets.CharacterMasks[i*8+j],"../../assets/caracteres.png", 
                {2+(5+kCharacterWidth)*j, 3+(3+kCharacterHeight)*i},
                {kCharacterWidth,kCharacterHeight}
                );
        }
    }

    for (int i = 0; i < kNumAlienTypes; i++) {
        for (int j = 0; j < kAliensPerType; j++) {
            FromFile(
                &g_assets.AlienMasks[i][j],
                "../../assets/enemigos.png",
                {2 + (kAlienWidth + kAlienSeparationX) * j,
                 2 + (kAlienHeight + kAlienSeparationY) * i},
                {kAlienWidth, kAlienHeight}
            );
        }
    }
}


#endif // ASSET_LOADER_CC