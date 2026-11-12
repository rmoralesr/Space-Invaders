#ifndef ASSET_LOADER_CC
#define ASSET_LOADER_CC 1

#include "../core/cirno.cc"
#include "../core/mask.cc"

//Dimensiones de los caracteres

constexpr int kNumCharacters = 42; // Numero de caracteres en la fuente
constexpr int kCharacterWidth = 5; // separacion entre sprites
constexpr int kCharacterHeight = 7;

//Dimensiones de los enemigos

constexpr int kAlienWidth = 12; 
constexpr int kAlienHeight = 8; 

//Cantidad de enemigos

constexpr int kNumAlienTypes = 3;
constexpr int kAliensPerType = 2;

//Separaciones
constexpr int kAlienSeparationX = 3;
constexpr int kAlienSeparationY = 1;

//Dimensiones del UFO (jefe)

constexpr int kUFOWidth = 16; 
constexpr int kUFOHeight = 7;
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
    Mask_t AlienMasks[kNumAlienTypes][kAliensPerType] = {};
    Mask_t UFOMasks[2] = {}; //Dos sprites de UFO
};

Assets g_assets;



void LoadAssets() {

    //cargar la fuente de texto

    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 8; j++){
            if (i*8+j < kNumCharacters) FromFile(
                &g_assets.CharacterMasks[i * 8 + j],"../../assets/caracteres.png", 
                {2+(5+kCharacterWidth)*j, 3+(3+kCharacterHeight)*i},
                {kCharacterWidth,kCharacterHeight}
                );
        }
    }

    //cargar 3 tipos de Aliens

   for (int i = 0; i < kNumAlienTypes; i++) {
        for (int j = 0; j < kAliensPerType; j++) {

            FromFile(
                &g_assets.AlienMasks[i][j],
                "../../assets/aliens.png",
                {2 + (kAlienWidth + kAlienSeparationX) * j,
                 1 + (kAlienHeight + kAlienSeparationY) * i},
                {kAlienWidth, kAlienHeight}
            );
        }
    }

    //cargar los 2 sprites del UFO

    for (int i = 0; i < 2; i++){
       FromFile(&g_assets.UFOMasks[i],
       "../../assets/UFO.png",
       {kUFOMarginLeft+(kUFOWidth + kUFOSeparation) * i,
        kUFOMarginTop},
       {kUFOWidth, kUFOHeight}
       );
    }

    /*
    //cargar el sprite del escudo

    for (int i = 0; i < 2; i++){
        FromFile(&g_assets.escudosMasks[i],
        "../assets/escudos.png",
        {kEscudosMarginLeft + (kEscudosWidth + kEscudosSeparation) * i,
        kEscudos}
    }*/


}

#endif // ASSET_LOADER_CC