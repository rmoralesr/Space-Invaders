// AUN NO ESTA TERMINADO

#ifndef SCORE_CC
#define SCORE_CC

#include "cirno.cc"
#include "../tools/ui-element.cc"
#include "types.cc"
#include <stdio.h>


void UpdateScore(){
    if (g_game.game_state != kGameState_Demo){
        Player_t *player = &g_game.player[g_game.player_playing];
        Vector2_t score_position;
        if (!g_game.player_playing) score_position = kScore1NumberPosition;
        else score_position = kScore2NumberPosition;
        GenerateNumber(player->score, 4, score_position);
        GenerateNumber(g_game.hiscore, 4, kHiScoreNumberPosition);
    }
}
void AddScore(int points, bool update_score = true){
    if (g_game.game_state != kGameState_Demo){
        Player_t *player = &g_game.player[g_game.player_playing];

        if (player->score < 1500 && player->score + points >= 1500) {
            player->lives++;
            GenerateLives(player->lives);
        }

        player->score += points;
        if(g_game.hiscore < player->score) {
            g_game.hiscore = player->score;
            if (g_game.hiscore > 9999) g_game.hiscore %= 10000;
        }
        if (player->score > 9999) player->score %= 10000;

        if (update_score) UpdateScore();
    }
}

#endif //SCORE_CC

