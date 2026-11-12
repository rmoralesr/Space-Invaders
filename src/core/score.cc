// AUN NO ESTA TERMINADO

#ifndef SCORE_CC
#define SCORE_CC

#include <stdio.h>

/* CONFIGURACION */
#define SCORE_MAX_PLAYERS 2
#define SCORE_START_LIVES 3
#define SCORE_MAX_LIFE_ICONS 6
#define SCORE_UFO_TABLE_SIZE 16
#define SCORE_POPUP_TICKS 24
#define SCORE_HIGHSCORE_FILE "highscore.dat"

/* TIPOS DE ENEMIGOS */
enum EnemyType {
    ENEMY_SMALL,
    ENEMY_MEDIUM,
    ENEMY_LARGE,
    ENEMY_UFO
};

/* INFORMACION DE PUNTUACION DE CADA JUGADOR */
struct PlayerScore {
    int score;
    int lives;
    int extra_life_given;
    int shot_counter;
};

/* SISTEMA GENERAL DE PUNTUACION */
struct ScoreSystem {
    struct PlayerScore players[SCORE_MAX_PLAYERS];

    int num_players;
    int active_player;
    int high_score;
    int extra_life_at;

    void (*on_extra_life)(void);

    int popup_points;
    int popup_ticks;
    int popup_x;
    int popup_y;
};

/* TABLA DE PUNTOS DEL UFO */
static const int UFO_POINTS[SCORE_UFO_TABLE_SIZE] = {
    100, 50, 50, 100,
    150, 100, 100, 50,
    300, 100, 100, 100,
    50, 150, 100, 50
};

/* CARGAR HI-SCORE DESDE EL ARCHIVO */
int LoadHighScore(void) {
    FILE *file;
    int score = 0;

    file = fopen(SCORE_HIGHSCORE_FILE, "r");

    if (file != NULL) {
        fscanf(file, "%d", &score);
        fclose(file);
    }

    return score;
}

/* GUARDAR HI-SCORE EN EL ARCHIVO */
void ScoreSaveHigh(const struct ScoreSystem *s) {
    FILE *file;

    file = fopen(SCORE_HIGHSCORE_FILE, "w");

    if (file != NULL) {
        fprintf(file, "%d", s->high_score);
        fclose(file);
    }
}

/* REINICIAR LOS DATOS DE LOS JUGADORES */
void ScoreReset(struct ScoreSystem *s) {
    int i;

    for (i = 0; i < SCORE_MAX_PLAYERS; i++) {
        s->players[i].score = 0;
        s->players[i].lives = SCORE_START_LIVES;
        s->players[i].extra_life_given = 0;
        s->players[i].shot_counter = 0;
    }

    s->active_player = 0;

    s->popup_points = 0;
    s->popup_ticks = 0;
    s->popup_x = 0;
    s->popup_y = 0;
}

/* INICIALIZAR EL SISTEMA DE PUNTUACION */
void ScoreInit(
    struct ScoreSystem *s,
    int num_players,
    int extra_life_at,
    void (*on_extra_life)(void)
) {
    if (num_players < 1) {
        num_players = 1;
    }

    if (num_players > SCORE_MAX_PLAYERS) {
        num_players = SCORE_MAX_PLAYERS;
    }

    s->num_players = num_players;
    s->extra_life_at = extra_life_at;
    s->on_extra_life = on_extra_life;
    s->high_score = LoadHighScore();

    ScoreReset(s);
}

/* SUMAR PUNTOS AL JUGADOR ACTIVO */
void ScoreAdd(struct ScoreSystem *s, int points) {
    struct PlayerScore *p;

    p = &s->players[s->active_player];

    p->score += points;

    if (p->score > s->high_score) {
        s->high_score = p->score;
    }

    if (!p->extra_life_given &&
        p->score >= s->extra_life_at) {

        p->lives++;
        p->extra_life_given = 1;

        if (s->on_extra_life != NULL) {
            s->on_extra_life();
        }
    }
}

