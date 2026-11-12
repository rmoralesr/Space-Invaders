#ifndef ASSET_LOADER_CC
#define ASSET_LOADER_CC 1

#include "../core/cirno.cc"
#include "../core/mask.cc"

//Dimensiones de los caracteres

constexpr int kNumCharacters = 36; // Numero de caracteres en la fuente
constexpr int kNumExtraCharacters = 6; // Ultimos 6 caracteres

constexpr int kCharacterWidth = 5; // separacion entre sprites
constexpr int kCharacterHeight = 7;

//Dimensiones de los enemigos

constexpr int kCharacterWidth = 12; 
constexpr int kCharacterHeight = 8; 

//Cantidad de enemigos

constexpr int kNumAlienTypes = 3;
constexpr int kAliensPerType = 2;

//Separaciones
constexpr int kAlienSeparationX = 3;
constexpr int kAlienSeparationY = 1;

//Dimensiones del UFO (jefe)

constexpr int kCharacterWidth = 16; 
constexpr int kCharacterHeight = 7;

constexpr int kUFOSeparation = 3;

constexpr int kUFOMarginTop = 2;
constexpr int kUFOMarginLeft = 1;

//Dimensiones del escudo

constexpr int kEscudoWidth = 36;
constexpr int kEscudoHeight = 26;

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

};

Assets g_assets;

//cargar los primeros 36 caracteres 

void LoadAssets() {

}


#endif // ASSET_LOADER_CC