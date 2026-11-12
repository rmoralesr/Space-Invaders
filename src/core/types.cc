#ifndef TYPES_CC
#define TYPES_CC 1

#include "../tools/input.cc"
#include "../entities/player.cc"
#include "cirno.cc"

// AQUÍ ESTÁN TODOS LOS TIPOS DE DATOS GENERALES QUE SE VAN A USAR EN EL JUEGO

// NO DEBE HABER VARIABLES GLOBALES, LAS FUNCIONES OBTIENEN LAS COSAS QUE NECESITAN JERÁRQUICAMENTE SIENDO GAME
// LA STRUCT MÁS ALTA EN LA JERARQUÍA, LA CUAL SOLO DEBERÍAN USAR LAS FUNCIONES DIRECTORAS MÁS IMPORTANTES
//
// LOS TIPOS QUE SE VAN A USAR POR DIFERENTES MÓDULOS SON DECLARADOS AQUÍ
//
// LOS TIPOS ESPECIFICOS PARA UN MODULO SE DECLARAN EN EL NAMESPACE DEL MODULO,
// PERO NO SE DEBERÍAN USAR EN NINGÚN OTRO ENTONCES

enum AlienType {
    kAlienType_octopus,
    kAlienType_crab,
    kAlienType_squid
};

enum DescendingState {
    kDescendingState_false,
    kDescendingState_waiting,
    kDescendingState_true
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
    DescendingState descending;
};

enum GameState {
    kGameState_Playing,
    kGameState_Previewing,
    kGameState_ChoosingPlayers
};

const unsigned char kTargetFps = 60; //Control de frames por segundo
const Vector2_t kWindowSize = {224, 256};
long long int g_tick = 0;

struct Game_t {
    // TODO
    GameState game_state;
    Input input;
    Player_t player;
    AlienArmy_t alien_army;
    int credits;
    int score1;
    int score2;
    int hiscore;
};

#endif // TYPES_CC