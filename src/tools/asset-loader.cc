#include "asset-loader.hh"

<<<<<<< HEAD
#include "../core/types.cc"
#include "../core/cirno.cc"
#include "../core/mask.cc"
#include <esat/sprite.h>

enum BulletType {
  kBulletType_Player,

  kBulletType_Squiggly,
  kBulletType_Plunger,
  kBulletType_Rolling // Esta te apunta a ti!
};

// PLAYER

//Dimensiones del player
constexpr int kPlayerWidth = 16; // Medida sin margenes 
constexpr int kPlayerHeight = 8; // Medida sin margenes 

//Separaciones
constexpr int kPlayerSeparation = 5; // Separacion del margen

//  ALIENS

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

constexpr int kUFOSeparation = 3; // Separacion del margen

//  SHIELD

//Dimensiones del escudo
constexpr int kShieldWidth = 36; // Medida sin margenes
constexpr int kShieldHeight = 26; // Medida sin margenes

//  CHARACTERS

//Dimensiones de los characters
constexpr int kCharactersWidth = 5; // Medida de cada characters
constexpr int kCharactersHeight = 7; // Medida de cada characters

//Cantidad de characters
constexpr int kNumCharacters = 42; // Numero de character

//Separaciones de los characters
constexpr int kCharacterSeparationX = 5; // Separacion entre sprites
constexpr int kCharacterSeparationY = 3; // Separacion entre sprites

// ANIMACION Y

// Y anim
constexpr int kYAnimSprites = 5;
constexpr int kYAnimWidth = 16;
constexpr int kYAnimHeigth = 8;
constexpr int kYAnimSeparation = 2;

struct Assets {
    Mask_t AlienMasks[kNumAlienTypes][kAliensPerType] = {};
    Mask_t AlienExplosionMask = {};

    Mask_t PlayerMask = {};
    Mask_t PlayerExplosion[2] = {};
    Mask_t PlayerBullet = {};
    Mask_t PlayerBulletExplosion = {};

    Mask_t AlienBulletsMasks[3][4] = {};
    Mask_t AlienBulletExplosionMask;

    Mask_t UFOMasks[2] = {};

    Mask_t ShieldMask = {};

    Mask_t CharacterMasks[kNumCharacters] = {};

    Mask_t YAnimMasks[kYAnimSprites] = {};

    Mask_t LineMask = {};

    esat::SpriteHandle ColorMask;
};
=======
>>>>>>> origin/features
Assets g_assets;

void LoadAssets() {

    //CARACTERES
    //cargar todos los caracteres
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            if (i * 8 + j < kNumCharacters) {
                FromFile(&g_assets.CharacterMasks[i * 8 + j],
                "../../assets/images/caracteres.png",
                {2 + (5 + kCharactersWidth) * j,
                3 + (3 + kCharactersHeight) * i},
                {kCharactersWidth, kCharactersHeight}
                );
            }
        }
    }

    //ALIENS
    //cargar 3 tipos de Aliens
    for (int i = 0; i < kNumAlienTypes; i++) {
        for (int j = 0; j < kAliensPerType; j++) {

          FromFile(
              &g_assets.AlienMasks[i][j],
               "../../assets/images/aliens.png",
               {2 + (kAlienWidth + kAlienSeparationX) * j,
               1 + (kAlienHeight + kAlienSeparationY) * i},
              {kAlienWidth, kAlienHeight}
            );
        }
    }

    //cargar explosion de los aliens
    FromFile(&g_assets.AlienExplosionMask, "../../assets/images/AlienExplosion.png");
    
    //cargar las balas de los aliens
    for (int i = 0; i < 4; i++){
        FromFile(
            &g_assets.AlienBulletsMasks[kBulletType_Plunger-1][i],
            "../../assets/images/disparo_plunger.png",
            {3*i, 0},
            {3, 7}
        );
    }

    for (int i = 0; i < 4; i++){
        FromFile(
            &g_assets.AlienBulletsMasks[kBulletType_Rolling-1][i],
            "../../assets/images/disparo_rolling.png",
            {3*i, 0},
            {3, 7}
        );
    }

    for (int i = 0; i < 4; i++){
        FromFile(
            &g_assets.AlienBulletsMasks[kBulletType_Squiggly-1][i],
            "../../assets/images/disparo_squiggly.png",
            {3*i, 0},
            {3, 7}
        );
    }

    //cargar la explosion del disparo (aliens)
    FromFile(&g_assets.AlienBulletExplosionMask, "../../assets/images/disparo_explosion_aliens.png");

    //UFO
    //cargar los 2 sprites del UFO
    FromFile(&g_assets.UFOMasks[0], "../../assets/images/UFO.png");
    FromFile(&g_assets.UFOMasks[1], "../../assets/images/UFOExplosion.png");

    //SHIELD
    //cargar el sprite del Shield
    FromFile(&g_assets.ShieldMask, "../../assets/images/Shield.png");
    
    //PLAYER
    //cargar los sprites de Player
    FromFile(&g_assets.PlayerMask, "../../assets/images/player.png");

    //cargar los sprites de PlayerBulet
    FromFile(&g_assets.PlayerBullet, "../../assets/images/PlayerBullet.png");

    //cargar los sprites de playerExplosion
    for(int i = 0; i < 2; i++){
        FromFile(
        &g_assets.PlayerExplosion[i],
        "../../assets/images/PlayerExplosion.png",
        {kPlayerWidth * i, 0},
        {kPlayerWidth, kPlayerHeight}
        );
<<<<<<< HEAD
=======
        FromFile(
        &g_assets.PlayerExplosion[i],
        "../../assets/images/PlayerExplosion.png",
        {kPlayerWidth * i, 0},
        {kPlayerWidth, kPlayerHeight}
        );
>>>>>>> origin/features
    }

    //cargar la explosion del disparo (jugador)
    FromFile(&g_assets.PlayerBulletExplosion, "../../assets/images/disparo_explosion.png");

    // ANIMACION Y
    //cargar los sprites de la animacion Y
    for (int i = 0; i < kYAnimSprites; i++){
       FromFile(&g_assets.YAnimMasks[i],
       "../../assets/images/Y-Anim.png",
       {(kYAnimWidth + kYAnimSeparation) * i, 0},
       {kYAnimWidth, kYAnimHeigth}
       );
    }
    
    g_assets.ColorMask = esat::SpriteFromFile("../../assets/images/ColorMask.png");

    g_assets.LineMask.size = {224, 1};
    for (int i = 0; i < 224; i++) WriteBit(&g_assets.LineMask, i, 0, 1);
}