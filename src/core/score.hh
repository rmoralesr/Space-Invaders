#ifndef SCORE_HH
#define SCORE_HH 1

#include <esat_extra/soloud/soloud.h>
#include <esat_extra/soloud/soloud_wav.h>

extern SoLoud::Soloud g_Soloud;
extern SoLoud::Wav g_ExtraLife;

// AUN NO ESTA TERMINADO

void UpdateScore();
void AddScore(int points, bool update_score = true);

#endif //SCORE_HH

