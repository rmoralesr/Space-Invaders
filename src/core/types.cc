#ifndef TYPES_CC
#define TYPES_CC 1

#include "../tools/input.cc"
#include "../entities/player.cc"
#include "cirno.cc"
#include "../entities/aliens_army.cc"

// AQUÍ ESTÁN TODOS LOS TIPOS DE DATOS GENERALES QUE SE VAN A USAR EN EL JUEGO

// NO DEBE HABER VARIABLES GLOBALES, LAS FUNCIONES OBTIENEN LAS COSAS QUE NECESITAN JERÁRQUICAMENTE SIENDO GAME
// LA STRUCT MÁS ALTA EN LA JERARQUÍA, LA CUAL SOLO DEBERÍAN USAR LAS FUNCIONES DIRECTORAS MÁS IMPORTANTES
//
// LOS TIPOS QUE SE VAN A USAR POR DIFERENTES MÓDULOS SON DECLARADOS AQUÍ
//
// LOS TIPOS ESPECIFICOS PARA UN MODULO SE DECLARAN EN EL NAMESPACE DEL MODULO,
// PERO NO SE DEBERÍAN USAR EN NINGÚN OTRO ENTONCES

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
    AlienArmy alien_army;
    int credits;
    int score1;
    int score2;
    int hiscore;
};

#endif // TYPES_CC