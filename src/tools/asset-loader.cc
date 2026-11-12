#ifndef ASSET_LOADER_CC
#define ASSET_LOADER_CC 1

#include "../core/cirno.cc"
#include "../core/mask.cc"

// Player

//Dimensiones del player
constexpr int kPlayerWidth = 18; // Medida sin margenes 
constexpr int kPlayerHeight = 12; // Medida sin margenes 

//Separaciones

constexpr int kPlayerSeparation = 5; // Separacion del margen

//  Aliens

//Dimensiones de los enemigos
constexpr int kAlienWidth = 12; // Medida sin margenes
constexpr int kAlienHeight = 8; // Medida sin margenes

//Cantidad de enemigos
constexpr int kNumAlienTypes = 3; // Numero de enemigos
constexpr int kAliensPerType = 2; // Numero de Sprites por cada enemigo

//Separaciones
constexpr int kAlienSeparationX = 3; // Separacion entre sprites
constexpr int kAlienSeparationY = 1; // Separacion entre sprites

//  UFO

//Dimensiones del UFO (jefe)
constexpr int kAlienSeparationY = 16; // Medida sin margenes 
constexpr int kAlienSeparationY = 7; // Medida sin margenes 

//Separaciones
constexpr int kAlienSeparationY = 1; // Separacion del margen

//  Shield

//Dimensiones del escudo
constexpr int kShieldWidth = 36; // Medida sin margenes
constexpr int kShieldHeight = 26; // Medida sin margenes

constexpr int kShieldMarginLeft = 3; // Separacion del margen
constexpr int kShieldMarginTop = 3; // Separacion del margen

//  Characters

//Dimensiones de los characters
constexpr int kCharactersWidth = 5; // Medida de cada characters
constexpr int kCharactersHeight = 7; // Medida de cada characters

//Cantidad de characters
constexpr int kNumCharacters = 36; // Numero de characters (A-Z, 0-9)
constexpr int kNumExtraCharacters = 6; // Ultimos 6 characters especiales

//Separaciones de los characters
constexpr int kCharacterSeparationX = 5; // Separacion entre sprites
constexpr int kCharacterSeparationY = 3; // Separacion entre sprites

struct Assets {
    Mask_t AlienMasks[kNumAlienTypes]
            [kAliensPerType] = {};

    Mask_t AlienMasks[3][2] = {}; // 3 tipos de alien y 2 sprites de cada uno

    Mask_t Squid[2] = {}; //Dos sprites de Pulpo
    Mask_t Crab[2] = {}; //Dos sprites de Cangrejo
    Mask_t Octopus[2] = {}; //Dos sprites de Calamar
    Mask_t UFOMasks[2] = {}; //Dos sprites de UFO

    Mask_t EscudoMask = {};

    Mask_t CharacterMasks[kNumCharacters] = {};
    Mask_t ExtraCharacterMasks[kNumCharacters] = {};
};

Assets g_assets;

//cargar los primeros 36 caracteres (A-Z, 0-9)

void LoadAssets() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            if (i * 8 + j < kNumCharacters) {
                FromFile(&g_assets.CharacterMasks[i * 8 + j],
                "../assets/caracteres.png",
                {2 + (5 + kCharactersWidth) * j,
                3 + (3 + kCharactersHeight) * i},
                {kCharactersWidth, kCharactersHeight}
                );
            }
        }
    }

    //cargar los 6 caracteres adicionales 

    for (int i = 0; i < kNumExtraCharacters; i++){
        FromFile(&g_assets.ExtraCharacterMasks[i],
        "../assets/caracteres.png",
        {2 +(5 + kCharactersWidth)*i,
        3 +(3 + kCharactersHeight)*6},
        {kCharactersWidth, kCharactersHeight}
        );
    }

    //cargar 3 tipos de Aliens

   for (int i = 0; i < kNumAlienTypes; i++) {
      for (int j = 0; j < kAliensPerType; j++) {

          FromFile(
              &g_assets.AlienMasks[i][j],
               "../assets/AliensVarios.png",
               {2 + (kAlienWidth + kAlienSeparationX) * j,
               2 + (kAlienHeight + kAlienSeparationY) * i},
              {kAlienWidth, kAlienHeight}
            );
        }
    }

    //cargar los 2 sprites del UFO

    
    

    //cargar el sprite del Shield

    for(int i = 0; i < 2; i++){
      FromFile(&g_assets.EscudoMask,
      "../assets/Shield.png",
      {kShieldMarginLeft, kShieldMarginTop}, 
      {kShieldWidth + kShieldHeight}
      );
    }

    //cargar los sprites de Player

    

}

#endif // ASSET_LOADER_CC