/* REGISTRAR UN DISPARO DEL JUGADOR */
void ScoreOnPlayerShot(struct ScoreSystem *s) {
    struct PlayerScore *p;

    p = &s->players[s->active_player];

    p->shot_counter++;

    if (p->shot_counter >= SCORE_UFO_TABLE_SIZE) {
        p->shot_counter = 0;
    }
}

/* REINICIAR EL CONTADOR DE DISPAROS */
void ScoreResetShotCounter(struct ScoreSystem *s) {
    s->players[s->active_player].shot_counter = 0;
}

/* REGISTRAR LA DESTRUCCION DE UN UFO */
void ScoreOnUfoKilled(
    struct ScoreSystem *s,
    int x,
    int y
) {
    struct PlayerScore *p;
    int points;

    p = &s->players[s->active_player];

    points = UFO_POINTS[p->shot_counter];

    ScoreAdd(s, points);

    s->popup_points = points;
    s->popup_ticks = SCORE_POPUP_TICKS;
    s->popup_x = x;
    s->popup_y = y;
}

/* OBTENER LOS PUNTOS SEGUN EL TIPO DE ENEMIGO */
int PointsFor(enum EnemyType enemy) {
    switch (enemy) {
        case ENEMY_SMALL:
            return 30;

        case ENEMY_MEDIUM:
            return 20;

        case ENEMY_LARGE:
            return 10;

        default:
            return 0;
    }
}

/* REGISTRAR LA DESTRUCCION DE UN ALIEN */
void ScoreOnEnemyKilled(
    struct ScoreSystem *s,
    enum EnemyType enemy
) {
    int points;

    points = PointsFor(enemy);

    ScoreAdd(s, points);
}

/* ACTUALIZAR EL TEMPORIZADOR DEL POPUP */
void ScoreUpdate(struct ScoreSystem *s) {
    if (s->popup_ticks > 0) {
        s->popup_ticks--;

        if (s->popup_ticks == 0) {
            s->popup_points = 0;
        }
    }
}

/* OBTENER LOS PUNTOS DEL POPUP */
int ScorePopupPoints(const struct ScoreSystem *s) {
    if (s->popup_ticks > 0) {
        return s->popup_points;
    }

    return 0;
}

/* OBTENER LA POSICION X DEL POPUP */
int ScorePopupX(const struct ScoreSystem *s) {
    return s->popup_x;
}

/* OBTENER LA POSICION Y DEL POPUP */
int ScorePopupY(const struct ScoreSystem *s) {
    return s->popup_y;
}

/* RESTAR UNA VIDA AL JUGADOR ACTIVO */
void ScoreLoseLife(struct ScoreSystem *s) {
    struct PlayerScore *p;

    p = &s->players[s->active_player];

    if (p->lives > 0) {
        p->lives--;
    }
}

/* CAMBIAR EL JUGADOR ACTIVO */
void ScoreSetActivePlayer(
    struct ScoreSystem *s,
    int player
) {
    if (player >= 0 && player < s->num_players) {
        s->active_player = player;
    }
}

/* OBTENER LA PUNTUACION DEL JUGADOR ACTIVO */
int ScoreGet(const struct ScoreSystem *s) {
    return s->players[s->active_player].score;
}

/* OBTENER EL HI-SCORE */
int ScoreGetHigh(const struct ScoreSystem *s) {
    return s->high_score;
}

/* OBTENER LAS VIDAS DEL JUGADOR ACTIVO */
int ScoreGetLives(const struct ScoreSystem *s) {
    return s->players[s->active_player].lives;
}

/* OBTENER EL NUMERO DE ICONOS DE VIDA QUE SE DIBUJAN */
int ScoreLivesToDraw(const struct ScoreSystem *s) {
    int lives;

    lives = ScoreGetLives(s);

    if (lives > SCORE_MAX_LIFE_ICONS) {
        return SCORE_MAX_LIFE_ICONS;
    }

    return lives;
}

#endif

