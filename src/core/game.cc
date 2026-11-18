#include "game.hh"

long long int g_tick = 0;
Game_t g_game = {};
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

void ClearQueuedActions(){
	for (int i = 0; i < Update_kQueueCapacity; i++){
		Update_queue[i].active = false;
	}
	g_wipe_screen.current_step = -1;
	g_wipe_screen.queued_tick = 0;
}

bool IsActionQueued(int id) {
	for (int i = 0; i < Update_kQueueCapacity; i++){
		if (Update_queue[i].active && Update_queue[i].id == id) return true;
	}
	return false;
}