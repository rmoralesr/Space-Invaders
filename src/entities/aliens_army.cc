#ifndef ALIENS_ARMY_CC
#define ALIENS_ARMY_CC 1

#include "body.cc"
#include "../core/types.cc"
#include "../tools/asset-loader.cc"

void AlienArmy_Init();
void Destruct(Alien_t* self);

// Definiciones

void AsignAlienMask(Alien_t *alien) {
    alien->body.mask = g_assets.AlienMasks[alien->type][alien->sprite_frame];
}

const int kAlienBulletsTable[] = {1,7,1,1,1,4,11,1,6,3,1,1,11,9,2,8,2,11,4,7,10};

AlienType GetAlienType(int row) {
    if (row < 2) {
        return kAlienType_octopus;
    }
    if (row < 4) {
        return kAlienType_crab;
    }
    return kAlienType_squid;
}

void Alien_Init(Alien_t *alien, int row, int col) {
    alien->type = GetAlienType(row);
    alien->active = false;
    alien->body.can_collide = true;
    alien->body.mask = {};
    alien->body.position.x = col * 16 + 24;
    alien->body.position.y = 136 - 8 - row * 16;
    alien->sprite_frame = 0;
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

bool AlienArmy_CanFire(AlienArmy_t *alien_army, int score, BulletType bullet) {
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
        if (alien_army->aliens[row*11+col].active) return alien_army->aliens[row*11+col].body.position.y;
    }
    return -1;
}

void AlienArmy_ControlBullets(AlienArmy_t *alien_army, int score, int playerX) {
    int y = -1;
    int x = -1;
    switch (alien_army->bullet_to_move) {
        case kBulletType_Squiggly:
            if (Bullet_registry[kBulletType_Squiggly].active) {
                Tick(&Bullet_registry[kBulletType_Squiggly]);
            } else {
                if (AlienArmy_CanFire(alien_army, score, kBulletType_Squiggly)) {
                    y = AlienArmy_PickBulletY(alien_army, kAlienBulletsTable[alien_army->squiggly_table_pos] - 1);
                    x = AlienArmy_PickBulletX(alien_army, playerX, kBulletType_Squiggly);
                    if (y != -1) {
                        Bullet_registry[1] = *Bullet_Construct(kBulletType_Squiggly);
                        Bullet_registry[1].body.mask = g_assets.CharacterMasks[0];
                        MoveTo(&Bullet_registry[1], {x, y+8});
                    }
                }
            }
            alien_army->bullet_to_move = kBulletType_Plunger;
            break;
        case kBulletType_Plunger:
            if (Bullet_registry[kBulletType_Plunger].active) {
                Tick(&Bullet_registry[kBulletType_Plunger]);
            } else {
                if (AlienArmy_CanFire(alien_army, score, kBulletType_Plunger)) {
                    y = AlienArmy_PickBulletY(alien_army, kAlienBulletsTable[alien_army->plunger_table_pos] - 1);
                    x = AlienArmy_PickBulletX(alien_army, playerX, kBulletType_Plunger);
                    if (y != -1) {
                        Bullet_registry[2] = *Bullet_Construct(kBulletType_Plunger);
                        Bullet_registry[2].body.mask = g_assets.CharacterMasks[1];
                        MoveTo(&Bullet_registry[2], {x, y+8});
                    }
                }
            }
            alien_army->bullet_to_move = kBulletType_Rolling;
            break;
        case kBulletType_Rolling:
            if (Bullet_registry[kBulletType_Rolling].active) {
                Tick(&Bullet_registry[kBulletType_Rolling]);
            } else {
                if (AlienArmy_CanFire(alien_army, score, kBulletType_Rolling)) {
                    y = AlienArmy_PickBulletY(alien_army, AlienArmy_GetColNearPlayer(alien_army, playerX));
                    x = AlienArmy_PickBulletX(alien_army, playerX, kBulletType_Rolling);
                    if (y != -1) {
                        Bullet_registry[3] = *Bullet_Construct(kBulletType_Rolling);
                        Bullet_registry[3].body.mask = g_assets.CharacterMasks[2];
                        MoveTo(&Bullet_registry[3], {x, y+8});
                    }
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
    if (alien_army->aliens_alive <= 0) return kAlienArmyFlag_exterminated;
    if (alien_army->army_state != kAlienArmyState_generating) {
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
                    if (alien_army->aliens[index].body.position.y >= 216) return kAlienArmyFlag_invaded;
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
        AlienArmy_ControlBullets(alien_army, score, playerX);
    } else {
        alien_army->aliens[index].active = true;
        alien_army->last_alien_moved++;
        if (index == 54) alien_army->army_state = kAlienArmyState_descendingFalse;
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
        if (alien_army->aliens[i].active) Draw(&alien_army->aliens[i].body);
    }
    if (alien_army->alien_explosion_tick > 0) Draw(&alien_army->aliens[alien_army->alien_died].body);
}

void Destruct(AlienArmy_t* self, int index) {
    if (self->aliens[index].active) {
        self->aliens[index].active = false;
        self->aliens_alive--;
        self->alien_explosion_tick = 16;
        self->alien_died = index;
        self->aliens[index].body.mask = g_assets.AlienExplosionMask;
    }
}

#endif // ALIENS_ARMY_CC