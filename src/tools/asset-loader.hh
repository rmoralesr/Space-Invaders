#ifndef ASSET_LOADER_HH
#define ASSET_LOADER_HH 1

#include <esat/sprite.h>

#include "../core/types.cc"
#include "../core/cirno.hh"
#include "../core/mask.hh"
#include "../entities/bullet.hh"

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
    Mask_t ExtraCharacterMasks[kNumCharacters] = {};

    Mask_t YAnimMasks[kYAnimSprites] = {};

    Mask_t LineMask = {};

    esat::SpriteHandle ColorMask;
};

Assets g_assets;

void LoadAssets();

#endif // ASSET_LOADER_HH