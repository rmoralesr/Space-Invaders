#ifndef ASSET_LOADER_CC
#define ASSET_LOADER_CC 1

#include "../core/cirno.cc"
#include "../core/mask.cc"
#include <esat/sprite.h>

// Player

//Dimensiones del player
constexpr int kPlayerWidth = 16; // Medida sin margenes 
constexpr int kPlayerHeight = 8; // Medida sin margenes 
constexpr int kPlayerWidth = 16; // Medida sin margenes 
constexpr int kPlayerHeight = 8; // Medida sin margenes 

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
constexpr int kUFOWidth = 16; // Medida sin margenes
constexpr int kUFOHeight = 7; // Medida sin margenes
constexpr int kUFOWidth = 16; // Medida sin margenes
constexpr int kUFOHeight = 7; // Medida sin margenes

constexpr int kUFOSeparation = 3;

constexpr int kUFOMarginTop = 2;
constexpr int kUFOMarginLeft = 1;

//  Shield

//Dimensiones del escudo
constexpr int kShieldWidth = 36; // Medida sin margenes
constexpr int kShieldHeight = 26; // Medida sin margenes

//  Characters

//Dimensiones de los characters
constexpr int kCharactersWidth = 5; // Medida de cada characters
constexpr int kCharactersHeight = 7; // Medida de cada characters

//Cantidad de characters
constexpr int kNumCharacters = 42; // Numero de character

//Separaciones de los characters
constexpr int kCharacterSeparationX = 5; // Separacion entre sprites
constexpr int kCharacterSeparationY = 3; // Separacion entre sprites

// Y anim
constexpr int kYAnimSprites = 5;
constexpr int kYAnimWidth = 16;
constexpr int kYAnimHeigth = 8;
constexpr int kYAnimSeparation = 2;

struct Assets {
    Mask_t AlienMasks[kNumAlienTypes][kAliensPerType] = {};
    Mask_t AlienExplosionMask = {};

    Mask_t PlayerMasks = {};
    Mask_t PlayerExplosion[2] = {};

    Mask_t AlienBulletsMasks[3][4] = {};
    Mask_t AlienBulletExplosionMask;

    Mask_t UFOMasks[2] = {}; //Dos sprites de UFO

    Mask_t ShieldMask = {};
    Mask_t ShieldMask = {};

    Mask_t CharacterMasks[kNumCharacters] = {};
    Mask_t ExtraCharacterMasks[kNumCharacters] = {};

    Mask_t YAnimMasks[kYAnimSprites] = {};

    esat::SpriteHandle ColorMask;

    esat::SpriteHandle ColorMask;
};
Assets g_assets;



void LoadAssets() {

    //cargar todos los caracteres

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            if (i * 8 + j < kNumCharacters) {
                FromFile(&g_assets.CharacterMasks[i * 8 + j],
                "../../assets/caracteres.png",
                {2 + (5 + kCharactersWidth) * j,
                3 + (3 + kCharactersHeight) * i},
                {kCharactersWidth, kCharactersHeight}
                );
            }
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

    FromFile(&g_assets.AlienExplosionMask, "../../assets/AlienExplosion.png");
    
    //cargar las balas de los aliens

    for (int i = 0; i < 4; i++){
        FromFile(
            &g_assets.AlienBulletsMasks[0][i],
            "../../assets/disparo_plunger.png",
            {3*i, 0},
            {3, 7}
        );
    }

    for (int i = 0; i < 4; i++){
        FromFile(
            &g_assets.AlienBulletsMasks[1][i],
            "../../assets/disparo_rolling.png",
            {3*i, 0},
            {3, 7}
        );
    }

    for (int i = 0; i < 4; i++){
        FromFile(
            &g_assets.AlienBulletsMasks[2][i],
            "../../assets/disparo_squiggly.png",
            {3*i, 0},
            {3, 7}
        );
    }

    FromFile(&g_assets.AlienBulletExplosionMask, "../../assets/disparo_explosion.png");

    //cargar los 2 sprites del UFO

    for (int i = 0; i < 2; i++){
          //cargar los 2 sprites del UFO
       FromFile(&g_assets.UFOMasks[i],
       "../../assets/UFO.png",
       {kUFOMarginLeft+(kUFOWidth + kUFOSeparation) * i,
        kUFOMarginTop},
       {kUFOWidth, kUFOHeight}
       );
    }

    //cargar el sprite del Shield
    
    FromFile(&g_assets.ShieldMask, "../../assets/Shield.png");
    
    //cargar los sprites de Player

    FromFile(&g_assets.PlayerMasks, "../../assets/player.png"); 

    for(int i = 0; i < 2; i++){
        FromFile(
        &g_assets.PlayerExplosion[i],
        "../../assets/PlayerExplosion.png",
        {kPlayerWidth * i, 0},
        {kPlayerWidth, kPlayerHeight}
        );
        FromFile(
        &g_assets.PlayerExplosion[i],
        "../../assets/PlayerExplosion.png",
        {kPlayerWidth * i, 0},
        {kPlayerWidth, kPlayerHeight}
        );
    }

    //cargar los sprites de la animacion Y
    for (int i = 0; i < kYAnimSprites; i++){
       FromFile(&g_assets.YAnimMasks[i],
       "../../assets/Y-Anim.png",
       {(kYAnimWidth + kYAnimSeparation) * i, 0},
       {kYAnimWidth, kYAnimHeigth}
       );
    }
    
    g_assets.ColorMask = esat::SpriteFromFile("../../assets/ColorMask.png");
    
}

#endif // ASSET_LOADER_CC