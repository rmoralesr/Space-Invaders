#ifndef TYPES_CC
#define TYPES_CC 1

#include "../tools/input.cc"
#include "cirno.cc"

// AQUÍ ESTÁN TODOS LOS TIPOS DE DATOS GENERALES QUE SE VAN A USAR EN EL JUEGO

// NO DEBE HABER VARIABLES GLOBALES, LAS FUNCIONES OBTIENEN LAS COSAS QUE NECESITAN JERÁRQUICAMENTE SIENDO GAME
// LA STRUCT MÁS ALTA EN LA JERARQUÍA, LA CUAL SOLO DEBERÍAN USAR LAS FUNCIONES DIRECTORAS MÁS IMPORTANTES
//
// LOS TIPOS QUE SE VAN A USAR POR DIFERENTES MÓDULOS SON DECLARADOS AQUÍ
//
// LOS TIPOS ESPECIFICOS PARA UN MODULO SE DECLARAN EN EL NAMESPACE DEL MODULO,
// PERO NO SE DEBERÍAN USAR EN NINGÚN OTRO ENTONCES

const unsigned char kTargetFps = 25; //Control de frames por segundo
const unsigned int kPixelScale = 4;
const Vector2_t kWindowSize = {224, 256};

enum GameState {
    kGameState_Playing,
    kGameState_Previewing,
    kGameState_ChoosingPlayers
};

enum AlienType {
    kAlienType_squid = 1,
    kAlienType_crab,
    kAlienType_octopus
};

struct Alien {
    bool is_alive;
    Vector2_t position;
    AlienType type;
};

struct AlienArmy {
    Alien aliens[55];
    int aliens_alive;
    int alien_died = -1;
    int last_alien_moved = 0;
};

struct Game {
    // TODO
    AlienArmy alien_army;
    GameState game_state;
    Input input;
    int credits;
};
