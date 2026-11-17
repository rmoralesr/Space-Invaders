#ifndef SOUND_CC
#define SOUND_CC

#include <esat_extra/soloud/soloud.h>
#include <esat_extra/soloud/soloud_wav.h>

SoLoud::Soloud g_Soloud;
SoLoud::Wav g_shoot;
SoLoud::Wav g_PlayerExplosion;
SoLoud::Wav g_UFOflying; //Este debe sonar en bucle mientras aparezca la UFO
SoLoud::Wav g_UFOdead;
SoLoud::Wav g_AlienExplosion;

//Faltan mas sonidos


/* ----PARA COPIAR Y PEGAR en MAIN
g_shoot.load("../sound/shoot.ogg");
g_PlayerExplosion.load("../sound/explosion.ogg");
g_UFOflying.load("../sound/ufo_highpitch.ogg");
g_UFOdead.load("../sound/ufo_lowpitch.ogg");
g_AlienExplosion.load("../sound/alienkilled.ogg");
*/

#endif // SOUND_CC