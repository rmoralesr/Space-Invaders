#include "asset-loader.hh"

void LoadAssets() {

    //CARACTERES
    //cargar todos los caracteres
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            if (i * 8 + j < kNumCharacters) {
                FromFile(&g_assets.CharacterMasks[i * 8 + j],
                "../../assets/caracteres.png",
                {2 + (5 + kCharactersWidth) * j,
                3 + (3 + kCharactersHeight) * i},
                {kCharactersWidth, kCharactersHeight}
                );
            }
        }
    }

    //ALIENS
    //cargar 3 tipos de Aliens
    for (int i = 0; i < kNumAlienTypes; i++) {
        for (int j = 0; j < kAliensPerType; j++) {

          FromFile(
              &g_assets.AlienMasks[i][j],
               "../../assets/aliens.png",
               {2 + (kAlienWidth + kAlienSeparationX) * j,
               1 + (kAlienHeight + kAlienSeparationY) * i},
              {kAlienWidth, kAlienHeight}
            );
        }
    }

    //cargar explosion de los aliens
    FromFile(&g_assets.AlienExplosionMask, "../../assets/AlienExplosion.png");
    
    //cargar las balas de los aliens
    for (int i = 0; i < 4; i++){
        FromFile(
            &g_assets.AlienBulletsMasks[kBulletType_Plunger-1][i],
            "../../assets/disparo_plunger.png",
            {3*i, 0},
            {3, 7}
        );
    }

    for (int i = 0; i < 4; i++){
        FromFile(
            &g_assets.AlienBulletsMasks[kBulletType_Rolling-1][i],
            "../../assets/disparo_rolling.png",
            {3*i, 0},
            {3, 7}
        );
    }

    for (int i = 0; i < 4; i++){
        FromFile(
            &g_assets.AlienBulletsMasks[kBulletType_Squiggly-1][i],
            "../../assets/disparo_squiggly.png",
            {3*i, 0},
            {3, 7}
        );
    }

    //cargar la explosion del disparo (aliens)
    FromFile(&g_assets.AlienBulletExplosionMask, "../../assets/disparo_explosion_aliens.png");

    //UFO
    //cargar los 2 sprites del UFO
    FromFile(&g_assets.UFOMasks[0], "../../assets/UFO.png");
    FromFile(&g_assets.UFOMasks[1], "../../assets/UFOExplosion.png");

    //SHIELD
    //cargar el sprite del Shield
    FromFile(&g_assets.ShieldMask, "../../assets/Shield.png");
    
    //PLAYER
    //cargar los sprites de Player
    FromFile(&g_assets.PlayerMask, "../../assets/player.png"); 

    //cargar los sprites de PlayerBulet
    FromFile(&g_assets.PlayerBullet, "../../assets/PlayerBullet.png");

    //cargar los sprites de playerExplosion
    for(int i = 0; i < 2; i++){
        FromFile(
        &g_assets.PlayerExplosion[i],
        "../../assets/PlayerExplosion.png",
        {kPlayerWidth * i, 0},
        {kPlayerWidth, kPlayerHeight}
        );
        FromFile(
        &g_assets.PlayerExplosion[i],
        "../../assets/PlayerExplosion.png",
        {kPlayerWidth * i, 0},
        {kPlayerWidth, kPlayerHeight}
        );
    }

    //cargar la explosion del disparo (jugador)
    FromFile(&g_assets.PlayerBulletExplosion, "../../assets/disparo_explosion.png");

    // ANIMACION Y
    //cargar los sprites de la animacion Y
    for (int i = 0; i < kYAnimSprites; i++){
       FromFile(&g_assets.YAnimMasks[i],
       "../../assets/Y-Anim.png",
       {(kYAnimWidth + kYAnimSeparation) * i, 0},
       {kYAnimWidth, kYAnimHeigth}
       );
    }
    
    g_assets.ColorMask = esat::SpriteFromFile("../../assets/ColorMask.png");

    g_assets.LineMask.size = {224, 1};
    for (int i = 0; i < 224; i++) WriteBit(&g_assets.LineMask, i, 0, 1);
}