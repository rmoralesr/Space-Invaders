// AUN NO ESTA TERMINADO

#ifndef SCORE_CC
#define SCORE_CC

#include "../tools/ui-element.cc"
#include "types.cc"
#include <stdio.h>

void AddScore(int points, Player_t *player, Game_t *game){
    player->score+=points;
    GenerateNumber(player->score, 4, kScore1NumberPosition);
    if(game->hiscore<player->score){
        game->hiscore=player->score;
        GenerateNumber(game->hiscore, 4, kHiScoreNumberPosition);
    }
    
}






#endif //SCORE_CC

