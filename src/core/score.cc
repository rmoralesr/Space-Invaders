// AUN NO ESTA TERMINADO

#ifndef SCORE_CC
#define SCORE_CC

#include "../tools/ui-element.cc"
#include "types.cc"
#include <stdio.h>

void AddScore(int points, Game_t *game){
    game->player[game->player_playing].score+=points;
    if(!game->player_playing){
    GenerateNumber(game->player[game->player_playing].score, 4, kScore1NumberPosition);
    }else{
        GenerateNumber(game->player[game->player_playing].score, 4, kScore2NumberPosition);
    }

    if(game->hiscore<game->player[game->player_playing].score){
        game->hiscore=game->player[game->player_playing].score;
        GenerateNumber(game->hiscore, 4, kHiScoreNumberPosition);
    }
    
}






#endif //SCORE_CC

