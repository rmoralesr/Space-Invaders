#ifndef ALIENS_ARMY_HH
#define ALIENS_ARMY_HH 1

#include <esat_extra/soloud/soloud.h>
#include <esat_extra/soloud/soloud_wav.h>

#include "body.hh"
#include "../entities/UFO.hh"
#include "../entities/bullet.hh"
#include "../core/score.hh"
#include "../tools/asset-loader.hh"

extern SoLoud::Soloud g_Soloud;
extern SoLoud::Wav g_AlienExplosion;
extern SoLoud::Wav g_AlienSounds[4];


const int kAlienBulletsTable[] = {1,7,1,1,1,4,11,1,6,3,1,1,11,9,2,8,2,11,4,7,10};

enum AlienType {
    kAlienType_octopus,
    kAlienType_crab,
    kAlienType_squid
};

enum AlienArmyState {
    kAlienArmyState_generating,
    kAlienArmyState_descendingFalse,
    kAlienArmyState_descendingWaiting,
    kAlienArmyState_descendingTrue
};

enum AlienArmyFlag {
    kAlienArmyFlag_none,
    kAlienArmyFlag_exterminated,
    kAlienArmyFlag_invaded
};

// Alien

struct Alien_t {
    Body_t body;
    bool active;
    bool showing;
    AlienType type;
    int sprite_frame;
    int id;
};

void Alien_Init(Alien_t *alien, int row, int col);
void AsignAlienMask(Alien_t *alien);

void Alien_Move(Alien_t *alien, Vector2_t delta);

void Dead(Alien_t *alien);

// AlienArmy

struct AlienArmy_t {
    bool active;
    Alien_t aliens[55];
    int sound_index = 0;
    int aliens_alive;
    int alien_died;
    int alien_explosion_tick;
    int last_alien_moved;
    int last_alien_showing;
    int direction;
    int plunger_table_pos;
    int squiggly_table_pos;
    int left_x;
    bool regenerating;
    BulletType bullet_to_move;
    AlienArmyState army_state;
    
};

void AlienArmy_Init(AlienArmy_t *alien_army);

AlienArmyFlag AlienArmy_Move(AlienArmy_t *alien_army, int score, int playerX);
AlienArmyFlag AlienArmy_Control(AlienArmy_t *alien_army, int score, int playerX);

int AlienArmy_ReloadRate(int score);
bool AlienArmy_CanFire(AlienArmy_t *alien_army, int score, BulletType bullet);

Alien_t *AlienArmy_GetLowestAlienInColumn(AlienArmy_t *alien_army, int col);
int AlienArmy_GetColNearPlayer(AlienArmy_t *alien_army, int playerX);
int AlienArmy_GetStartingY(int rounds);

int AlienArmy_PickBulletX(AlienArmy_t *alien_army, int playerX, BulletType bullet_type);
int AlienArmy_PickBulletY(AlienArmy_t *alien_army, int col);

void AlienArmy_ShootSquiggly(AlienArmy_t *alien_army, int playerX);
void AlienArmy_ShootPlunger(AlienArmy_t *alien_army, int playerX);
void AlienArmy_ShootRolling(AlienArmy_t *alien_army, int playerX);

void AlienArmy_ControlBullets(AlienArmy_t *alien_army, int score, int playerX);

void AlienArmy_SetRegeneration(AlienArmy_t *alien_army);

void AliensRender(AlienArmy_t *alien_army);

AlienType GetAlienType(int row);

int AlienGetPoints(AlienType type);

#endif // ALIENS_ARMY_HH