#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

uint64_t get_last_write_time(const char* path) {
    HANDLE hFile = CreateFile( path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("CreateFile of file:%s failed with %d\n", path, GetLastError());
        return 0;
    }
    
    FILETIME ftWrite;

    // Retrieve the file times for the file.
    if (!GetFileTime( hFile, NULL, NULL, &ftWrite)) {
        return 0;
    }

    CloseHandle(hFile);

    uint64_t time = ( (uint64_t)ftWrite.dwHighDateTime << 32 ) + ftWrite.dwLowDateTime;

    return time;
}

int main() {
    char* intended_cwd = "C:\\Users\\einav\\Desktop\\coding\\c\\opengl\\project10\\compile";
    SetCurrentDirectory(intended_cwd);
    char cwd[2048];
    getcwd(cwd, 2048);
    if( strcmp(cwd, intended_cwd) != 0 ) {
        printf("unintended current working directory:\n  \"%s\".\nshould be:\n  \"%s\".\ndid not compile.", cwd, intended_cwd);
        return 1;
    }
    
    uint64_t wt_glad_c = get_last_write_time( "../src/engine/glad/glad.c");
    uint64_t wt_glad_h = get_last_write_time( "../src/engine/glad/glad.h");
    uint64_t wt_glad_khr_h = get_last_write_time( "../src/engine/glad/khrplatform.h");

    uint64_t wt_engine_c = get_last_write_time( "../src/engine/engine.c");
    uint64_t wt_engine_h = get_last_write_time( "../src/engine/engine.h");

    uint64_t wt_config_h = get_last_write_time( "../src/game/config.h");
    
    uint64_t wt_threads_c = get_last_write_time( "../src/engine/threads/threads.c");
    uint64_t wt_threads_h = get_last_write_time( "../src/engine/threads/threads.h");
    
    uint64_t wt_sockets_c = get_last_write_time( "../src/engine/sockets/sockets.c");
    uint64_t wt_sockets_h = get_last_write_time( "../src/engine/sockets/sockets.h");
    
    uint64_t wt_netframe_client_c = get_last_write_time( "../src/engine/netframe/netframe_client.c");
    uint64_t wt_netframe_server_c = get_last_write_time( "../src/engine/netframe/netframe_server.c");
    uint64_t wt_netframe_h = get_last_write_time( "../src/engine/netframe/netframe.h");
    
    uint64_t wt_audio_c = get_last_write_time( "../src/engine/audio/audio.c");
    uint64_t wt_audio_h = get_last_write_time( "../src/engine/audio/audio.h");
    
    uint64_t wt_utils_c = get_last_write_time( "../src/engine/utils/utils.c");
    uint64_t wt_utils_h = get_last_write_time( "../src/engine/utils/utils.h");
    
    uint64_t wt_vmq_c = get_last_write_time( "../src/engine/vec_mat_quat/vec_mat_quat.c");
    uint64_t wt_vmq_h = get_last_write_time( "../src/engine/vec_mat_quat/vec_mat_quat.h");
    
    uint64_t wt_cameras_c = get_last_write_time( "../src/engine/cameras/cameras.c");
    uint64_t wt_cameras_h = get_last_write_time( "../src/engine/cameras/cameras.h");
    
    uint64_t wt_shaders_c = get_last_write_time( "../src/engine/shaders/shaders.c");
    uint64_t wt_shaders_h = get_last_write_time( "../src/engine/shaders/shaders.h");
    
    uint64_t wt_textures_c = get_last_write_time( "../src/engine/textures/textures.c");
    uint64_t wt_textures_h = get_last_write_time( "../src/engine/textures/textures.h");

    uint64_t wt_fbos_c = get_last_write_time( "../src/engine/frame_buffer_objects/frame_buffer_objects.c");
    uint64_t wt_fbos_h = get_last_write_time( "../src/engine/frame_buffer_objects/frame_buffer_objects.h");
    
    uint64_t wt_mesh_anim_c = get_last_write_time( "../src/engine/meshes_and_animations/meshes_and_animations.c");
    uint64_t wt_mesh_anim_h = get_last_write_time( "../src/engine/meshes_and_animations/meshes_and_animations.h");
    
    uint64_t wt_sdm_c = get_last_write_time( "../src/engine/simple_draw_module/simple_draw_module.c");
    uint64_t wt_sdm_h = get_last_write_time( "../src/engine/simple_draw_module/simple_draw_module.h");

    uint64_t wt_game_c = get_last_write_time( "../src/game/game.c");
    uint64_t wt_game_h = get_last_write_time( "../src/game/game.h");

    uint64_t wt_init_c = get_last_write_time( "../src/game/init.c");
    uint64_t wt_update_c = get_last_write_time( "../src/game/update.c");
    uint64_t wt_render_c = get_last_write_time( "../src/game/render.c");
    uint64_t wt_handle_event_c = get_last_write_time( "../src/game/handle_event.c");
    uint64_t wt_clean_c = get_last_write_time( "../src/game/clean.c");
    
    uint64_t last_compile_time = get_last_write_time( "../main.exe");
    
    if (
        wt_glad_h                   > last_compile_time ||
        wt_glad_khr_h               > last_compile_time ||
        wt_engine_h                 > last_compile_time ||
        wt_config_h                 > last_compile_time ||
        wt_threads_h                > last_compile_time ||
        wt_sockets_h                > last_compile_time ||
        wt_netframe_h               > last_compile_time ||
        wt_audio_h                  > last_compile_time ||
        wt_utils_h                  > last_compile_time ||
        wt_vmq_h                    > last_compile_time ||
        wt_cameras_h                > last_compile_time ||
        wt_shaders_h                > last_compile_time ||
        wt_textures_h               > last_compile_time ||
        wt_fbos_h                   > last_compile_time ||
        wt_mesh_anim_h              > last_compile_time ||
        wt_sdm_h                    > last_compile_time
    ){
        // recompile all .c files
        printf("recompiling all\n");
        system(
            "gcc "
            "../src/engine/glad/glad.c "
            "../src/engine/engine.c "
            "../src/engine/threads/threads.c "
            "../src/engine/sockets/sockets.c "
            "../src/engine/netframe/netframe_client.c "
            "../src/engine/netframe/netframe_server.c "
            "../src/engine/audio/audio.c "
            "../src/engine/utils/utils.c "
            "../src/engine/vec_mat_quat/vec_mat_quat.c "
            "../src/engine/cameras/cameras.c "
            "../src/engine/shaders/shaders.c "
            "../src/engine/textures/textures.c "
            "../src/engine/frame_buffer_objects/frame_buffer_objects.c "
            "../src/engine/meshes_and_animations/meshes_and_animations.c "
            "../src/engine/simple_draw_module/simple_draw_module.c "
            "../src/game/game.c "
            "../src/game/init.c "
            "../src/game/update.c "
            "../src/game/render.c "
            "../src/game/handle_event.c "
            "../src/game/clean.c "
            "-include ../src/game/config.h "
            "-c -g"
        );
    }else{
        if (
            wt_glad_c               > last_compile_time ||
            wt_engine_c             > last_compile_time ||
            wt_threads_c            > last_compile_time ||
            wt_sockets_c            > last_compile_time ||
            wt_netframe_client_c    > last_compile_time ||
            wt_netframe_server_c    > last_compile_time ||
            wt_audio_c              > last_compile_time ||
            wt_utils_c              > last_compile_time ||
            wt_vmq_c                > last_compile_time ||
            wt_cameras_c            > last_compile_time ||
            wt_shaders_c            > last_compile_time ||
            wt_textures_c           > last_compile_time ||
            wt_fbos_c               > last_compile_time ||
            wt_mesh_anim_c          > last_compile_time ||
            wt_sdm_c                > last_compile_time
        ){
            // recompile engine
            printf("recompiling engine\n");
            system(
                "gcc "
                "../src/engine/glad/glad.c "
                "../src/engine/engine.c "
                "../src/engine/threads/threads.c "
                "../src/engine/sockets/sockets.c "
                "../src/engine/netframe/netframe_client.c "
                "../src/engine/netframe/netframe_server.c "
                "../src/engine/audio/audio.c "
                "../src/engine/utils/utils.c "
                "../src/engine/vec_mat_quat/vec_mat_quat.c "
                "../src/engine/cameras/cameras.c "
                "../src/engine/shaders/shaders.c "
                "../src/engine/textures/textures.c "
                "../src/engine/frame_buffer_objects/frame_buffer_objects.c "
                "../src/engine/meshes_and_animations/meshes_and_animations.c "
                "../src/engine/simple_draw_module/simple_draw_module.c "
                "-include ../src/game/config.h "
                "-c -g"
            );
        }

        if (
            wt_game_h               > last_compile_time
        ){
            // recompile all game files
            printf("recompiling all game files\n");
            system(
                "gcc "
                "../src/game/game.c "
                "../src/game/init.c "
                "../src/game/update.c "
                "../src/game/render.c "
                "../src/game/handle_event.c "
                "../src/game/clean.c "
                "-include ../src/game/config.h "
                "-c -g"
            );
        }else{
            if (wt_game_c           > last_compile_time){
                // recompile game.c
                printf("recompiling game.c\n");
                system(
                    "gcc "
                    "../src/game/game.c "
                    "-include ../src/game/config.h "
                    "-c -g"
                );
            }
            
            if (wt_init_c           > last_compile_time){
                // recompile init.c
                printf("recompiling init.c\n");
                system(
                    "gcc "
                    "../src/game/init.c "
                    "-include ../src/game/config.h "
                    "-c -g"
                );
            }
            
            if (wt_update_c         > last_compile_time){
                // recompile update.c
                printf("recompiling update.c\n");
                system(
                    "gcc "
                    "../src/game/update.c "
                    "-include ../src/game/config.h "
                    "-c -g"
                );
            }
            
            if (wt_render_c         > last_compile_time){
                // recompile render.c
                printf("recompiling render.c\n");
                system(
                    "gcc "
                    "../src/game/render.c "
                    "-include ../src/game/config.h "
                    "-c -g"
                );
            }
            
            if (wt_handle_event_c   > last_compile_time){
                // recompile handle_event.c
                printf("recompiling handle_event.c\n");
                system(
                    "gcc "
                    "../src/game/handle_event.c "
                    "-include ../src/game/config.h "
                    "-c -g"
                );
            }
            
            if (wt_clean_c          > last_compile_time){
                // recompile clean.c
                printf("recompiling clean.c\n");
                system(
                    "gcc "
                    "../src/game/clean.c "
                    "-include ../src/game/config.h "
                    "-c -g"
                );
            }
        }
    }

    // link
    printf("linking\n");
    system(
        "gcc "
        "glad.o "
        "engine.o "
        "threads.o "
        "sockets.o "
        "netframe_client.o "
        "netframe_server.o "
        "audio.o "
        "utils.o "
        "vec_mat_quat.o "
        "cameras.o "
        "shaders.o "
        "textures.o "
        "frame_buffer_objects.o "
        "meshes_and_animations.o "
        "simple_draw_module.o "
        "game.o "
        "init.o "
        "update.o "
        "render.o "
        "handle_event.o "
        "clean.o "
        "-o ../main.exe "
        "-l \"mingw32\" "
        "-l \"SDL2main\" "
        "-l \"SDL2\" "
        "-l \"opengl32\" "
        "-l \"SDL2_image\" "
        "-l \"SDL2_mixer\" "
        "-l \"libpng16-16\" "
        "-l \"Ws2_32\" "
    );
    
    return 0;
};