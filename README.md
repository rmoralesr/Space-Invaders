# ORGANIZACIÓN

## TRABAJO EN EQUIPO Y ORGANIZACIÓN EN GRUPO

> [!Caution]
> Este proyecto está hecho para aprender a trabajar en equipo, tenemos que comunicarnos entre nosotros y trabajar juntos, cada uno tiene sus roles y todas las opiniones cuentan por igual.

- Se trabaja por archivos separados, no más de una persona encargada de un archivo.
- Todas las funciones que no estén 100% claras con el nombre deben tener un comentario de `@brief` que explique con pocas palabras su funcionalidad.
- No se debe tocar el código de otras personas sin su consentimiento, incluso si es un bug, simplemente avisa a la persona que se encarga de esa parte.

## RAMAS

- Cada persona trabaja en su propia rama. (ej. features-ivan, features-raul...)
- La rama main no se debe tocar a menos que se sepa que todo lo que está hecho funciona y compila sin errores, y no está a medias.
- Se sube el trabajo a la rama features general cuando al menos no tiene errores de compilación.

## COMPILACION

- Crear el archivo compilacion.bat dentro de una carpeta llamada .compiled para que no llenar main de basura
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

cl /nologo /Zi /GR- /EHs /MD /Fe:..\src\main\main.exe %SOURCES% ^
-I ..\Lib_Graph\ESAT_rev248\include ..\Lib_Graph\ESAT_rev248\bin\ESAT.lib   opengl32.lib user32.lib gdi32.lib shell32.lib Ws2_32.lib

endlocal

@echo ---------------------------------------------------
@echo  Proceso por lotes finalizado.
@echo ---------------------------------------------------



compilacion.bat C: && cd ../src/main && main && cd ../../.compiled

https://archive.org/details/arcade_invaders#