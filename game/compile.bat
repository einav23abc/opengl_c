@echo off

@REM change to batch file's dir
cd /D "%~dp0"

gcc ^
    code/engine_config.c ^
    code/game.c ^
    code/meshes.c ^
    code/shaders.c ^
    code/textures.c ^
    code/sounds.c ^
    code/nine_slices.c ^
    code/fonts.c ^
    code/ui_lists.c ^
    code/alerts.c ^
    code/billboards.c ^
    code/init.c ^
    code/update.c ^
    code/render.c ^
    code/handle_event.c ^
    code/clean.c ^
    -o ../export/a_scaled_affair.exe ^
    -L"../engine" -l "engine" ^
    -l "SDL2main" ^
    -l "SDL2" ^
    -l "opengl32" ^
    -l "SDL2_image" ^
    -l "SDL2_mixer" ^
    -l "libpng16-16" ^
    -g
