#ifndef ASSET_LOADER_CC
#define ASSET_LOADER_CC 1

#include "../core/cirno.cc"
#include "../core/mask.cc"

//  Caracteres

//Dimensiones de los caracteres

constexpr int kCaracterWidth = 5; // Medida de 
constexpr int kCaracterHeight = 7; 

//Cantidad de caracteres
constexpr int kNumCharacters = 36; // Numero de caracteres (A-Z, 0-9)
constexpr int kNumExtraCharacters = 6; // Ultimos 6 caracteres especiales

//Separaciones de los caracteres
constexpr int kCharacterSeparationX = 5; // Separacion entre sprites
constexpr int kCharacterSeparationY = 3; // Separacion entre sprites

//  Enemigos

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
constexpr int kUFOWidth = 16; // Medida sin margenes
constexpr int kUFOHeight = 7; // Medida sin margenes

//Separaciones
constexpr int kUFOSeparation = 3; // Separacion entre sprites

constexpr int kUFOMarginTop = 2; // Separacion con el margen
constexpr int kUFOMarginLeft = 1; // Separacion con el margen

//Dimensiones del escudo

constexpr int kEscudoWidth = 36; // Medida sin margenes
constexpr int kEscudoHeight = 26; // Medida sin margenes

constexpr int kEscudoMarginLeft = 3;
constexpr int kEscudoMarginTop = 3;

struct Assets {
    Mask_t CharacterMasks[kNumCharacters] = {};
    Mask_t ExtraCharacterMasks[kNumCharacters] = {};
    Mask_t AlienMasks[kNumAlienTypes]
            [kAliensPerType] = {};

    Mask_t AlienMasks[3][2] = {}; // 3 tipos de alien y 2 sprites de cada uno

    Mask_t Squid[2] = {}; //Dos sprites de Pulpo
    Mast_t Crab[2] = {}; //Dos sprites de Cangrejo
    Mast_t Octopus[2] = {}; //Dos sprites de Calamar
    Mask_t UFOMasks[2] = {}; //Dos sprites de UFO

    Mask_t EscudoMask = {};

};

Assets g_assets;

//cargar los primeros 36 caracteres 

void LoadAssets() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            if (i * 8 + j < kNumCharacters) {
                FromFile(&g_assets.CharacterMasks[i * 8 + j],
                "../assets/caracteres.png",
                {2 + (5 + kCharacterWidth) * j,
                3 + (3 + kCharacterHeight) * i},
                {kCharacterWidth, kCharacterHeight}
                );
            }
        }
    }

    //cargar los 6 caracteres adicionales 

    for (int i = 0; i < kNumExtraCharacters; i++){
        FromFile(&g_assets.ExtraCharacterMasks[i],
        "../assets/caracteres.png",
        {2 +(5 + kCharacterWidth)*i,
        3 +(3 + kCharacterHeight)*6},
        {kCharacterWidth, kCharacterHeight}
        );
    }

    //cargar 3 tipos de Aliens

   for (int i = 0; i < kNumAlienTypes; i++) {
        for (int j = 0; j < kAliensPerType; j++) {

            FromFile(
                &g_assets.AlienMasks[i][j],
                "../assets/Aliens.png",
                {2 + (kAlienWidth + kAlienSeparationX) * j,
                 2 + (kAlienHeight + kAlienSeparationY) * i},
                {kAlienWidth, kAlienHeight}
            );
        }
    }

    //cargar los 2 sprites del UFO

    for (int i = 0; i < 2; i++){
       FromFile(&g_assets.UFOMasks[i],
       "../assets/UFO.png",
       {kUFOMarginLeft (kUFOWidth + kUFOSeparation) * i,
        kUFOMarginTop},
       {kUFOWidth, kUFOHeight}
       );
    }

    //cargar el sprite del escudo

    for (int i = 0; i < 2; i++){
        FromFile(&g_assets.EscudosMasks,
        "../assets/escudos.png",
        {kEscudosMarginLeft, kEscudosMarginTop}, 
        {kEscudosWidth + kEscudosHeight}
        );
    }


}

#endif // ASSET_LOADER_CC