#ifndef PLAYER_HH
#define PLAYER_HH 1

#include <esat_extra/soloud/soloud.h>
#include <esat_extra/soloud/soloud_wav.h>

#include "../core/cirno.hh"
#include "../core/mask.hh"
#include "../core/draw.hh"
#include "../tools/asset-loader.hh"
#include "../tools/ui-element.hh"
#include "body.hh"
#include "bullet.hh"
#include "aliens_army.hh"

extern SoLoud::Soloud g_Soloud;
extern SoLoud::Wav g_PlayerExplosion;

const int kBunkerLimit = 4;

struct Bunker_t {
    Body_t body;
};

struct Player_t {
    bool active;
    bool pause;
    bool dead;
    bool dying;
    int pause_tick;
    int dead_tick;
    int dying_tick;
    int score;
    int lives;
    int rounds;

    Body_t body;
    Bunker_t bunker[kBunkerLimit];
    AlienArmy_t alien_army;
};


void Player_Assets(Player_t *self_assets);

void Player_Bunker_Assets(Player_t *self_assets);
void Player_Bunker_Reset();

void Player_Construct(Player_t *self);

void PlayerSetUp(Player_t *self);
void Player_Reset(Player_t *self);
void Player_Bunker_Reset();

void Dead(Player_t *self, int damage);
void Dead_Animation(Player_t *self);

void Move(Player_t *self, Vector2_t delta);

void Shoot(Player_t *self);

void Draw_Bunkers(Player_t *self_bunkers);

void Draw(Player_t *self);

#endif // PLAYER_HH