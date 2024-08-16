@echo off

echo compiling to libengine.a
gcc ^
    glad/glad.c ^
    engine_config.c ^
    engine.c ^
    threads/threads.c ^
    audio/audio.c ^
    utils/utils.c ^
    vec_mat_quat/vec_mat_quat.c ^
    cameras/cameras.c ^
    shaders/shaders.c ^
    textures/textures.c ^
    frame_buffer_objects/frame_buffer_objects.c ^
    meshes_and_animations/meshes_and_animations.c ^
    simple_draw_module/simple_draw_module.c ^
    -c

ar -rcs libengine.a ./*.o


echo compiling to libengine_soft_debug.a
gcc ^
    glad/glad.c ^
    engine_config.c ^
    engine.c ^
    threads/threads.c ^
    audio/audio.c ^
    utils/utils.c ^
    vec_mat_quat/vec_mat_quat.c ^
    cameras/cameras.c ^
    shaders/shaders.c ^
    textures/textures.c ^
    frame_buffer_objects/frame_buffer_objects.c ^
    meshes_and_animations/meshes_and_animations.c ^
    simple_draw_module/simple_draw_module.c ^
    -D SOFT_DEBUG_MODE ^
    -c ^
    -g

ar -rcs libengine_soft_debug.a ./*.o


echo compiling to libengine_debug.a
gcc ^
    glad/glad.c ^
    engine_config.c ^
    engine.c ^
    threads/threads.c ^
    audio/audio.c ^
    utils/utils.c ^
    vec_mat_quat/vec_mat_quat.c ^
    cameras/cameras.c ^
    shaders/shaders.c ^
    textures/textures.c ^
    frame_buffer_objects/frame_buffer_objects.c ^
    meshes_and_animations/meshes_and_animations.c ^
    simple_draw_module/simple_draw_module.c ^
    -D DEBUG_MODE ^
    -c ^
    -g

ar -rcs libengine_debug.a ./*.o


del .\*.o