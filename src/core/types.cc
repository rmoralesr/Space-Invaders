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
const int kBunkerLimit = 4;
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

struct Bunker_t {
    Body_t body;
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
    int plunger_table_pos;
    int squiggly_table_pos;
    int left_x;
    BulletType bullet_to_move;
    AlienArmyState army_state;
};

struct Player_t {
    bool active;
    bool dead;
    int score;
    int lives;

    Body_t body;
    Bunker_t bunker[kBunkerLimit];
    Bullet_t bullet;
    AlienArmy_t alien_army;
};

enum GameState {
    kGameState_Playing,
    kGameState_Previewing,
    kGameState_ChoosingPlayers,
    kGameState_AboutToPlay
};

enum kUpdateID {
	kUpdateID_Previewing,
    kUpdateID_InsertCoin,
    kUpdateID_ChoosingPlayer,
    kUpdateID_AboutToPlay,
    kUpdateID_Playing,
	kUpdateID_UpdateScore
};

struct Game_t {
    GameState game_state;
    Input input;
    Player_t player[2];
    bool player_playing;
    bool multiplayer;
    int credits;
    int hiscore;
};

struct QueuedUpdate{
	bool active = false;
	int id;
	long long int queued_tick;
};

Game_t g_game = {};

const unsigned int Update_kQueueCapacity = 10;
QueuedUpdate Update_queue[Update_kQueueCapacity] = {0};

void QueueUpdate(int id, long long int tick){
    int n = 0;
    bool free_space = false;
    while(n < Update_kQueueCapacity && !free_space){
        if (!Update_queue[n].active) free_space = true;
        else n++;
    }
	if (free_space){
		Update_queue[n].active = true;
		Update_queue[n].id = id;
		Update_queue[n].queued_tick = tick;
	}
}

#endif // TYPES_CC