@echo off
echo This is an inferior compile sequence. Use "./compile/compile.exe" instead unless "./compile/compile.exe" is not compiling correctly.
gcc ^
    src/engine/glad/glad.c ^
    src/engine/engine.c ^
    src/engine/threads/threads.c ^
    src/engine/audio/audio.c ^
    src/engine/utils/utils.c ^
    src/engine/vec_mat_quat/vec_mat_quat.c ^
    src/engine/cameras/cameras.c ^
    src/engine/shaders/shaders.c ^
    src/engine/textures/textures.c ^
    src/engine/frame_buffer_objects/frame_buffer_objects.c ^
    src/engine/meshes_and_animations/meshes_and_animations.c ^
    src/engine/simple_draw_module/simple_draw_module.c ^
    src/game/game.c ^
    src/game/init.c ^
    src/game/update.c ^
    src/game/render.c ^
    src/game/handle_event.c ^
    src/game/clean.c ^
    -o ./main.exe ^
    -l "mingw32" ^
    -l "SDL2main" ^
    -l "SDL2" ^
    -l "opengl32" ^
    -l "SDL2_image" ^
    -l "SDL2_mixer" ^
    -l "libpng16-16" ^
    -g
