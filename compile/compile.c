#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

uint64_t get_last_write_time(const char* path)
{
    HANDLE hFile = CreateFile( path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("CreateFile of file:%s failed with %d\n", path, GetLastError());
        return 0;
    }
    
    FILETIME ftWrite;

    // Retrieve the file times for the file.
    if (!GetFileTime( hFile, NULL, NULL, &ftWrite))
    {
        return 0;
    }

    CloseHandle(hFile);

    uint64_t time = ( (uint64_t)ftWrite.dwHighDateTime << 32 ) + ftWrite.dwLowDateTime;

    return time;
}

int main()
{
    char* intended_cwd = "C:\\Users\\einav\\Desktop\\coding\\c\\opengl\\project8\\compile";
    SetCurrentDirectory( intended_cwd );
    char cwd[2048];
    getcwd( cwd, 2048 );
    if( strcmp(cwd, intended_cwd) != 0 )
    {
        printf( "unintended current working directory:\n  \"%s\".\nshould be:\n  \"%s\".\ndid not compile.", cwd, intended_cwd );
        return 1;
    }

    uint64_t wt_engine_c = get_last_write_time( "../src/engine/engine.c" );
    uint64_t wt_engine_h = get_last_write_time( "../src/engine/engine.h" );
    uint64_t wt_engine_types_h = get_last_write_time( "../src/engine/engine_types.h" );
    uint64_t wt_engine_functions_h = get_last_write_time( "../src/engine/engine_functions.h" );

    uint64_t wt_game_c = get_last_write_time( "../src/game/game.c" );
    uint64_t wt_game_h = get_last_write_time( "../src/game/game.h" );
    uint64_t wt_game_types_h = get_last_write_time( "../src/game/game_types.h" );
    uint64_t wt_game_functions_h = get_last_write_time( "../src/game/game_functions.h" );

    uint64_t wt_init_c = get_last_write_time( "../src/game/init.c" );
    uint64_t wt_update_c = get_last_write_time( "../src/game/update.c" );
    uint64_t wt_render_c = get_last_write_time( "../src/game/render.c" );
    uint64_t wt_handle_event_c = get_last_write_time( "../src/game/handle_event.c" );
    uint64_t wt_clean_c = get_last_write_time( "../src/game/clean.c" );
    
    uint64_t last_compile_time = get_last_write_time( "../main.exe" );
    
    if (
        wt_engine_h > last_compile_time ||
        wt_engine_types_h > last_compile_time ||
        wt_engine_functions_h > last_compile_time
    ){
        // recompile all .c files
        printf( "recompiling all\n" );
        system( "gcc ../src/engine/engine.c ../src/game/game.c ../src/game/init.c ../src/game/update.c ../src/game/render.c ../src/game/handle_event.c ../src/game/clean.c -c -g" );
    }else{
        if (
            wt_engine_c > last_compile_time
        ){
            // recompile engine.c
            printf( "recompiling engine.c\n" );
            system( "gcc ../src/engine/engine.c -c -g" );
        }

        if (
            wt_game_h > last_compile_time ||
            wt_game_types_h > last_compile_time ||
            wt_game_functions_h > last_compile_time
        ){
            // recompile all files (except engine.c)
            printf( "recompiling all (except engine.c)\n" );
            system( "gcc ../src/game/game.c ../src/game/init.c ../src/game/update.c ../src/game/render.c ../src/game/handle_event.c ../src/game/clean.c -c -g" );
        }else{
            if (
                wt_game_c > last_compile_time
            ){
                // recompile game.c
                printf( "recompiling game.c\n" );
                system( "gcc ../src/game/game.c -c -g" );
            }
            
            if (
                wt_init_c > last_compile_time
            ){
                // recompile init.c
                printf( "recompiling init.c\n" );
                system( "gcc ../src/game/init.c -c -g" );
            }
            
            if (
                wt_update_c > last_compile_time
            ){
                // recompile update.c
                printf( "recompiling update.c\n" );
                system( "gcc ../src/game/update.c -c -g" );
            }
            
            if (
                wt_render_c > last_compile_time
            ){
                // recompile render.c
                printf( "recompiling render.c\n" );
                system( "gcc ../src/game/render.c -c -g" );
            }
            
            if (
                wt_handle_event_c > last_compile_time
            ){
                // recompile handle_event.c
                printf( "recompiling handle_event.c\n" );
                system( "gcc ../src/game/handle_event.c -c -g" );
            }
            
            if (
                wt_clean_c > last_compile_time
            ){
                // recompile init.c
                printf( "recompiling clean.c\n" );
                system( "gcc ../src/game/clean.c -c -g" );
            }
        }
    }

    // link
    printf( "linking\n" );
    system( "gcc engine.o game.o init.o update.o render.o handle_event.o clean.o -o ../main.exe -l \"mingw32\" -l \"SDL2main\" -l \"SDL2\" -l \"SDL2_image\" -l \"libpng16-16\" -l \"zlib1\" -l \"opengl32\" -l \"glew32\"" );
    
    return 0;
};