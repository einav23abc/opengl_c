@echo off

@REM change to batch file's dir
cd /D "%~dp0"

gcc ^
    engine_config.c ^
    game.c ^
    init.c ^
    update.c ^
    render.c ^
    handle_event.c ^
    clean.c ^
    -o ../export/main.exe ^
    -L"../engine" -l "engine_soft_debug" ^
    -l "mingw32" ^
    -l "SDL2main" ^
    -l "SDL2" ^
    -l "opengl32" ^
    -l "SDL2_image" ^
    -l "SDL2_mixer" ^
    -l "libpng16-16" ^
    -g
