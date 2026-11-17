#ifndef PLAYER_HH
#define PLAYER_HH 1

#include "../core/cirno.hh"
#include "../core/mask.hh"
#include "../core/draw.hh"
#include "../core/types.cc"
#include "../tools/asset-loader.hh"
#include "../tools/ui-element.cc"
#include "body.hh"
#include "bullet.hh"
#include "UFO.hh"
#include "aliens_army.cc"

struct Bunker_t {
    Body_t body;
};

struct Player_t {
    bool active;
    bool dead;
    bool dying;
    int dying_tick;
    int score;
    int lives;

    Body_t body;
    Bunker_t bunker[kBunkerLimit];
    AlienArmy_t alien_army;
};


void Player_Assets(Player_t *self_assets);

void Player_Bunker_Assets(Player_t *self_assets);

void Player_Construct(Player_t *self);
void Destruct(Player_t *self);

void Player_Reset(Player_t *self);

void Dead(Player_t *self, int damage);

// 2s de animación alternando frame a frame los dos sprites. 
void Dead_Animation(Player_t *self);

void MoveTo(Player_t *self, Vector2_t pos);

void Move(Player_t *self, Vector2_t delta);

void Shoot(Player_t *self);

void Draw_Bunkers(Player_t *self_bunkers);

void Draw(Player_t *self);

#endif // PLAYER_HH