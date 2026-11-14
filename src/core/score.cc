// AUN NO ESTA TERMINADO

#ifndef SCORE_CC
#define SCORE_CC

#include "cirno.cc"
#include "../tools/ui-element.cc"
#include "types.cc"
#include <stdio.h>

void AddScore(int points, Game_t *game){
    Player_t *player = &game->player[game->player_playing];
    Vector2_t score_position;
    if (game->player_playing) score_position = kScore1NumberPosition;
    else score_position = kScore2NumberPosition;

    player->score+=points;
    GenerateNumber(player->score, 4, score_position);

    if(game->hiscore<player->score){
        game->hiscore=player->score;
        GenerateNumber(game->hiscore, 4, kHiScoreNumberPosition);
    }
}



#endif //SCORE_CC

