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
SoLoud::Wav g_ExtraLife;
SoLoud::Wav g_AlienSounds[4];


/* ----PARA COPIAR Y PEGAR en MAIN
g_shoot.load("../sound/shoot.ogg");
g_PlayerExplosion.load("../sound/explosion.ogg");
g_UFOflying.load("../sound/ufo_highpitch.ogg");
g_UFOdead.load("../sound/ufo_lowpitch.ogg");
g_AlienExplosion.load("../sound/alienkilled.ogg");
g_ExtraLife.load("../sound/extendedPlay.ogg");
g_AlienSounds[0].load("../sound/fastinvader1.ogg");
g_AlienSounds[1].load("../sound/fastinvader2.ogg");
g_AlienSounds[2].load("../sound/fastinvader3.ogg");
g_AlienSounds[3].load("../sound/fastinvader4.ogg");





g_Soloud.play(g_Mov1);
g_Soloud.play(g_Mov2);
g_Soloud.play(g_Mov3);
g_Soloud.play(g_Mov4);
*/

#endif // SOUND_CC