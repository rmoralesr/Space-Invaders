#include "aliens_army.hh"
#include "..\core\game.hh"
#include "player.hh"

void AsignAlienMask(Alien_t *alien) {
    alien->body.mask = g_assets.AlienMasks[alien->type][alien->sprite_frame];
}

AlienType GetAlienType(int row) {
    if (row < 2) {
        return kAlienType_octopus;
    }
    if (row < 4) {
        return kAlienType_crab;
    }
    return kAlienType_squid;
}

int AlienArmy_GetStartingY(int rounds) {
    switch (rounds) {
        case 0: return 136;
            break;
        case 1: return 160;
            break;
        case 2: return 176;
            break;
        case 3:
        case 4:
        case 5:
            return 184;
            break;
        case 6:
        case 7:
        case 8:
            return 192;
            break;
        default: break;
    }
    return 136;
}

void Alien_Init(Alien_t *alien, int row, int col) {
    alien->type = GetAlienType(row);
    alien->active = false;
    alien->body.can_collide = true;
    alien->body.mask = {};
    alien->body.position.x = col * 16 + 24;
    alien->body.position.y = AlienArmy_GetStartingY(g_game.player[g_game.player_playing].rounds) - 8 - row * 16;
    alien->sprite_frame = 0;
    alien->id = row*11+col;
    alien->showing = true;
    AsignAlienMask(alien);
}

void AlienArmy_Init(AlienArmy_t *alien_army) {
    alien_army->aliens_alive = 55;
    alien_army->last_alien_moved = 54;
    alien_army->alien_died = -1;
    alien_army->alien_explosion_tick = 0;
    alien_army->direction = 1;
    alien_army->army_state = kAlienArmyState_generating;
    alien_army->bullet_to_move = kBulletType_Squiggly;
    alien_army->plunger_table_pos = 0;
    alien_army->squiggly_table_pos = 6;
    alien_army->regenerating = false;
    alien_army->left_x = 24 + kAlienWidth;
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 11; col++) {
            Alien_Init(&alien_army->aliens[row*11+col], row, col);
        }
    }
}

void Alien_Move(Alien_t *alien, Vector2_t delta) {
    Move(&alien->body, delta);
    alien->sprite_frame = (alien->sprite_frame+1)%2;
    AsignAlienMask(alien);
    
   
}

int AlienArmy_ReloadRate(int score) {
    if (score < 300)  return 30;
    if (score < 1100) return 16;
    if (score < 2100) return 11;
    if (score < 3100) return 8;
    return 7;
}

bool AlienArmy_CanFire(AlienArmy_t *alien_army, int score, BulletType bullet) { // TODO mas condicionales
    if (g_game.player[g_game.player_playing].pause) return false;
    int rate = AlienArmy_ReloadRate(score);
    for (int i = 1; i < 4; i++) {
        if (bullet != i && Bullet_registry[i].step <= rate && Bullet_registry[i].active) return false;
    }
    return true;
}

int AlienArmy_GetColNearPlayer(AlienArmy_t *alien_army, int playerX) {
    int col = (playerX + 8 - (alien_army->left_x - kAlienWidth))/ 16;
    if (col < 0) col = 0;
    if (col > 10) col = 10;
    return col;
}

int AlienArmy_PickBulletX(AlienArmy_t *alien_army, int playerX, BulletType bullet_type) {
    int col;
    switch (bullet_type) {
        case kBulletType_Rolling:
            col = AlienArmy_GetColNearPlayer(alien_army, playerX);
            return col * 16 + alien_army->left_x - 8;
            break;
        case kBulletType_Plunger:
            col = kAlienBulletsTable[alien_army->plunger_table_pos];
            alien_army->plunger_table_pos = (alien_army->plunger_table_pos + 1) % 16;
            return (col-1) * 16 + alien_army->left_x - 8;
            break;
        case kBulletType_Squiggly:
            col = kAlienBulletsTable[alien_army->squiggly_table_pos];
            alien_army->squiggly_table_pos = 6 + (alien_army->squiggly_table_pos - 6 + 1) % 15;
            return (col-1) * 16 + alien_army->left_x - 8;
            break;
        default: break;
    }
    return 0;
}

