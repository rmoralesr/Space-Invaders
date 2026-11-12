#ifndef TYPES_CC
#define TYPES_CC 1

#include "../tools/input.cc"
#include "../entities/body.cc"
#include "../entities/bullet.cc"
#include "cirno.cc"

// AQUÍ ESTÁN TODOS LOS TIPOS DE DATOS GENERALES QUE SE VAN A USAR EN EL JUEGO

// NO DEBE HABER VARIABLES GLOBALES, LAS FUNCIONES OBTIENEN LAS COSAS QUE NECESITAN JERÁRQUICAMENTE SIENDO GAME
// LA STRUCT MÁS ALTA EN LA JERARQUÍA, LA CUAL SOLO DEBERÍAN USAR LAS FUNCIONES DIRECTORAS MÁS IMPORTANTES
//
// LOS TIPOS QUE SE VAN A USAR POR DIFERENTES MÓDULOS SON DECLARADOS AQUÍ
//
// LOS TIPOS ESPECIFICOS PARA UN MODULO SE DECLARAN EN EL NAMESPACE DEL MODULO,
// PERO NO SE DEBERÍAN USAR EN NINGÚN OTRO ENTONCES

const unsigned char kTargetFps = 60; //Control de frames por segundo
const Vector2_t kWindowSize = {224, 256};
long long int g_tick = 0;

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

struct Alien_t {
    Body_t body;
    bool active;
    AlienType type;
    int sprite_frame;
};

struct AlienArmy_t {
    Alien_t aliens[55];
    int aliens_alive;
    int alien_died;
    int alien_explosion_tick;
    int last_alien_moved;
    int direction;
    AlienArmyState descending;
};

struct Player_t {
    bool active = false;
    bool dying;
    int score;
    int lives;

    Body_t body;
    Bullet_t bullet;
    AlienArmy_t alien_army;
};

enum GameState {
    kGameState_Playing,
    kGameState_Previewing,
    kGameState_ChoosingPlayers,
    kGameState_AboutToPlay
};

struct Game_t {
    GameState game_state;
    Input input;
    Player_t player[2];
    int credits;
    int hiscore;
};

#endif // TYPES_CC