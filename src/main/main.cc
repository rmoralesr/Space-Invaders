@REM Compilación y Enlace con biblioteca gráfica.
@cls
@echo ---------------------------------------------------
@echo  ESAT Curso 2025-2026 Asignatura PRG Primero
@echo ---------------------------------------------------
@echo  Proceso por lotes iniciado.
@echo ---------------------------------------------------
@echo off

setlocal enabledelayedexpansion

set "SOURCES="

for /r "..\src" %%f in (*cc) do (
    set "SOURCES=!SOURCES! %%f"
)

cl /nologo /Zi /GR- /EHs /MD %SOURCES% -I ..\Lib_Graph\ESAT_rev248\include ..\Lib_Graph\ESAT_rev248\bin\ESAT.lib   opengl32.lib user32.lib gdi32.lib shell32.lib Ws2_32.lib

endlocal

@echo ---------------------------------------------------
@echo  Proceso por lotes finalizado.
@echo ---------------------------------------------------

#include <esat/window.h>
#include <esat/draw.h>
#include <esat/input.h>
#include <esat/sprite.h>
#include <esat/time.h>

// LOS INCLUDES DEBEN ESTAR EN ORDEN, LOS QUE NECESITAN OTROS QUE SE PONGAN DEBAJO DE ELLOS
#include "../core/cirno.hh"
#include "../core/types.cc"
#include "../core/mask.hh"
#include "../entities/body.hh"
#include "../tools/ui-element.cc"
#include "update.cc"
#include "render.cc"

/** @brief Controla el maximo de frames por segundo */
void FrameWait(double last_time);


/** @brief Inicializa las variables necesarias para el juego */
void GameInit(Game_t *game) {
    // TODO
    
    game->credits = 0;
    game->player_playing = 0;

    LoadAssets();
    GenerateLives(3);
    GenerateText("score<1> hi-score score<2>", {10,9});
    GenerateNumber(game->player[0].score,4, kScore1NumberPosition);
    GenerateNumber(game->player[1].score,4, kScore2NumberPosition);
    GenerateNumber(game->hiscore,4,kHiScoreNumberPosition);
    GenerateText("credit", {140,240});
    GenerateNumber(game->credits,2, kCreditsNumberPosition);

    for (int i = 0; i < 2; i++){
        Player_Construct(&game->player[i]);
    }

    game->game_state = kGameState_InsertCoin;
    PreviewingInit(game);
}


int esat::main(int argc, char **argv) {
    esat::WindowInit(kWindowSize.x*kPixelScale, kWindowSize.y*kPixelScale);
    WindowSetMouseVisibility(true);

    GameInit(&g_game);

    while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
        double last_time = esat::Time();

        GameUpdate(&g_game);

        esat::DrawBegin();
        esat::DrawClear(0,0,0);
        esat::DrawSetStrokeColor(0,0,0,0);

        GameRender(&g_game);

        esat::DrawEnd();
        esat::WindowFrame();

        FrameWait(last_time);
    }

    esat::SpriteRelease(g_assets.ColorMask);

    esat::WindowDestroy();
    return 0;
}


void FrameWait(double last_time) {
    double current_time;
    do{
        current_time = esat::Time();
    } while(current_time - last_time <= 1000.0 / kTargetFps);
}