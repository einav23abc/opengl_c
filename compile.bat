@echo off
gcc ^
    src/game/engine_config.c ^
    src/game/game.c ^
    src/game/init.c ^
    src/game/update.c ^
    src/game/render.c ^
    src/game/handle_event.c ^
    src/game/clean.c ^
    -o export/main.exe ^
    -L"src/engine" -l "engine" ^
    -l "mingw32" ^
    -l "SDL2main" ^
    -l "SDL2" ^
    -l "opengl32" ^
    -l "SDL2_image" ^
    -l "SDL2_mixer" ^
    -l "libpng16-16" ^
    -g
