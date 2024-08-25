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
    code/spaces.c ^
    code/nine_slices.c ^
    code/fonts.c ^
    code/billboards.c ^
    code/ui_lists.c ^
    code/alerts.c ^
    code/init.c ^
    code/update.c ^
    code/render.c ^
    code/handle_event.c ^
    code/clean.c ^
    code/pages/scrolled_background.c ^
    code/pages/message_page_enter.c ^
    code/pages/in_game/page.c ^
    code/pages/in_game/against_ai.c ^
    code/pages/in_game/against_connected.c ^
    code/pages/in_game/init.c ^
    code/pages/in_game/enter.c ^
    code/pages/in_game/update.c ^
    code/pages/in_game/render.c ^
    code/pages/in_game/mouse_press.c ^
    code/pages/main_menu/enter.c ^
    code/pages/main_menu/render.c ^
    code/pages/how_to_play/render.c ^
    code/pages/play/enter.c ^
    code/pages/play/render.c ^
    code/pages/open_lan/enter.c ^
    code/pages/open_lan/render.c ^
    code/pages/join_lan/page.c ^
    code/pages/join_lan/enter.c ^
    code/pages/join_lan/render.c ^
    code/pages/join_lan/key_press.c ^
    code/pages/joining_game/enter.c ^
    code/pages/joining_game/render.c ^
    code/pages/disconnected_from_host/render.c ^
    code/pages/disconnected_from_client/render.c ^
    -o ../export/a_scaled_affair_debug.exe ^
    -L"../engine" -l "engine_debug" ^
    -l "SDL2main" ^
    -l "SDL2" ^
    -l "opengl32" ^
    -l "SDL2_image" ^
    -l "SDL2_mixer" ^
    -l "libpng16-16" ^
    -l "Ws2_32" ^
    -D DEBUG_MODE ^
    -D DEBUG_SOFT_MODE ^
    -g

    @REM -D TEXTURES_FROM_FILES ^
    @REM -D RESAVE_MESHES ^