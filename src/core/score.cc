// AUN NO ESTA TERMINADO

#ifndef SCORE_CC
#define SCORE_CC

#include "cirno.cc"
#include "../tools/ui-element.cc"
#include "types.cc"
#include <stdio.h>


void UpdateScore(){
    Player_t *player = &g_game.player[g_game.player_playing];
    Vector2_t score_position;
    if (!g_game.player_playing) score_position = kScore1NumberPosition;
    else score_position = kScore2NumberPosition;
    GenerateNumber(player->score, 4, score_position);
    GenerateNumber(g_game.hiscore, 4, kHiScoreNumberPosition);
}
void AddScore(int points, bool update_score = true){
    Player_t *player = &g_game.player[g_game.player_playing];
    Vector2_t score_position;
    if (!g_game.player_playing) score_position = kScore1NumberPosition;
    else score_position = kScore2NumberPosition;

    player->score += points;
    if(g_game.hiscore < player->score) g_game.hiscore = player->score;
    if (update_score) UpdateScore();
}



#endif //SCORE_CC