int AlienArmy_PickBulletY(AlienArmy_t *alien_army, int col) {
    for (int row = 0; row < 5; row++) {
        if (alien_army->aliens[row*11+col].active) return alien_army->aliens[row*11+col].body.position.y - 1;
    }
    return -1;
}

void AlienArmy_SetRegeneration(AlienArmy_t *alien_army) {
    alien_army->regenerating = true;
    for (int i = 0; i < 55; i++) {
        alien_army->aliens[i].showing = false;
    }
    alien_army->last_alien_showing = 54;
}

Alien_t *AlienArmy_GetLowestAlienInColumn(AlienArmy_t *alien_army, int col) {
    for (int row = 0; row < 5; row++) {
        if (alien_army->aliens[row*11+col].active) return &alien_army->aliens[row*11+col];
    }
    return nullptr;
}

void AlienArmy_ShootRolling(AlienArmy_t *alien_army, int playerX) {
    int y = AlienArmy_PickBulletY(alien_army, AlienArmy_GetColNearPlayer(alien_army, playerX));
    int x = AlienArmy_PickBulletX(alien_army, playerX, kBulletType_Rolling);
    if (y != -1) {
        Bullet_registry[3] = *Bullet_Construct(kBulletType_Rolling);
        Bullet_registry[3].body.mask = g_assets.AlienBulletsMasks[1][0];
        MoveTo(&Bullet_registry[3], {x, y+8});
        AdvanceUFOPointsIndex();
    }
}

void AlienArmy_ShootSquiggly(AlienArmy_t *alien_army, int playerX) {
    int y = AlienArmy_PickBulletY(alien_army, kAlienBulletsTable[alien_army->squiggly_table_pos] - 1);
    int x = AlienArmy_PickBulletX(alien_army, playerX, kBulletType_Squiggly);
    if (y != -1) {
        Bullet_registry[1] = *Bullet_Construct(kBulletType_Squiggly);
        Bullet_registry[1].body.mask = g_assets.AlienBulletsMasks[2][0];
        MoveTo(&Bullet_registry[1], {x, y+8});
        AdvanceUFOPointsIndex();
    }
}

void AlienArmy_ShootPlunger(AlienArmy_t *alien_army, int playerX) {
    int y = AlienArmy_PickBulletY(alien_army, kAlienBulletsTable[alien_army->plunger_table_pos] - 1);
    int x = AlienArmy_PickBulletX(alien_army, playerX, kBulletType_Plunger);
    if (y != -1) {
        Bullet_registry[2] = *Bullet_Construct(kBulletType_Plunger);
        Bullet_registry[2].body.mask = g_assets.AlienBulletsMasks[0][0];
        MoveTo(&Bullet_registry[2], {x, y+8});
        AdvanceUFOPointsIndex();
    }
}

void AlienArmy_ControlBullets(AlienArmy_t *alien_army, int score, int playerX) {
    switch (alien_army->bullet_to_move) {
        case kBulletType_Squiggly:
            if (!Bullet_registry[kBulletType_Squiggly].active) {
                if (AlienArmy_CanFire(alien_army, score, kBulletType_Squiggly)) {
                    AlienArmy_ShootSquiggly(alien_army, playerX);
                }
            }
            alien_army->bullet_to_move = kBulletType_Plunger;
            break;
        case kBulletType_Plunger:
            if (!Bullet_registry[kBulletType_Plunger].active) {
                if (AlienArmy_CanFire(alien_army, score, kBulletType_Plunger)) {
                    AlienArmy_ShootPlunger(alien_army, playerX);
                }
            }
            alien_army->bullet_to_move = kBulletType_Rolling;
            break;
        case kBulletType_Rolling:
            if (!Bullet_registry[kBulletType_Rolling].active) {
                if (AlienArmy_CanFire(alien_army, score, kBulletType_Rolling) || g_game.game_state == kGameState_Demo) {
                    AlienArmy_ShootRolling(alien_army, playerX);
                }
            }
            alien_army->bullet_to_move = kBulletType_Squiggly;
            break;
        default: break;
    }
}

