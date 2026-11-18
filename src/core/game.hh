#ifndef GAME_HH
#define GAME_HH 1

#include "cirno.hh"
#include "../tools/input.hh"
#include "../entities/player.hh"

const unsigned char kTargetFps = 60; //Control de frames por segundo
const Vector2_t kWindowSize = {224, 256};
extern long long int g_tick;

enum GameState {
    kGameState_Previewing,
    kGameState_Demo,
    kGameState_DemoDeath,
    kGameState_InsertCoin,
    kGameState_ChoosingPlayers,
    kGameState_AboutToPlay,
    kGameState_Playing,
    kGameState_Death,
    kGameState_GameOver
};

enum kUpdateID {
	kUpdateID_Previewing,
    kUpdateID_Demo,
    kUpdateID_InsertCoin,
    kUpdateID_ChoosingPlayer,
    kUpdateID_AboutToPlay,
    kUpdateID_Playing,
    kUpdateID_Afterdeath,
    kUpdateID_GameOver,
    kUpdateID_OnGameOverEnd,
	kUpdateID_UpdateScore
};

// Game

struct Game_t {
    GameState game_state;
    Input input;
    Player_t player[2];
    bool player_playing;
    bool multiplayer;
    int credits;
    int hiscore;
};

extern Game_t g_game;

// QueuedUpdate

struct QueuedUpdate{
	bool active = false;
	int id;
	long long int queued_tick;
};

const unsigned int Update_kQueueCapacity = 10;
extern QueuedUpdate Update_queue[Update_kQueueCapacity];

void QueueUpdate(int id, long long int tick);

#endif // GAME_HH