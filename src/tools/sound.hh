#ifndef SOUND_HH
#define SOUND_HH 1

#include <esat_extra/soloud/soloud.h>
#include <esat_extra/soloud/soloud_wav.h>

extern SoLoud::Soloud g_Soloud;
extern SoLoud::Wav g_shoot;
extern SoLoud::Wav g_PlayerExplosion;
extern SoLoud::Wav g_UFOflying; //Este debe sonar en bucle mientras aparezca la UFO
extern SoLoud::Wav g_UFOdead;
extern SoLoud::Wav g_AlienExplosion;
extern SoLoud::Wav g_ExtraLife;
extern SoLoud::Wav g_AlienSounds[4];

#endif // SOUND_HH