AlienArmyFlag AlienArmy_Move(AlienArmy_t *alien_army, int score, int playerX) {
    int index = (alien_army->last_alien_moved + 1) % 55;
    bool finished = false;
    if (alien_army->aliens_alive <= 0) {
        return kAlienArmyFlag_exterminated;
    }
    if (alien_army->army_state != kAlienArmyState_generating && !alien_army->regenerating) {
        do {
            if (index == 0) {
                if (alien_army->army_state == kAlienArmyState_descendingWaiting) {
                    alien_army->left_x -= 2 * alien_army->direction;
                    alien_army->army_state = kAlienArmyState_descendingTrue;
                    alien_army->direction *= -1;
                } else {
                    alien_army->left_x += 2 * alien_army->direction;
                }
            }
            if (alien_army->aliens[index].active) {
                if (alien_army->army_state == kAlienArmyState_descendingTrue) {
                    Alien_Move(&alien_army->aliens[index], {0, 8});
                    if (alien_army->aliens[index].body.position.y >= 208) return kAlienArmyFlag_invaded;
                }
                Alien_Move(&alien_army->aliens[index], {alien_army->direction*2, 0});
                if (alien_army->aliens[index].body.position.x <= 8 || alien_army->aliens[index].body.position.x >= 208) {
                    alien_army->army_state = kAlienArmyState_descendingWaiting;
                }
            }
            if (alien_army->army_state == kAlienArmyState_descendingTrue && index == 54) {
                alien_army->army_state = kAlienArmyState_descendingFalse;
            }
            alien_army->last_alien_moved++;
            if (alien_army->aliens[index].active) finished = true;
            else index = (index + 1) % 55;
        } while (!finished && alien_army->aliens_alive > 0);
        
        if (g_game.game_state == kGameState_Playing){
            int sound_interval = alien_army->aliens_alive / 2 + 4; // lento con muchos, rapido con pocos
            if (g_tick % sound_interval == 0) {
                g_Soloud.play(g_AlienSounds[alien_army->sound_index]);
                alien_army->sound_index = (alien_army->sound_index + 1) % 4;
            }
        }

        AlienArmy_ControlBullets(alien_army, score, playerX);
    } else {
        if (alien_army->army_state == kAlienArmyState_generating) {
            alien_army->aliens[index].active = true;
            alien_army->last_alien_moved++;
            if (index == 54) alien_army->army_state = kAlienArmyState_descendingFalse;
        } else {
            do {
                index = (alien_army->last_alien_showing + 1) % 55;
                if (alien_army->aliens[index].active) {
                    alien_army->aliens[index].showing = true;
                    finished = true;
                } else {
                    alien_army->last_alien_showing++;
                }
                if (index == 54) alien_army->regenerating = false;
            } while (!finished);
            alien_army->last_alien_showing++;
        }
    }
    return kAlienArmyFlag_none;
}

AlienArmyFlag AlienArmy_Control(AlienArmy_t *alien_army, int score, int playerX) {
    if (alien_army->alien_explosion_tick <= 0) {
        return AlienArmy_Move(alien_army, score, playerX);
    }
    alien_army->alien_explosion_tick--;
    return kAlienArmyFlag_none;
}

void AliensRender(AlienArmy_t *alien_army) {
    for (int i = 0; i < 55; i++) {
        if (alien_army->aliens[i].active && alien_army->aliens[i].showing) Draw(&alien_army->aliens[i].body);
    }
    if (alien_army->alien_explosion_tick > 0) Draw(&alien_army->aliens[alien_army->alien_died].body);
}

int AlienGetPoints(AlienType type) {
    switch (type) {
        case kAlienType_crab: return 20;
        case kAlienType_squid: return 30;
        case kAlienType_octopus: return 10;
    }
    return 0;
}

void Dead(Alien_t *alien) {
    if (alien->active) {
        alien->active = false;

        if (g_game.game_state == kGameState_Playing){
            g_Soloud.play(g_AlienExplosion);
        }

        alien->body.mask = g_assets.AlienExplosionMask;
        AddScore(AlienGetPoints(alien->type));
        AlienArmy_t *alien_army = &g_game.player[g_game.player_playing].alien_army;

        alien_army->aliens_alive--;
        alien_army->alien_explosion_tick = 16;
        // alien_army->alien_died = alien - alien_army->aliens;
        alien_army->alien_died = alien->id;
    }
}