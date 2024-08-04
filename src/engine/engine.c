//  compile with:
//      ./compile/compile.exe
//  or with:
//      ./compile.bat
//  or with:
//      cd project8
//      gcc src/engine/engine.c {game .c files} -o ./main.exe -l "mingw32" -l "SDL2main" -l "SDL2" -l "SDL2_image" -l "libpng16-16" -l "zlib1" -l "opengl32" -l "glew32"

//  to do:
//      simple draw functions (rectangles,lines,points)
//      ui system

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#include "engine_types.h"
#include "engine_functions.h"

#define DEBUG_MODE

// <externs>
    extern __attribute__((weak)) const uint32_t ENGINE_CONFIG_OUTPORT_WIDTH;
    extern __attribute__((weak)) const uint32_t ENGINE_CONFIG_OUTPORT_HEIGHT;

    extern __attribute__((weak)) const uint32_t ENGINE_CONFIG_WINDOW_START_WIDTH;
    extern __attribute__((weak)) const uint32_t ENGINE_CONFIG_WINDOW_START_HEIGHT;

    extern __attribute__((weak)) const float ENGINE_CONFIG_TARGET_FRAME_DELAY;

    extern __attribute__((weak)) const float ENGINE_CONFIG_DEFAULT_BACKGROUND_COLOR_R;
    extern __attribute__((weak)) const float ENGINE_CONFIG_DEFAULT_BACKGROUND_COLOR_G;
    extern __attribute__((weak)) const float ENGINE_CONFIG_DEFAULT_BACKGROUND_COLOR_B;

    extern __attribute__((weak)) const float ENGINE_CONFIG_DEFAULT_PILLARBOX_COLOR_R;
    extern __attribute__((weak)) const float ENGINE_CONFIG_DEFAULT_PILLARBOX_COLOR_G;
    extern __attribute__((weak)) const float ENGINE_CONFIG_DEFAULT_PILLARBOX_COLOR_B;

    __attribute__((weak)) uint32_t game_init();
    __attribute__((weak)) void game_update();
    __attribute__((weak)) void game_render();
    __attribute__((weak)) void game_handle_event();
    __attribute__((weak)) void game_clean();
// </externs>

// <variables declaration>
    
    // <set by engine config>
        const uint32_t OUTPORT_WIDTH;
        const uint32_t OUTPORT_HEIGHT;
        const uint32_t OUTPORT_VIEWPORT_SIZE;
        const int32_t OUTPORT_VIEWPORT_X;
        const int32_t OUTPORT_VIEWPORT_Y;

        const uint32_t WINDOW_START_WIDTH;
        const uint32_t WINDOW_START_HEIGHT;

        const float DEFAULT_BACKGROUND_COLOR_R;
        const float DEFAULT_BACKGROUND_COLOR_G;
        const float DEFAULT_BACKGROUND_COLOR_B;

        const float DEFAULT_PILLARBOX_COLOR_R;
        const float DEFAULT_PILLARBOX_COLOR_G;
        const float DEFAULT_PILLARBOX_COLOR_B;

        const float TARGET_FRAME_DELAY;
    // </set by engine config>

    const SDL_Event event;

    uint8_t keys[SDL_NUM_SCANCODES]; // time since key pressed; 0 if released

    uint8_t running;
    uint32_t frame_start_time;
    const uint32_t delta_time;
    const float delta_frames;
    uint32_t acum_frames_time;
    uint32_t acum_frames_amount;

    SDL_Window* window;
    int32_t window_width;
    int32_t window_height;
    fbo_t* window_fbo;

    const SDL_GLContext context;
    
    const fbo_t* outport_fbo;
    const shader_t* screen_quad_mesh_shader;
    const mesh_t* screen_quad_mesh;
    
    const shader_t* default_shader;

    const texture_t* default_texture;

    #define _SHADERS_MAX_AMOUNT_ (128)
    uint64_t shaders_amount = 0;
    int64_t current_shader = -1;
    shader_t* shaders_list[_SHADERS_MAX_AMOUNT_];

    #define _CAMERAS_MAX_AMOUNT_ (128)
    uint64_t cameras_amount = 0;
    int64_t current_camera = -1;
    camera_t* cameras_list[_CAMERAS_MAX_AMOUNT_];

    #define _TEXTURES_MAX_AMOUNT_ (128)
    uint64_t textures_amount = 0;
    texture_t* textures_list[_TEXTURES_MAX_AMOUNT_];

    #define _MESHES_MAX_AMOUNT_ (128)
    uint64_t meshes_amount = 0;
    mesh_t* meshes_list[_MESHES_MAX_AMOUNT_];

    #define _ANIMATIONS_MAX_AMOUNT_ (128)
    uint64_t animations_amount = 0;
    animation_t* animations_list[_ANIMATIONS_MAX_AMOUNT_];

    #define _FBOS_MAX_AMOUNT_ (128)
    uint64_t fbos_amount = 0;
    uint64_t current_fbo = 0;
    fbo_t* fbos_list[_FBOS_MAX_AMOUNT_];

    // <simple draw module>
    float simple_draw_module_color_r;
    float simple_draw_module_color_g;
    float simple_draw_module_color_b;
    float simple_draw_module_color_a;
    const shader_t* simple_draw_module_cube_shader;
    const mesh_t* simple_draw_module_rectangle_mesh;
    const mesh_t* simple_draw_module_cube_mesh;
    const mesh_t* simple_draw_module_ball_mesh;
    // </simple draw module>

// </variables declaration>

// <constants>
    const uint64_t SHADERS_MAX_AMOUNT = _SHADERS_MAX_AMOUNT_;
    const uint64_t CAMERAS_MAX_AMOUNT = _CAMERAS_MAX_AMOUNT_;
    const uint64_t TEXTURES_MAX_AMOUNT = _TEXTURES_MAX_AMOUNT_;
    const uint64_t MESHES_MAX_AMOUNT = _MESHES_MAX_AMOUNT_;
    const uint64_t ANIMATIONS_MAX_AMOUNT = _ANIMATIONS_MAX_AMOUNT_;
    const uint64_t FBOS_MAX_AMOUNT = _FBOS_MAX_AMOUNT_;
// </constants>

int32_t main(int32_t argc, char** argv) {
    if (game_init == NULL){
        printf("game_init() does not exist\n");
    }
    if (game_render == NULL){
        printf("game_render() does not exist\n");
    }
    if (game_update == NULL){
        printf("game_update() does not exist\n");
    }
    if (game_handle_event == NULL){
        printf("game_handle() does not exist\n");
    }
    if (game_clean == NULL){
        printf("game_clean() does not exist\n");
    }

    
    uint32_t init_result = init();
    if (init_result != 0){
        return init_result;
    }
    
    *((uint32_t*)&delta_time) = 0;
    acum_frames_time = 0;
    acum_frames_amount = 0;

    running = 1;
    while(running){
        *((float*)&delta_frames) = ((float)delta_time)/TARGET_FRAME_DELAY;

        frame_start_time = SDL_GetTicks();

        while (SDL_PollEvent((SDL_Event*)&event)){
            handle_event();
        }
        update();
        render();

        *((uint32_t*)&delta_time) = SDL_GetTicks() - frame_start_time;
        
        #ifdef DEBUG_MODE
        acum_frames_amount += 1;
        acum_frames_time += delta_time;
        if (acum_frames_time >= 1000){
            printf("Avrage fps in the last seccond:%d\n", 1000*acum_frames_amount/acum_frames_time);
            acum_frames_amount = 0;
            acum_frames_time = 0;
        };
        #endif
    }
    
    clean();

    #ifdef DEBUG_MODE
        printf("ended sucessfully");
    #endif
    return 0;
}

// <functions>
    // <miscellaneous>
        uint32_t hash(uint32_t x) {
            x ^= x >> 16;
            x *= 0x7feb352dU;
            x ^= x >> 15;
            x *= 0x846ca68bU;
            x ^= x >> 16;
            return x;
        }
        uint32_t uintmin(uint32_t a, uint32_t b) {
            if (a < b){
                return a;
            }
            return b;
        }
        uint32_t uintmax(uint32_t a, uint32_t b) {
            if (a > b){
                return a;
            }
            return b;
        }
        int32_t intmin(int32_t a, int32_t b) {
            if (a < b){
                return a;
            }
            return b;
        }
        int32_t intmax(int32_t a, int32_t b) {
            if (a > b){
                return a;
            }
            return b;
        }
        int64_t int64min(int64_t a, int64_t b) {
            if (a < b){
                return a;
            }
            return b;
        }
        int64_t int64max(int64_t a, int64_t b) {
            if (a > b){
                return a;
            }
            return b;
        }
        uint64_t load_file_contents(char** load_to, const char* file_path) {
            FILE* file_pointer = fopen(file_path, "r");
            if (file_pointer == NULL){
                printf("failed to open:\"%s\".\n", file_path);
                
                return 0;
            };

            uint32_t file_size = 0;

            //Getting File Size
            fseek(file_pointer, 0, SEEK_END);
            file_size = ftell(file_pointer);
            rewind(file_pointer);

            //Reading From File
            *load_to = (char*)malloc(sizeof(char) * (file_size+1));
            if (*load_to == NULL){
                printf("malloc of size %u failed\n", file_size+1);
                return 1;
            };
            uint64_t read_count = fread(*load_to, sizeof(char), file_size, file_pointer);
            (*load_to)[read_count] = '\0';
            fclose(file_pointer);

            return read_count;
        }
        int64_t str_find_substr(char* str, char* substr) {
            uint64_t i = 0;
            uint64_t j = 0;
            char ch = str[0];
            while (ch != '\0') {
                j = 0;
                while (ch == substr[j]) {
                    j += 1;
                    ch = str[i+j];
                    if (substr[j] == '\0'){
                        return i;
                    }
                    if (ch == '\0'){
                        return -1;
                    }
                }
                
                i += 1;
                ch = str[i];
            }

            return -1;
        }
        // assumes str is numbers with spaces in between
        // array needs to be freed at the end of use
        float* str_to_float_array(char* str, uint64_t arr_size) {
            uint64_t i = 0;
            float* arr = malloc(sizeof(float)*arr_size);
            if (arr == NULL){
                return NULL;
            }
            for (uint64_t j = 0; j < arr_size; j++) {
                arr[j] = atof(&(str[i]));
                int64_t di = str_find_substr(&(str[i+1]), " ");
                if (di == -1){
                    return arr;
                }
                i += di+1;
            }
            return arr;
        }
        // assumes str is numbers with spaces in between
        // array needs to be freed at the end of use
        int32_t* str_to_int_array(char* str, uint64_t arr_size) {
            uint64_t i = 0;
            int32_t* arr = calloc(1, sizeof(int32_t)*arr_size);
            if (arr == NULL){
                return NULL;
            }
            for (uint64_t j = 0; j < arr_size; j++) {
                arr[j] = atoi(&(str[i]));
                int64_t di = str_find_substr(&(str[i+1]), " ");
                if (di == -1){
                    return arr;
                }
                i += di+1;
            }
            return arr;
        }
        // assumes str is numbers with spaces in between
        // array needs to be bigger/equal to floats_amount
        void str_to_existing_float_array(char* str, uint64_t floats_amount, float* arr) {
            uint64_t i = 0;
            for (uint64_t j = 0; j < floats_amount; j++) {
                arr[j] = atof(&(str[i]));
                int64_t di = str_find_substr(&(str[i+1]), " \0");
                if (di = -1){
                    return;
                }
                i += di;
            }
            return;
        }
    // </miscellaneous>

    // <app>
        uint32_t init() {
            // <engine config inputs>
                // <OUTPORT_WIDTH>
                    if (&ENGINE_CONFIG_OUTPORT_WIDTH == NULL){
                        *((uint32_t*)(&OUTPORT_WIDTH)) = 320;
                    }else{
                        *((uint32_t*)(&OUTPORT_WIDTH)) = ENGINE_CONFIG_OUTPORT_WIDTH;
                    }
                // </OUTPORT_WIDTH>

                // <OUTPORT_HEIGHT>
                    if (&ENGINE_CONFIG_OUTPORT_HEIGHT == NULL){
                        *((uint32_t*)(&OUTPORT_HEIGHT)) = 180;
                    }else{
                        *((uint32_t*)(&OUTPORT_HEIGHT)) = ENGINE_CONFIG_OUTPORT_HEIGHT;
                    }
                // </OUTPORT_HEIGHT>


                // <WINDOW_START_WIDTH>
                    if (&ENGINE_CONFIG_WINDOW_START_WIDTH == NULL){
                        *((uint32_t*)(&WINDOW_START_WIDTH)) = 320;
                    }else{
                        *((uint32_t*)(&WINDOW_START_WIDTH)) = ENGINE_CONFIG_WINDOW_START_WIDTH;
                    }
                // </WINDOW_START_WIDTH>

                // <WINDOW_START_HEIGHT>
                    if (&ENGINE_CONFIG_WINDOW_START_HEIGHT == NULL){
                        *((uint32_t*)(&WINDOW_START_HEIGHT)) = 180;
                    }else{
                        *((uint32_t*)(&WINDOW_START_HEIGHT)) = ENGINE_CONFIG_WINDOW_START_HEIGHT;
                    }
                // </WINDOW_START_HEIGHT>

                // <TARGET_FRAME_DELAY>
                    if (&ENGINE_CONFIG_TARGET_FRAME_DELAY == NULL){
                        *((float*)(&TARGET_FRAME_DELAY)) = (1000/60);
                    }else{
                        *((float*)(&TARGET_FRAME_DELAY)) = ENGINE_CONFIG_TARGET_FRAME_DELAY;
                    }
                // </TARGET_FRAME_DELAY>

                // <DEFAULT_BACKGROUND_COLOR>
                    if (&ENGINE_CONFIG_DEFAULT_BACKGROUND_COLOR_R == NULL){
                        *((float*)(&DEFAULT_BACKGROUND_COLOR_R)) = 0.2;
                    }else{
                        *((float*)(&DEFAULT_BACKGROUND_COLOR_R)) = ENGINE_CONFIG_DEFAULT_BACKGROUND_COLOR_R;
                    }
                    if (&ENGINE_CONFIG_DEFAULT_BACKGROUND_COLOR_G == NULL){
                        *((float*)(&DEFAULT_BACKGROUND_COLOR_G)) = 0.2;
                    }else{
                        *((float*)(&DEFAULT_BACKGROUND_COLOR_G)) = ENGINE_CONFIG_DEFAULT_BACKGROUND_COLOR_G;
                    }
                    if (&ENGINE_CONFIG_DEFAULT_BACKGROUND_COLOR_B == NULL){
                        *((float*)(&DEFAULT_BACKGROUND_COLOR_B)) = 0.2;
                    }else{
                        *((float*)(&DEFAULT_BACKGROUND_COLOR_B)) = ENGINE_CONFIG_DEFAULT_BACKGROUND_COLOR_B;
                    }
                // </DEFAULT_BACKGROUND_COLOR>

                // <DEFAULT_PILLARBOX_COLOR>
                    if (&ENGINE_CONFIG_DEFAULT_PILLARBOX_COLOR_R == NULL){
                        *((float*)(&DEFAULT_PILLARBOX_COLOR_R)) = 0.1;
                    }else{
                        *((float*)(&DEFAULT_PILLARBOX_COLOR_R)) = ENGINE_CONFIG_DEFAULT_PILLARBOX_COLOR_R;
                    }
                    if (&ENGINE_CONFIG_DEFAULT_PILLARBOX_COLOR_G == NULL){
                        *((float*)(&DEFAULT_PILLARBOX_COLOR_G)) = 0.1;
                    }else{
                        *((float*)(&DEFAULT_PILLARBOX_COLOR_G)) = ENGINE_CONFIG_DEFAULT_PILLARBOX_COLOR_G;
                    }
                    if (&ENGINE_CONFIG_DEFAULT_PILLARBOX_COLOR_B == NULL){
                        *((float*)(&DEFAULT_PILLARBOX_COLOR_B)) = 0.1;
                    }else{
                        *((float*)(&DEFAULT_PILLARBOX_COLOR_B)) = ENGINE_CONFIG_DEFAULT_PILLARBOX_COLOR_B;
                    }
                // </DEFAULT_PILLARBOX_COLOR>
            // </engine config inputs>

            // <SDL,opengl,glew,window setup>
                // <init SDL>
                    if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_VIDEO) != 0) {
                        printf("Failed to initialize SDL\n");
                        return 1;
                    }
                // </init SDL>

                // <set context attributes>
                    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) != 0) {
                        printf("error\n");
                    }
                    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3) != 0) {
                        printf("error\n");
                    }
                    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0) {
                        printf("error\n");
                    }
                    if(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0) {
                        printf("error\n");
                    }
                // </set context attributes>

                // <create window>
                    window = SDL_CreateWindow(
                        "SDL App",
                        100,
                        100,
                        WINDOW_START_WIDTH,
                        WINDOW_START_HEIGHT,
                        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
                   );
                    if (window == NULL) {
                        SDL_Quit();
                        printf("Failed to create window\n");
                        return 1;
                    }
                    window_width = WINDOW_START_WIDTH;
                    window_height = WINDOW_START_HEIGHT;
                // </create window>

                // <init rendering context>
                    *((SDL_GLContext*)&context) = SDL_GL_CreateContext(window);
                    if (context == NULL) {
                        printf("Failed to create GL context\n");
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        return 1;
                    }
                // </init rendering context>

                // <init glew>
                    GLenum err;
                    glewExperimental = GL_TRUE; // Please expose OpenGL 3.x+ interfaces
                    err = glewInit();
                    if (err != GLEW_OK) {
                        printf("Failed to init GLEW\n");
                        SDL_GL_DeleteContext(context);
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        return 1;
                    }
                // </init glew>
            // </SDL,opengl,glew,window setup>

            // <window frame buffer object>
                if(1){
                window_fbo = malloc(sizeof(fbo_t));
                if (window_fbo == NULL) {
                    return -1;
                }
                
                window_fbo->fbo_id = 0;
                window_fbo->color_status = 0;
                window_fbo->depth_stencil_status = 0;
                
                window_fbo->width = 0;
                window_fbo->height = 0;
                window_fbo->color = 0;
                window_fbo->color_format = 0;
                window_fbo->depth_stencil = 0;

                *((uint64_t*)&window_fbo->fbo_index) = fbos_amount;
                fbos_list[fbos_amount] = window_fbo;
                fbos_amount += 1;
                }
            // </window frame buffer object>

            // <outport frame buffer>
                outport_fbo = create_fbo(   OUTPORT_WIDTH, OUTPORT_HEIGHT,
                                            1, GL_RGB, NULL, 0,
                                            4, NULL, 0);
            // </outport frame buffer>

            // <screen_quad_mesh_shader>
                // used along with screen_quad_mesh to draw a texture to a section of the screen
                screen_quad_mesh_shader = shader_create_from_files(
                    "./src/engine/screen_quad_mesh.vert",
                    "./src/engine/screen_quad_mesh.frag",
                    "in_vertex_data", 1,
                    "", 0
               );
                if (screen_quad_mesh_shader == NULL){
                    return 1;
                }
            // </screen_quad_mesh_shader>

            // <screen_quad_mesh>
                // used along with screen_quad_mesh_shader to draw a texture to a section of the screen
                if(1){
                float vertices_data_arr[] = {
                    0,0,
                    0,1,
                    1,1,
                    1,0
                };

                vbo_data_t vbo_datas_arr[1] = {
                    {
                        .data_arr_size = sizeof(vertices_data_arr),
                        .data_arr = (void*)vertices_data_arr,
                        .size = 2,
                        .type = GL_FLOAT,
                        .stride = 2*sizeof(float),
                        .divisor = 0
                    }
                };

                uint32_t indices_array[] = {
                    1, 0, 2,
                    2, 0, 3
                };

                screen_quad_mesh = generate_mesh(vbo_datas_arr, 1, indices_array, 6);
                }
            // </screen_quad_mesh>

            // <default_shader>
                default_shader = shader_create_from_files(
                    "./src/engine/default.vert",
                    "./src/engine/default.frag",
                    "in_vertex_position\0in_vertex_texcoord", 2,
                    "", 0
               );
                if (default_shader == NULL){
                    return 1;
                }
            // </default_shader>

            // <default_texture>
                default_texture = load_texture("./src/engine/def_tex.png");
            // </default_texture>
            
            // <simple draw module>
                simple_draw_module_set_color(1, 1, 1, 1);

                // <simple_draw_module_cube_shader>
                    simple_draw_module_cube_shader = shader_create_from_files(
                        "./src/engine/simple_draw_module/cube.vert",
                        "./src/engine/simple_draw_module/cube.frag",
                        "in_vertex_position\0in_vertex_texcoord", 2,
                        "u_position\0u_size\0u_color", 3
                   );
                // </simple_draw_module_cube_shader>

                // <simple_draw_module_rectangle_mesh>
                    if(1){
                    float vertices_position_arr[] = {
                        0,0,0,
                        0,1,0,
                        1,1,0,
                        1,0,0
                    };

                    vbo_data_t vbo_datas_arr[1] = {
                        {
                            .data_arr_size = sizeof(vertices_position_arr),
                            .data_arr = (void*)vertices_position_arr,
                            .size = 3,
                            .type = GL_FLOAT,
                            .stride = 3*sizeof(float),
                            .divisor = 0
                        }
                    };

                    uint32_t indices_array[] = {
                        0, 1, 2,
                        0, 2, 3
                    };

                    simple_draw_module_rectangle_mesh = generate_mesh(vbo_datas_arr, 1, indices_array, 6);
                    }
                // </simple_draw_module_rectangle_mesh>

                // <simple_draw_module_cube_mesh>
                    if(1){
                    float vertices_position_arr[] = {
                        0,0,0,
                        1,0,0,
                        0,1,0,
                        1,1,0,

                        0,0,1,
                        1,0,1,
                        0,1,1,
                        1,1,1
                    };

                    vbo_data_t vbo_datas_arr[1] = {
                        {
                            .data_arr_size = sizeof(vertices_position_arr),
                            .data_arr = (void*)vertices_position_arr,
                            .size = 3,
                            .type = GL_FLOAT,
                            .stride = 3*sizeof(float),
                            .divisor = 0
                        }
                    };

                    uint32_t indices_array[] = {
                        //Top
                        2, 6, 7,
                        2, 3, 7,
                        //Bottom
                        0, 4, 5,
                        0, 1, 5,
                        //Left
                        0, 2, 6,
                        0, 4, 6,
                        //Right
                        1, 3, 7,
                        1, 5, 7,
                        //Front
                        0, 2, 3,
                        0, 1, 3,
                        //Back
                        4, 6, 7,
                        4, 5, 7
                    };

                    simple_draw_module_cube_mesh = generate_mesh(vbo_datas_arr, 1, indices_array, 6*6);
                    }
                // </simple_draw_module_cube_mesh>

                // <simple_draw_module_ball_mesh>
                    if(1){
                    simple_draw_module_ball_mesh = mesh_generate_ball(4);
                    }
                // </simple_draw_module_cube_mesh>
            // </simple draw module>

            // <game_init>
                if (game_init != NULL) {
                    uint32_t game_init_res = game_init();
                    if (game_init_res != 0) {
                        printf("game_init() failed with %u\n", game_init_res);
                        clean();
                        return game_init_res;
                    }
                }
            // </game_init>

            return 0;
        }
        
        void handle_event() {
            SDL_Scancode scancode;

            switch(event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    scancode = event.key.keysym.scancode;
                    if (scancode < SDL_NUM_SCANCODES && keys[scancode] == 0) {
                        keys[scancode] = 1;
                    }
                    break;
                case SDL_KEYUP:
                    scancode = event.key.keysym.scancode;
                    if (scancode < SDL_NUM_SCANCODES) {
                        keys[scancode] = 0;
                    }
                    break;
                case SDL_WINDOWEVENT:
                    switch(event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            SDL_GetWindowSize(window, &window_width, &window_height);
                            break;
                    }
                    break;
            }
            
            // <game_handle_event>
                if (game_handle_event != NULL)
                    game_handle_event();
            // </game_handle_event>
            return;
        }

        void update() {
            // <game_update>
                if (game_update != NULL)
                    game_update();
            // </game_update>
            
            // <keys>
                // count time since press
                for (int32_t i = 0; i < SDL_NUM_SCANCODES; i++) {
                    if (keys[i]) {
                        keys[i]++;
                        // avoid integer overflow
                        if (keys[i] == 0) {
                            keys[i]--;
                        }
                    }
                }
            // </keys>
            return;
        }

        /*  Gets called by the function {main} for every frame
        
            Calls the {game_render} function (if exists) after:
            1. The frame buffer was set to {outport_fbo}
            2. The color buffer and depth buffer have been cleared
            3. The shader was set to {default_shader}
            4. Every active camera's wvp_mat was updated
            5. Depth test set to {less}
            6. Back faces culled and face culling enabled
            
            After the {game_render} function was called:
            1. The {outport_fbo} frame buffer will be drawn to the screen
            2. The {current_camera} will be set to -1 */
        void render() {
            use_fbo((fbo_t*)outport_fbo);

            // clear outport fbo
            glClearColor(DEFAULT_BACKGROUND_COLOR_R, DEFAULT_BACKGROUND_COLOR_G, DEFAULT_BACKGROUND_COLOR_B, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glDepthMask(GL_TRUE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);


            // set all shaders to have not been updated by any camera yet
            for (uint64_t s = 0; s < shaders_amount; s++) {
                shaders_list[s]->wvp_mat_camera_index = -1;
            }

            // <update every active camera's world_view_projection_matrix>
            for (uint64_t c = 0; c < cameras_amount; c++) {
                // skip non active cameras
                if (cameras_list[c]->active == 0) {
                    continue;
                }
                
                camera_update_wvp_mat(cameras_list[c]);
            }
            
            shader_use((shader_t*)default_shader);
            
            // <game_render>
                if (game_render != NULL)
                    game_render();
            // </game_render>
            
            current_camera = -1; // not in render period
            // </render for each camera>
            
            // <draw outport frame buffer to screen>
                use_fbo(window_fbo);
                uint32_t pixel_scale = uintmin(window_width/OUTPORT_WIDTH , window_height/OUTPORT_HEIGHT);
                uint32_t w = OUTPORT_WIDTH*pixel_scale;
                uint32_t h = OUTPORT_HEIGHT*pixel_scale;
                glViewport((window_width-w)*0.5,(window_height-h)*0.5,w,h);

                glClearColor(DEFAULT_PILLARBOX_COLOR_R, DEFAULT_PILLARBOX_COLOR_G, DEFAULT_PILLARBOX_COLOR_B, 1.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

                glCullFace(GL_BACK);
                glDisable(GL_DEPTH_TEST);
                shader_use((shader_t*)screen_quad_mesh_shader);
                bind_fbo_color_texture((fbo_t*)outport_fbo, screen_quad_mesh_shader->u_texture_loc, 0);
                draw_mesh((mesh_t*)screen_quad_mesh);
                glEnable(GL_DEPTH_TEST);
            // </draw outport frame buffer to screen>
            
            // show drawn image
            SDL_GL_SwapWindow(window);
            return;
        }
        
        void clean() {
            // <game clean>
                if (game_clean)
                    game_clean();
            // </game clean>
            
            // <clean shaders>
                printf("cleaning %u shaders\n", shaders_amount);
                for (uint64_t i = 0; i < shaders_amount; i++) {
                    shader_clean(shaders_list[i]);
                }
            // </clean shaders>
            
            // <clean textures>
                printf("cleaning %u textures\n", textures_amount);
                for (uint64_t i = 0; i < textures_amount; i++) {
                    clean_texture(textures_list[i]);
                }
            // </clean textures>

            // <clean meshes>
                printf("cleaning %u meshes\n", meshes_amount);
                for (uint64_t i = 0; i < meshes_amount; i++) {
                    clean_mesh(meshes_list[i]);
                }
            // </clean meshes>

            // <clean animations>
                printf("cleaning %u animations\n", animations_amount);
                for (uint64_t i = 0; i < animations_amount; i++) {
                    clean_animation(animations_list[i]);
                }
            // </clean animations>

            // <clean cameras>
                printf("cleaning %u cameras\n", cameras_amount);
                for (uint64_t i = 0; i < cameras_amount; i++) {
                    camera_clean(cameras_list[i]);
                }
            // </clean cameras>

            // <clean fbos>
                printf("cleaning %u fbos\n", fbos_amount);
                for (uint64_t i = 0; i < fbos_amount; i++) {
                    clean_fbo(fbos_list[i]);
                }
            // </clean fbos>

            SDL_GL_DeleteContext(context);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return;
        }
    // </app>
    
    // <textures>
        texture_t* load_texture(const char* file_path) {
            if (textures_amount >= TEXTURES_MAX_AMOUNT){
                return NULL;
            };

            texture_t* texture = malloc(sizeof(texture_t));

            uint32_t texture_id;
            SDL_Surface* texture_load_surface;
            texture_load_surface = IMG_Load(file_path);
            if (texture_load_surface == NULL){
                printf("failed to load texture '%s'.\n", file_path);
                return NULL;
            };
            glGenTextures(1,&texture_id);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_load_surface->w, texture_load_surface->h, 0,
                            GL_RGBA, GL_UNSIGNED_BYTE, texture_load_surface->pixels);
            SDL_FreeSurface(texture_load_surface);

            texture->texture_id = texture_id;
            
            *((uint64_t*)&texture->texture_index) = textures_amount;
            textures_list[textures_amount] = texture;
            textures_amount += 1;
            return texture;
        }
        void bind_texture(texture_t* texture, int32_t uniform, uint8_t texture_num) {
            glUniform1i(uniform, texture_num);
            glActiveTexture(GL_TEXTURE0+texture_num);
            glBindTexture(GL_TEXTURE_2D, texture->texture_id);
            return;
        }
        void destroy_texture(texture_t* texture) {
            textures_amount -= 1;

            // move (textures_list[textures_amount]) to position (texture->texture_index) at (textures_list)
            texture_t* last_textures = textures_list[textures_amount];
            *((uint64_t*)&last_textures->texture_index) = texture->texture_index;
            textures_list[texture->texture_index] = last_textures;

            clean_texture(texture);
            return;
        }
        void clean_texture(texture_t* texture) {
            glDeleteTextures(1, &texture->texture_id);
            free(texture);
            return;
        }
    // </textures>

    // <frame buffer objects>
        // https://learnopengl.com/Advanced-OpenGL/Framebuffers
        
        fbo_t* create_fbo(  uint32_t width, uint32_t height,
                            /*
                            0 = no color attachment
                            1 = texture
                            2 = render buffer object
                            */
                            uint8_t color_status,
                            // GL_RGB, GL_RGBA, etc ; only relevent for texture color attachment
                            GLint color_format,
                            // irrelevent if color_status is not 1
                            texture_parameter_t* color_texture_parameters,
                            // irrelevent if color_status is not 1
                            uint32_t color_texture_parameters_count,
                            /*
                            0 = no depth and stencil attachment
                            1 = texture
                            2 = texture, only depth
                            3 = texture, only stencil
                            4 = render buffer object
                            5 = render buffer object, only depth
                            6 = render buffer object, only stencil
                            */
                            uint8_t depth_stencil_status,
                            // irrelevent if depth_stencil_status is not 1, 2 or 3
                            texture_parameter_t* depth_stencil_texture_parameters,
                            // irrelevent if depth_stencil_status is not 1, 2 or 3
                            uint32_t depth_stencil_texture_parameters_count) {
            if (fbos_amount >= FBOS_MAX_AMOUNT) {
                return NULL;
            }
            
            // save binded frame buffer here
            uint64_t last_fbo = current_fbo;

            fbo_t* fbo = malloc(sizeof(fbo_t));
            if (fbo == NULL) {
                return NULL;
            }

            // create fbo
            glGenFramebuffers(1, &(fbo->fbo_id));
            glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo_id);

            // color
            if (color_status == 1) { // texture
                // create texture
                glGenTextures(1, &(fbo->color));
                glBindTexture(GL_TEXTURE_2D, fbo->color);
                glTexImage2D(GL_TEXTURE_2D, 0, color_format, width, height, 0, color_format, GL_UNSIGNED_BYTE, NULL);
                
                // default texture parameters
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                // user set texture parameters
                for (uint32_t i = 0; i < color_texture_parameters_count; i++) {
                    glTexParameteri(GL_TEXTURE_2D, color_texture_parameters[i].pname, color_texture_parameters[i].param);
                }

                // attach to frame buffer object
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo->color, 0);  
            }else if (color_status == 2) { // render buffer object
                // create render buffer object
                glGenRenderbuffers(1, &(fbo->color));
                glBindRenderbuffer(GL_RENDERBUFFER, fbo->color);
                glRenderbufferStorage(GL_RENDERBUFFER, color_format, width, height);

                // attach to frame buffer object
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, fbo->color);
            }
            fbo->color_status = color_status;
            fbo->color_format = color_format;

            // depth and stencil
            if (depth_stencil_status == 1) { // texture, both depth and stencil
                // create texture
                glGenTextures(1, &(fbo->depth_stencil));
                glBindTexture(GL_TEXTURE_2D, fbo->depth_stencil);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

                // default texture parameters
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                // user set texture parameters
                for (uint32_t i = 0; i < depth_stencil_texture_parameters_count; i++) {
                    glTexParameteri(GL_TEXTURE_2D, depth_stencil_texture_parameters[i].pname, depth_stencil_texture_parameters[i].param);
                }

                // attach to frame buffer object
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, fbo->depth_stencil, 0); 
            }else if (depth_stencil_status == 2) { // texture, only depth
                // create texture
                glGenTextures(1, &(fbo->depth_stencil));
                glBindTexture(GL_TEXTURE_2D, fbo->depth_stencil);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);

                // default texture parameters
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                // user set texture parameters
                for (uint32_t i = 0; i < depth_stencil_texture_parameters_count; i++) {
                    glTexParameteri(GL_TEXTURE_2D, depth_stencil_texture_parameters[i].pname, depth_stencil_texture_parameters[i].param);
                }

                // attach to frame buffer object
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo->depth_stencil, 0); 
            }else if (depth_stencil_status == 3) { // texture, only stencil
                // create texture
                glGenTextures(1, &(fbo->depth_stencil));
                glBindTexture(GL_TEXTURE_2D, fbo->depth_stencil);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, width, height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_INT, NULL);
                
                // default texture parameters
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                // user set texture parameters
                for (uint32_t i = 0; i < depth_stencil_texture_parameters_count; i++) {
                    glTexParameteri(GL_TEXTURE_2D, depth_stencil_texture_parameters[i].pname, depth_stencil_texture_parameters[i].param);
                }

                // attach to frame buffer object
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, fbo->depth_stencil, 0); 
            }else if (depth_stencil_status == 4) { // render buffer object, both depth and stenchil
                // create render buffer object
                glGenRenderbuffers(1, &(fbo->color));
                glBindRenderbuffer(GL_RENDERBUFFER, fbo->color);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

                // attach to frame buffer object
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo->color);
            }else if (depth_stencil_status == 5) { // render buffer object, only depth
                // create render buffer object
                glGenRenderbuffers(1, &(fbo->color));
                glBindRenderbuffer(GL_RENDERBUFFER, fbo->color);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

                // attach to frame buffer object
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->color);
            }else if (depth_stencil_status == 6) { // render buffer object, only stenchil
                // create render buffer object
                glGenRenderbuffers(1, &(fbo->color));
                glBindRenderbuffer(GL_RENDERBUFFER, fbo->color);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, width, height);

                // attach to frame buffer object
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo->color);
            }
            fbo->depth_stencil_status = depth_stencil_status;

            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                printf("frame buffer object creation failed\n");
                clean_fbo(fbo);
                return NULL;
            }

            // <append fbo to fbos_list>
                *((uint64_t*)(&fbo->fbo_index)) = fbos_amount;
                fbos_list[fbos_amount] = fbo;
                fbos_amount += 1;
            // </append fbo to fbos_amount>

            // rebind last frame buffer here
            use_fbo(fbos_list[last_fbo]);

            return fbo;
        }
        void use_fbo(fbo_t* fbo) {
            current_fbo = fbo->fbo_index;
            glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo_id);
            if (current_camera != -1) {
                camera_update_fbo_viewport(cameras_list[current_camera]);
            }
        }
        void bind_fbo_color_texture(fbo_t* fbo, int32_t uniform, uint8_t texture_num) {
            glUniform1i(uniform, texture_num);
            glActiveTexture(GL_TEXTURE0+texture_num);
            glBindTexture(GL_TEXTURE_2D, fbo->color);
        }
        void bind_fbo_depth_stencil_texture(fbo_t* fbo, int32_t uniform, uint8_t texture_num) {
            glUniform1i(uniform, texture_num);
            glActiveTexture(GL_TEXTURE0+texture_num);
            glBindTexture(GL_TEXTURE_2D, fbo->depth_stencil);
        }
        void destroy_fbo(fbo_t* fbo) {
            if (current_fbo == fbo->fbo_index) {
                use_fbo(window_fbo);
            }

            fbos_amount -= 1;

            // move (fbos_list[fbos_amount]) to position (fbo->fbo_index) at (fbos_list)
            fbo_t* last_fbo = fbos_list[fbos_amount];
            *((uint64_t*)&last_fbo->fbo_index) = fbo->fbo_index;
            fbos_list[last_fbo->fbo_index] = last_fbo;

            clean_fbo(fbo);
        }
        void clean_fbo(fbo_t* fbo) {
            if (fbo->color_status == 1) {
                glDeleteTextures(1, &(fbo->color));
            }else if (fbo->color_status == 2) {
                glDeleteRenderbuffers(1, &(fbo->color));
            }

            if (fbo->depth_stencil_status >= 1 && fbo->depth_stencil_status <= 3) {
                glDeleteTextures(1, &(fbo->depth_stencil));
            }else if (fbo->depth_stencil_status >= 4 && fbo->depth_stencil_status <= 6) {
                glDeleteRenderbuffers(1, &(fbo->depth_stencil));
            }
            
            glDeleteFramebuffers(1, &(fbo->fbo_id));
            free(fbo);
            return;
        }
    // </frame buffer objects>

    // <shaders>
        shader_t* shader_create(const char** vert_shader_str, const char** frag_shader_str,
                                const char* attribute_names , uint32_t attributes_count,
                                const char* uniform_names, uint32_t uniforms_count) {
            if (shaders_amount >= SHADERS_MAX_AMOUNT) {
                return NULL;
            }

            shader_t* shader = malloc(sizeof(shader_t));
            if (shader == NULL) {
                return NULL;
            }
            
            int32_t status;
            char err_buf[512];

            // <compile vertex shader>
                shader->vert_shader = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(shader->vert_shader, 1, vert_shader_str, NULL);
                glCompileShader(shader->vert_shader);
                glGetShaderiv(shader->vert_shader, GL_COMPILE_STATUS, &status); // check glCompileShader sucess
                if (status != GL_TRUE) {
                    glGetShaderInfoLog(shader->vert_shader, sizeof(err_buf), NULL, err_buf);
                    err_buf[sizeof(err_buf)-1] = '\0';
                    fprintf(stderr, "Vertex shader compilation failed: %s\nVertex shader:\n\"%s\"\n\n", err_buf, *vert_shader_str);
                    free(shader);
                    return NULL;
                }
            // </compile vertex shader>

            // <compile fragment shader>
                shader->frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(shader->frag_shader, 1, frag_shader_str, NULL);
                glCompileShader(shader->frag_shader);
                glGetShaderiv(shader->frag_shader, GL_COMPILE_STATUS, &status); // check glCompileShader sucess
                if (status != GL_TRUE) {
                    glGetShaderInfoLog(shader->frag_shader, sizeof(err_buf), NULL, err_buf);
                    err_buf[sizeof(err_buf)-1] = '\0';
                    fprintf(stderr, "Fragment shader compilation failed: %s\nFragment shader:\n\"%s\"\n\n", err_buf, *frag_shader_str);
                    free(shader);
                    return NULL;
                }
            // </compile fragment shader>
            
            // <attach shaders to the shader-program>
                shader->program = glCreateProgram();
                glAttachShader(shader->program, shader->vert_shader);
                glAttachShader(shader->program, shader->frag_shader);
            // </attach shaders to the shader-program>

            // <attributes location binding>
                uint32_t attribute_names_offset = 0;
                for (uint32_t i = 0; i < attributes_count; i++) {
                    glBindAttribLocation(shader->program, i, &(attribute_names[attribute_names_offset]));
                    attribute_names_offset += strlen(&(attribute_names[attribute_names_offset]))+1;
                }
            // </attributes location binding>

            // <link vertex and fragment shaders>
                glLinkProgram(shader->program);
                glGetProgramiv(shader->program, GL_LINK_STATUS, &status); // check glLinkProgram sucess
                if (status != GL_TRUE) {
                    glGetProgramInfoLog(shader->program, sizeof(err_buf), NULL, err_buf);
                    err_buf[sizeof(err_buf)-1] = '\0';
                    fprintf(stderr, "Shader program linking failed: %s\nVertex shader:\n\"%s\"\n\nFragment shader:\n\"%s\"\n\n", err_buf, *vert_shader_str, *frag_shader_str);
                    free(shader);
                    return NULL;
                }
            // </link vertex and fragment shaders>

            // the shader was not updated by any camera yet
            shader->wvp_mat_camera_index = -1;
            
            // <shader default uniforms>
                shader->u_texture_loc                               = glGetUniformLocation(shader->program , "u_texture");
                shader->u_camera_world_view_projection_matrix_loc   = glGetUniformLocation(shader->program , "u_camera_world_view_projection_matrix");
                shader->u_instanced_drawing_float_data_loc          = glGetUniformLocation(shader->program , "u_instanced_drawing_float_data");
                shader->u_instanced_drawing_uint_data_loc           = glGetUniformLocation(shader->program , "u_instanced_drawing_uint_data");
                shader->u_joint_matrices_loc                        = glGetUniformLocation(shader->program , "u_joint_matrices");
            // </shader default uniforms>

            // <shader user defined uniforms>
                shader->uniform_locations = malloc(sizeof(int32_t)*uniforms_count);
                uint32_t uniform_names_offset = 0;
                for (uint32_t u = 0; u < uniforms_count; u++) {
                    shader->uniform_locations[u] = glGetUniformLocation(shader->program , &(uniform_names[uniform_names_offset]));
                    uniform_names_offset += strlen(&(uniform_names[uniform_names_offset]))+1;
                }
            // </shader user defined uniforms>

            // <append shader to shaders_list>
                *((uint64_t*)(&shader->shader_index)) = shaders_amount;
                shaders_list[shaders_amount] = shader;
                shaders_amount += 1;
            // </append shader to shaders_list>
            
            shader->active = 1; // set shader as active

            return shader;
        }
        shader_t* shader_create_from_files( const char* vert_shader_file_path, const char* frag_shader_file_path,
                                            const char* attribute_names , uint32_t attributes_count,
                                            const char* uniform_names, uint32_t uniforms_count) {
            char* vert_shader_str;
            load_file_contents(&vert_shader_str,vert_shader_file_path);
            if (vert_shader_str == NULL) {
                return NULL;
            }
            
            char* frag_shader_str;
            load_file_contents(&frag_shader_str,frag_shader_file_path);
            if (frag_shader_str == NULL) {
                free(vert_shader_str);
                return NULL;
            }

            shader_t* shader = shader_create((const char**)&vert_shader_str, (const char**)&frag_shader_str, attribute_names, attributes_count, uniform_names, uniforms_count);
            
            free(vert_shader_str);
            free(frag_shader_str);

            return shader;
        }
        void shader_use(shader_t* shader) {
            if (shader->shader_index == current_shader) {
                return;
            }
            if (shader->shader_index < 0 && shader->shader_index >= shaders_amount) {
                return;
            }
            glUseProgram(shader->program);
            current_shader = shader->shader_index;
            
            if (shader->wvp_mat_camera_index != current_camera) {
                shader_update_camera_uniforms();
                shader->wvp_mat_camera_index = current_camera;
            }
            return;
        }
        void shader_update_camera_uniforms() {
            if (current_shader < 0 && current_shader >= shaders_amount) {
                return;
            }
            if (current_camera >= 0 && current_camera < cameras_amount) {
                if (shaders_list[current_shader]->u_camera_wvp_mat_loc != -1) {
                    glUniformMatrix4fv(shaders_list[current_shader]->u_camera_wvp_mat_loc, 1, 0, cameras_list[current_camera]->wvp_mat.mat);
                }
            }
        }
        void shader_destroy(shader_t* shader) {
            if (shader == default_shader){
                printf("cannot destroy default shader!\n");
                return;
            }
            if (current_shader == shader->shader_index){
                shader_use((shader_t*)default_shader);
            }
            
            shaders_amount -= 1;

            // move (shaders_list[shaders_amount]) to position (shader->shader_index) at (shaders_list)
            shader_t* last_shader = shaders_list[shaders_amount];
            if (current_shader == last_shader->shader_index){
                current_shader = shader->shader_index;
            }
            *((uint64_t*)&last_shader->shader_index) = shader->shader_index;
            shaders_list[shader->shader_index] = last_shader;

            shader_clean(shader);
            return;
        }
        void shader_clean(shader_t* shader) {
            glDetachShader(shader->program, shader->vert_shader);
            glDetachShader(shader->program, shader->frag_shader);
            glDeleteProgram(shader->program);
            glDeleteShader(shader->vert_shader);
            glDeleteShader(shader->frag_shader);
            free(shader->uniform_locations);
            return;
        }
    // </shaders>

    // <meshes and animations>
        mesh_t* generate_mesh(vbo_data_t* vbo_datas_arr, uint32_t vbo_datas_arr_size, uint32_t* indices_array, uint32_t indices_count) {
            if (meshes_amount >= MESHES_MAX_AMOUNT) {
                return NULL;
            }

            mesh_t* mesh = malloc(sizeof(mesh_t));
            if (mesh == NULL) {
                printf("malloc failed at mesh generation.\n");
                return NULL;
            }
            
            // create the vertex array object
            glGenVertexArrays(1, &mesh->vao);
            glBindVertexArray(mesh->vao);

            mesh->vbos_amount = vbo_datas_arr_size;
            mesh->vbos = malloc(sizeof(uint32_t)*mesh->vbos_amount);
            if (mesh->vbos == NULL) {
                printf("malloc failed at mesh generation.\n");
                return mesh;
            }
            
            for (uint32_t i = 0; i < mesh->vbos_amount; i++) {
                glGenBuffers(1, &(mesh->vbos[i]));
                glBindBuffer(GL_ARRAY_BUFFER, mesh->vbos[i]);
                glBufferData(GL_ARRAY_BUFFER, vbo_datas_arr[i].data_arr_size, vbo_datas_arr[i].data_arr, GL_STATIC_DRAW);
                if (vbo_datas_arr[i].type == GL_INT) {
                    glVertexAttribIPointer(i, vbo_datas_arr[i].size, vbo_datas_arr[i].type, vbo_datas_arr[i].stride, (void*)0);
                }else {
                    glVertexAttribPointer(i, vbo_datas_arr[i].size, vbo_datas_arr[i].type, GL_FALSE, vbo_datas_arr[i].stride, (void*)0);
                }
                glEnableVertexAttribArray(i);
                glVertexAttribDivisor(i, vbo_datas_arr[i].divisor);
            }

            mesh->indices_count = indices_count;
            mesh->indices_array = malloc(sizeof(uint32_t)*mesh->indices_count);
            if (mesh->indices_array == NULL) {
                printf("malloc failed at mesh generation.\n");
                return mesh;
            }
            memcpy(mesh->indices_array, indices_array, sizeof(uint32_t)*mesh->indices_count);

            // default joint values
            mesh->joints_amount = 0;
            mesh->joints = NULL;

            *((uint64_t*)&mesh->mesh_index) = meshes_amount;
            meshes_list[meshes_amount] = mesh;
            meshes_amount += 1;

            return mesh;
        }
        mesh_t* generate_2d_quad_mesh(  float min_x, float max_x, float min_y, float max_y,
                                        float min_u, float max_u, float min_v, float max_v) {
            float vertices_position_arr[] = {
                min_x, min_y, 0,
                min_x, max_y, 0,
                max_x, max_y, 0,
                max_x, min_y, 0
            };
            float vertices_texcoord_arr[] = {
                min_u,max_v,
                min_u,min_v,
                max_u,min_v,
                max_u,max_v
            };
            vbo_data_t vbo_datas_arr[2] = {
                {
                    .data_arr_size = sizeof(vertices_position_arr),
                    .data_arr = (void*)vertices_position_arr,
                    .size = 3,
                    .type = GL_FLOAT,
                    .stride = 3*sizeof(float),
                    .divisor = 0
                },
                {
                    .data_arr_size = sizeof(vertices_texcoord_arr),
                    .data_arr = (void*)vertices_texcoord_arr,
                    .size = 2,
                    .type = GL_FLOAT,
                    .stride = 2*sizeof(float),
                    .divisor = 0
                }
            };
            
            uint32_t indices_array[] = {
                0, 1, 2,
                0, 2, 3
            };
            return generate_mesh(vbo_datas_arr, 2, indices_array, 6);
        }
        mesh_t* mesh_generate_ball(uint32_t divisions) {
            const float start_vertices_position_arr[] = {
                // Side A
                 sqrt(8.0/9.0),  0,             -1.0/3.0,
                -sqrt(2.0/9.0),  sqrt(2.0/3.0), -1.0/3.0,
                -sqrt(2.0/9.0), -sqrt(2.0/3.0), -1.0/3.0,
                
                // Side B
                 sqrt(8.0/9.0),  0,             -1.0/3.0,
                -sqrt(2.0/9.0),  sqrt(2.0/3.0), -1.0/3.0,
                 0,              0,              1,
                
                // Side C
                 sqrt(8.0/9.0),  0,             -1.0/3.0,
                -sqrt(2.0/9.0), -sqrt(2.0/3.0), -1.0/3.0,
                 0,              0,              1,
                
                // Side D
                -sqrt(2.0/9.0),  sqrt(2.0/3.0), -1.0/3.0,
                -sqrt(2.0/9.0), -sqrt(2.0/3.0), -1.0/3.0,
                 0,              0,              1
            };
            uint32_t vertices_amount = 12;

            const uint32_t start_indices_array[] = {
                0, 1, 2,
                3, 4, 5,
                6, 7, 8,
                9,10,11
            };
            uint32_t polygons_amount = 4;

            float* vertices_position_arr = malloc(sizeof(start_vertices_position_arr));
            memcpy(vertices_position_arr, start_vertices_position_arr, sizeof(start_vertices_position_arr));
            uint32_t* indices_array = malloc(sizeof(start_indices_array));
            memcpy(indices_array, start_indices_array, sizeof(start_indices_array));
            
            for (uint32_t divs = 0; divs < divisions; divs++) {
                float* new_vertices_position_arr = malloc(sizeof(float)*polygons_amount*4*3*3);
                if (new_vertices_position_arr == NULL) {
                    break;
                }
                uint32_t* new_indices_array = malloc(sizeof(uint32_t)*polygons_amount*4*3);
                if (new_indices_array == NULL) {
                    free(new_vertices_position_arr);
                    break;
                }
                
                for (uint32_t p = 0; p < polygons_amount; p++) {
                    // A
                    new_vertices_position_arr[((p*4 )*3 )*3  ] = vertices_position_arr[indices_array[p*3  ]*3  ];
                    new_vertices_position_arr[((p*4 )*3 )*3+1] = vertices_position_arr[indices_array[p*3  ]*3+1];
                    new_vertices_position_arr[((p*4 )*3 )*3+2] = vertices_position_arr[indices_array[p*3  ]*3+2];

                    new_vertices_position_arr[((p*4 )*3+1)*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3  ] + vertices_position_arr[indices_array[p*3+1]*3  ]);
                    new_vertices_position_arr[((p*4 )*3+1)*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3+1] + vertices_position_arr[indices_array[p*3+1]*3+1]);
                    new_vertices_position_arr[((p*4 )*3+1)*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3+2] + vertices_position_arr[indices_array[p*3+1]*3+2]);
                    
                    new_vertices_position_arr[((p*4 )*3+2)*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3  ] + vertices_position_arr[indices_array[p*3+2]*3  ]);
                    new_vertices_position_arr[((p*4 )*3+2)*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3+1] + vertices_position_arr[indices_array[p*3+2]*3+1]);
                    new_vertices_position_arr[((p*4 )*3+2)*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3+2] + vertices_position_arr[indices_array[p*3+2]*3+2]);
                    
                    // B
                    new_vertices_position_arr[((p*4+1)*3 )*3  ] = vertices_position_arr[indices_array[p*3+1]*3  ];
                    new_vertices_position_arr[((p*4+1)*3 )*3+1] = vertices_position_arr[indices_array[p*3+1]*3+1];
                    new_vertices_position_arr[((p*4+1)*3 )*3+2] = vertices_position_arr[indices_array[p*3+1]*3+2];

                    new_vertices_position_arr[((p*4+1)*3+1)*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3  ] + vertices_position_arr[indices_array[p*3  ]*3  ]);
                    new_vertices_position_arr[((p*4+1)*3+1)*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3+1] + vertices_position_arr[indices_array[p*3  ]*3+1]);
                    new_vertices_position_arr[((p*4+1)*3+1)*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3+2] + vertices_position_arr[indices_array[p*3  ]*3+2]);
                    
                    new_vertices_position_arr[((p*4+1)*3+2)*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3  ] + vertices_position_arr[indices_array[p*3+2]*3  ]);
                    new_vertices_position_arr[((p*4+1)*3+2)*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3+1] + vertices_position_arr[indices_array[p*3+2]*3+1]);
                    new_vertices_position_arr[((p*4+1)*3+2)*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3+2] + vertices_position_arr[indices_array[p*3+2]*3+2]);
                    
                    // C
                    new_vertices_position_arr[((p*4+2)*3 )*3  ] = vertices_position_arr[indices_array[p*3+2]*3  ];
                    new_vertices_position_arr[((p*4+2)*3 )*3+1] = vertices_position_arr[indices_array[p*3+2]*3+1];
                    new_vertices_position_arr[((p*4+2)*3 )*3+2] = vertices_position_arr[indices_array[p*3+2]*3+2];

                    new_vertices_position_arr[((p*4+2)*3+1)*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3+2]*3  ] + vertices_position_arr[indices_array[p*3  ]*3  ]);
                    new_vertices_position_arr[((p*4+2)*3+1)*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3+2]*3+1] + vertices_position_arr[indices_array[p*3  ]*3+1]);
                    new_vertices_position_arr[((p*4+2)*3+1)*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3+2]*3+2] + vertices_position_arr[indices_array[p*3  ]*3+2]);
                    
                    new_vertices_position_arr[((p*4+2)*3+2)*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3+2]*3  ] + vertices_position_arr[indices_array[p*3+1]*3  ]);
                    new_vertices_position_arr[((p*4+2)*3+2)*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3+2]*3+1] + vertices_position_arr[indices_array[p*3+1]*3+1]);
                    new_vertices_position_arr[((p*4+2)*3+2)*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3+2]*3+2] + vertices_position_arr[indices_array[p*3+1]*3+2]);

                    // MIDDLE
                    new_vertices_position_arr[((p*4+3)*3 )*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3  ] + vertices_position_arr[indices_array[p*3+1]*3  ]);
                    new_vertices_position_arr[((p*4+3)*3 )*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3+1] + vertices_position_arr[indices_array[p*3+1]*3+1]);
                    new_vertices_position_arr[((p*4+3)*3 )*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3+2] + vertices_position_arr[indices_array[p*3+1]*3+2]);
                    
                    new_vertices_position_arr[((p*4+3)*3+1)*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3  ] + vertices_position_arr[indices_array[p*3+2]*3  ]);
                    new_vertices_position_arr[((p*4+3)*3+1)*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3+1] + vertices_position_arr[indices_array[p*3+2]*3+1]);
                    new_vertices_position_arr[((p*4+3)*3+1)*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3  ]*3+2] + vertices_position_arr[indices_array[p*3+2]*3+2]);
                    
                    new_vertices_position_arr[((p*4+3)*3+2)*3  ] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3  ] + vertices_position_arr[indices_array[p*3+2]*3  ]);
                    new_vertices_position_arr[((p*4+3)*3+2)*3+1] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3+1] + vertices_position_arr[indices_array[p*3+2]*3+1]);
                    new_vertices_position_arr[((p*4+3)*3+2)*3+2] = 0.5 * (vertices_position_arr[indices_array[p*3+1]*3+2] + vertices_position_arr[indices_array[p*3+2]*3+2]);


                    // A
                    new_indices_array[(p*4 )*3  ] = ((p*4 )*3 );
                    new_indices_array[(p*4 )*3+1] = ((p*4 )*3+1);
                    new_indices_array[(p*4 )*3+2] = ((p*4 )*3+2);
                    
                    // B
                    new_indices_array[(p*4+1)*3  ] = ((p*4+1)*3 );
                    new_indices_array[(p*4+1)*3+1] = ((p*4+1)*3+1);
                    new_indices_array[(p*4+1)*3+2] = ((p*4+1)*3+2);
                    
                    // C
                    new_indices_array[(p*4+2)*3  ] = ((p*4+2)*3 );
                    new_indices_array[(p*4+2)*3+1] = ((p*4+2)*3+1);
                    new_indices_array[(p*4+2)*3+2] = ((p*4+2)*3+2);
                    
                    // MIDDLE
                    new_indices_array[(p*4+3)*3  ] = ((p*4+3)*3 );
                    new_indices_array[(p*4+3)*3+1] = ((p*4+3)*3+1);
                    new_indices_array[(p*4+3)*3+2] = ((p*4+3)*3+2);
                }

                vertices_amount = polygons_amount*4*3;
                free(vertices_position_arr);
                vertices_position_arr = new_vertices_position_arr;
                
                polygons_amount = polygons_amount*4;
                free(indices_array);
                indices_array = new_indices_array;
            }

            /*
            for (uint32_t i = 0; i < vertices_amount; i++){
                printf("%u:\t%f\t%f\t%f\n", i,
                    vertices_position_arr[i*3  ],
                    vertices_position_arr[i*3+1],
                    vertices_position_arr[i*3+2]
               );
            }
            printf("\n");
            for (uint32_t i = 0; i < polygons_amount; i++){
                printf("%u:\t%u\t%u\t%u\n", i,
                    indices_array[i*3  ],
                    indices_array[i*3+1],
                    indices_array[i*3+2]
               );
            }
            */
            
            for (uint32_t i = 0; i < vertices_amount; i++){ // normalise all vertices
                float len = sqrt(
                    vertices_position_arr[i*3  ]*vertices_position_arr[i*3  ] +
                    vertices_position_arr[i*3+1]*vertices_position_arr[i*3+1] +
                    vertices_position_arr[i*3+2]*vertices_position_arr[i*3+2]
               );
                vertices_position_arr[i*3  ] /= len;
                vertices_position_arr[i*3+1] /= len;
                vertices_position_arr[i*3+2] /= len;
            }

            vbo_data_t vbo_datas_arr[1] = {
                {
                    .data_arr_size = sizeof(float)*vertices_amount*3,
                    .data_arr = (void*)vertices_position_arr,
                    .size = 3,
                    .type = GL_FLOAT,
                    .stride = 3*sizeof(float),
                    .divisor = 0
                }
            };

            mesh_t* result = generate_mesh(vbo_datas_arr, 1, indices_array, polygons_amount*3);

            free(vertices_position_arr);
            free(indices_array);

            return result;
        }
        mesh_t* mesh_from_wavefront_obj_ext(const char* obj_file_path, quat_vec_vec_t transform_qvv) {
            char* obj_str;
            uint64_t file_length = load_file_contents(&obj_str,obj_file_path);
            if (obj_str == NULL) {
                return NULL;
            }

            mesh_t* mesh = NULL;

            uint32_t vertices_count = 0;
            uint32_t normals_count = 0;
            uint32_t uvs_count = 0;
            uint32_t faces_count = 0;
            uint32_t unique_vertices_count = 0;
            uint32_t polygons_count = 0; // triangles

            int64_t vertex_index;
            int64_t normal_index;
            int64_t uv_index;
            int64_t unique_vertex_index;
            int64_t polygon_index;
            int64_t face_first_unique_vertex_index;

            char last_char = '\0';
            // 0 = irrelevent
            // 1 = vertex
            // 2 = normal
            // 3 = uv
            // 4 = face
            uint8_t line_type = 0;
            uint32_t line_space_count = 0;
            uint32_t between_spaces_slash_count;

            uint64_t i;

            vec3_t vec3;

            vec3_t* vertices;
            vec3_t* normals;
            vec2_t* uvs;
            uint32_t* indices_array;
            float* vertices_position_arr;
            float* vertices_texcoord_arr;
            float* vertices_normal_arr;

            int32_t num;


            // count
            for (i = 0; i < file_length; i++) {
                if (last_char == '\n') {
                    line_space_count = 0;

                    // line type after newline
                    if (obj_str[i] == 'v') {
                        if (obj_str[i+1] == 'n') {
                            line_type = 2;
                            i += 2;
                            normals_count += 1;
                        }else if (obj_str[i+1] == 't') {
                            line_type = 3;
                            i += 2;
                            uvs_count += 1;
                        }else if (obj_str[i+1] == ' ') {
                            line_type = 1;
                            i += 1;
                            vertices_count += 1;
                        }else {
                            printf("wierd read at obj file load\n");
                            line_type = 0;
                        }
                    }else if (obj_str[i] == 'f') {
                        line_type = 4;
                        i += 1;
                        faces_count += 1;
                        polygons_count += 1;
                    }else {
                        line_type = 0;
                    }
                }

                // multipile polyons at 1 face
                if (line_type == 4) {
                    if (obj_str[i] == ' ') {
                        line_space_count += 1;
                        unique_vertices_count += 1;
                        if (line_space_count > 3) {
                            polygons_count += 1;
                        }
                    }
                }

                last_char = obj_str[i];
            }


            vertices = malloc(sizeof(vec3_t)*vertices_count);
            normals = malloc(sizeof(vec3_t)*normals_count);
            uvs = malloc(sizeof(vec2_t)*uvs_count);
            // final
            indices_array = malloc(sizeof(uint32_t)*polygons_count*3);
            vertices_position_arr = malloc(sizeof(float)*unique_vertices_count*3);
            vertices_texcoord_arr = malloc(sizeof(float)*unique_vertices_count*2);
            vertices_normal_arr = malloc(sizeof(float)*unique_vertices_count*3);
            
            if (
                vertices == NULL ||
                normals == NULL ||
                uvs == NULL ||
                vertices_position_arr == NULL ||
                indices_array == NULL ||
                vertices_texcoord_arr == NULL ||
                vertices_normal_arr == NULL
            ) {
                goto clean_and_return;
            }


            // set array
            vertex_index = -1;
            normal_index = -1;
            uv_index = -1;
            unique_vertex_index = -1;
            polygon_index = -1;
            face_first_unique_vertex_index = -1;
            
            line_type = 0;
            line_space_count = 0;
            between_spaces_slash_count = 0;
            
            for (i = 0; i < file_length; i++) {
                if (last_char == '\n') {
                    line_space_count = 0;

                    // line type after newline
                    if (obj_str[i] == 'v') {
                        if (obj_str[i+1] == 'n') {
                            line_type = 2;
                            i += 2;
                            normal_index += 1;
                        }else if (obj_str[i+1] == 't') {
                            line_type = 3;
                            i += 2;
                            uv_index += 1;
                        }else if (obj_str[i+1] == ' ') {
                            line_type = 1;
                            i += 1;
                            vertex_index += 1;
                        }else {
                            printf("wierd read at obj file load\n");
                            line_type = 0;
                        }
                    }else if (obj_str[i] == 'f') {
                        line_type = 4;
                        i += 1;
                        polygon_index += 1;
                    }else {
                        line_type = 0;
                    }
                }

                if (line_type == 1) {
                    if (obj_str[i] == ' ') {
                        if (line_space_count == 0) {
                            vertices[vertex_index].x = atof(obj_str+i);
                        }
                        if (line_space_count == 1) {
                            vertices[vertex_index].y = atof(obj_str+i);
                        }
                        if (line_space_count == 2) {
                            vertices[vertex_index].z = atof(obj_str+i);
                        }
                        line_space_count += 1;
                    }
                }else if (line_type == 2) {
                    if (obj_str[i] == ' ') {
                        if (line_space_count == 0) {
                            normals[normal_index].x = atof(obj_str+i);
                        }
                        if (line_space_count == 1) {
                            normals[normal_index].y = atof(obj_str+i);
                        }
                        if (line_space_count == 2) {
                            normals[normal_index].z = atof(obj_str+i);
                        }
                        line_space_count += 1;
                    }
                }else if (line_type == 3) {
                    if (obj_str[i] == ' ') {
                        if (line_space_count == 0) {
                            uvs[uv_index].x = atof(obj_str+i);
                        }
                        if (line_space_count == 1) {
                            uvs[uv_index].y = atof(obj_str+i);
                        }
                        line_space_count += 1;
                    }
                }if (line_type == 4) {
                    if (obj_str[i] == ' ') {
                        between_spaces_slash_count = 0;

                        // append unique vertex
                        unique_vertex_index += 1;
                        num = atoi(obj_str+i);
                        vertices_position_arr[unique_vertex_index*3    ] = vertices[num-1].x;
                        vertices_position_arr[unique_vertex_index*3 + 1] = vertices[num-1].y;
                        vertices_position_arr[unique_vertex_index*3 + 2] = vertices[num-1].z;
                        
                        if (line_space_count == 0) {
                            face_first_unique_vertex_index = unique_vertex_index;
                        }
                        
                        if (line_space_count <= 2) {
                            indices_array[polygon_index*3 + line_space_count] = unique_vertex_index
                                                           /* fix indices order for face culling */ + (-line_space_count+1)*2;
                        }else {
                            // multipile polyons at 1 face
                            // add an entire polygon
                            polygon_index += 1;
                            
                            indices_array[polygon_index*3    ] = unique_vertex_index;
                            indices_array[polygon_index*3 + 1] = unique_vertex_index-1;
                            indices_array[polygon_index*3 + 2] = face_first_unique_vertex_index;
                        }
                        
                        line_space_count += 1;
                    }else if (obj_str[i] == '/') {
                        between_spaces_slash_count += 1;

                        num = atoi(obj_str+i+1);

                        if (between_spaces_slash_count == 1) {
                            vertices_texcoord_arr[unique_vertex_index*2    ] = uvs[num-1].x;
                            vertices_texcoord_arr[unique_vertex_index*2 + 1] = uvs[num-1].y;
                        }else {
                            vertices_normal_arr[unique_vertex_index*3    ] = normals[num-1].x;
                            vertices_normal_arr[unique_vertex_index*3 + 1] = normals[num-1].y;
                            vertices_normal_arr[unique_vertex_index*3 + 2] = normals[num-1].z;
                        }
                        
                    }
                }

                last_char = obj_str[i];
            }

            // transform
            for (i = 0; i < unique_vertices_count; i++) {
                vec3 = (vec3_t){
                    .x = vertices_position_arr[i*3    ],
                    .y = vertices_position_arr[i*3 + 1],
                    .z = vertices_position_arr[i*3 + 2]
                };
                vec3 = vec_scale_rotate_translate(vec3, transform_qvv);
                vertices_position_arr[i*3    ] = vec3.x;
                vertices_position_arr[i*3 + 1] = vec3.y;
                vertices_position_arr[i*3 + 2] = vec3.z;
            }
            for (i = 0; i < unique_vertices_count; i++) {
                vec3 = (vec3_t){
                    .x = vertices_normal_arr[i*3    ],
                    .y = vertices_normal_arr[i*3 + 1],
                    .z = vertices_normal_arr[i*3 + 2]
                };
                vec3 = rotate_vector(vec3, transform_qvv.rot);
                vertices_normal_arr[i*3    ] = vec3.x;
                vertices_normal_arr[i*3 + 1] = vec3.y;
                vertices_normal_arr[i*3 + 2] = vec3.z;
            }

            // <debug>
                #if 0
                printf(
                    "data from '%s':\n  vertices count: %u\n  normals count: %u\n  uvs count: %u\n  unique_vertices_count: %u\n  faces count: %u\n  polygons count: %u\n",
                    obj_file_path,
                    vertices_count,
                    normals_count,
                    uvs_count,
                    unique_vertices_count,
                    faces_count,
                    polygons_count
                );
                printf(
                    "  counts:\n    vertex_index: %llu\n    normal_index: %llu\n    uv_index: %llu\n    unique_vertex_index: %llu\n    polygon_index: %llu\n",
                    vertex_index,
                    normal_index,
                    uv_index,
                    unique_vertex_index,
                    polygon_index
                );
                printf("  vertices:[\n");
                for (uint64_t k = 0; k < vertices_count; k++) {
                    printf("    (%f, %f, %f),\n", vertices[k].x, vertices[k].y, vertices[k].z);
                }
                printf("  ]\n  normals:[\n");
                for (uint64_t k = 0; k < normals_count; k++) {
                    printf("    (%f, %f, %f),\n", normals[k].x, normals[k].y, normals[k].z);
                }
                printf("  ]\n  uvs:[\n");
                for (uint64_t k = 0; k < uvs_count; k++) {
                    printf("    (%f, %f),\n", uvs[k].x, uvs[k].y);
                }
                printf("  ]\n  indices_array:[\n");
                for (uint64_t k = 0; k < polygons_count; k++) {
                    printf("    (%u, %u, %u),\n", indices_array[k*3], indices_array[k*3+1], indices_array[k*3+2]);
                }
                printf("  ]\n  vertices_position_arr:[\n");
                for (uint64_t k = 0; k < unique_vertices_count; k++) {
                    printf("    (%f, %f, %f),\n", vertices_position_arr[k*3], vertices_position_arr[k*3+1], vertices_position_arr[k*3+2]);
                }
                printf("  ]\n  vertices_texcoord_arr:[\n");
                for (uint64_t k = 0; k < unique_vertices_count; k++) {
                    printf("    (%f, %f),\n", vertices_texcoord_arr[k*2], vertices_texcoord_arr[k*2+1]);
                }
                printf("  ]\n  vertices_normal_arr:[\n");
                for (uint64_t k = 0; k < unique_vertices_count; k++) {
                    printf("    (%f, %f, %f),\n", vertices_normal_arr[k*3], vertices_normal_arr[k*3+1], vertices_normal_arr[k*3+2]);
                }
                printf("  ]\n\n");
                #endif
            // </debug>

            vbo_data_t vbo_datas_arr[3] = {
                {
                    .data_arr_size = sizeof(float)*unique_vertices_count*3,
                    .data_arr = (void*)vertices_position_arr,
                    .size = 3,
                    .type = GL_FLOAT,
                    .stride = 3*sizeof(float),
                    .divisor = 0
                },
                {
                    .data_arr_size = sizeof(float)*unique_vertices_count*2,
                    .data_arr = (void*)vertices_texcoord_arr,
                    .size = 2,
                    .type = GL_FLOAT,
                    .stride = 2*sizeof(float),
                    .divisor = 0
                },
                {
                    .data_arr_size = sizeof(float)*unique_vertices_count*3,
                    .data_arr = (void*)vertices_normal_arr,
                    .size = 3,
                    .type = GL_FLOAT,
                    .stride = 3*sizeof(float),
                    .divisor = 0
                }
            };

            mesh = generate_mesh(vbo_datas_arr, 3, indices_array, polygons_count*3);

            goto clean_and_return;
            clean_and_return: {
                if (obj_str != NULL)               {free(obj_str);}
                if (vertices != NULL)              {free(vertices);}
                if (normals != NULL)               {free(normals);}
                if (uvs != NULL)                   {free(uvs);}
                if (indices_array != NULL)         {free(indices_array);}
                if (vertices_position_arr != NULL) {free(vertices_position_arr);}
                if (vertices_texcoord_arr != NULL) {free(vertices_texcoord_arr);}
                if (vertices_normal_arr != NULL)   {free(vertices_normal_arr);}
                
                return mesh;
            }
        }
        mesh_t* mesh_from_wavefront_obj(const char* obj_file_path) {
            quat_vec_vec_t transform_qvv = (quat_vec_vec_t){
                .rot = (quat_t){
                    .w = 1,
                    .x = 0,
                    .y = 0,
                    .z = 0
                },
                .pos = (vec3_t){
                    .x = 0,
                    .y = 0,
                    .z = 0,
                },
                .scale = (vec3_t){
                    .x = 1,
                    .y = 1,
                    .z = 1,
                }
            };
            return mesh_from_wavefront_obj_ext(obj_file_path, transform_qvv);
        }
        void static mesh_from_collada_dae_joint_hierarchy(char* dae_str, mesh_t* mesh) {
            uint32_t i;
            int64_t str_i;
            int64_t dstr_i1;
            int64_t dstr_i2;
            uint32_t current_parent_joint;
            uint32_t current_joint;
            int32_t joint_depth;
            char* name;

            // find first joint
            str_i = str_find_substr(dae_str, "<node");
            while (str_find_substr(&(dae_str[str_i]), "type=\"NODE\">") < str_find_substr(&(dae_str[str_i]), "type=\"JOINT\">")) {
                str_i += str_find_substr(&(dae_str[str_i+1]), "<node");
            }
            
            joint_depth = -1;
            while (1) {
                dstr_i1 = str_find_substr(&(dae_str[str_i]), "</node>");
                dstr_i2 = str_find_substr(&(dae_str[str_i]), "<node");
                if (dstr_i1 < dstr_i2) {
                    str_i += dstr_i1;
                    str_i += str_find_substr(&(dae_str[str_i]), ">");
                    // go back with current_joint and current_parent_joint
                    current_joint = current_parent_joint;
                    current_parent_joint = mesh->joints[current_parent_joint].parent;
                    joint_depth -= 1;
                    if (joint_depth < 0) {
                        break;
                    }
                    continue;
                }

                // set str_i to the start of the "<node..." string
                str_i += dstr_i2;
                joint_depth += 1;

                // set current_parent_joint to the last joint
                current_parent_joint = current_joint;

                // find joint name
                str_i += str_find_substr(&(dae_str[str_i]), "name=\"");
                str_i += strlen("name=\"");
                dstr_i1 = str_find_substr(&(dae_str[str_i]), "\"");
                name = malloc(sizeof(char)*(dstr_i1+1));
                memcpy(name, &(dae_str[str_i]), dstr_i1);
                name[dstr_i1] = '\0';
                // find joint index from name
                for (i = 0; i < mesh->joints_amount; i++) {
                    if (strcmp(name, mesh->joints[i].name) == 0) {
                        current_joint = i;
                        break;
                    }
                }
                free(name);

                if (joint_depth == 0) {
                    current_parent_joint = current_joint;
                }

                // set parent
                mesh->joints[current_joint].parent = current_parent_joint;
                
                // go to end of joint row
                str_i += str_find_substr(&(dae_str[str_i]), ">");
            }

        }
        mesh_t* mesh_from_collada_dae_ext(const char* dae_file_path, quat_vec_vec_t transform_qvv) {
            char* dae_str;
            uint64_t file_length = load_file_contents(&dae_str,dae_file_path);
            if (dae_str == NULL) {
                return NULL;
            }

            mesh_t* mesh;
            int64_t str_i;
            int64_t dstr_i;

            uint64_t vertices_positions_array_length;
            float* vertices_positions_array;
            uint64_t vertices_normals_array_length;
            float* vertices_normals_array;
            uint64_t vertices_texcoords_array_length;
            float* vertices_texcoords_array;
            uint64_t vertices_joint_ids_array_length;
            int32_t* vertices_joint_ids_array;
            uint64_t vertices_joint_wheights_array_length;
            float* vertices_joint_wheights_array;
            
            uint64_t joints_data_count_array_length;
            int32_t* joints_data_count_array;
            uint64_t joints_data_array_length;
            int32_t* joints_data_array;
            uint64_t joint_wheights_array_length;
            float* joint_wheights_array;

            int32_t i;
            int32_t j;
            uint64_t triangles_count;
            uint64_t current_triangles_count;
            uint64_t current_triangle_index;
            int32_t* triangles_data_arr;
            int32_t vertex_i;
            int32_t normal_i;
            int32_t texcoord_i;
            int32_t joint_data_count;
            uint64_t joint_data_index;
            float joint_data_wheight_max1;
            int64_t joint_data_wheight_max1_index;
            float joint_data_wheight_max2;
            int64_t joint_data_wheight_max2_index;
            float joint_data_wheight_max3;
            int64_t joint_data_wheight_max3_index;
            float joint_data_wheight_tmp;
            int64_t joint_data_wheight_tmp_index;
            uint64_t indice_index;
            float vertex_joint_wheights_total;
            vec3_t vec3;

            uint32_t* indices_array;
            float* vbo_vertices_position_arr;
            float* vbo_vertices_texcoord_arr;
            float* vbo_vertices_normal_arr;
            int32_t* vbo_vertices_joint_id_arr;
            float* vbo_vertices_joint_wheight_arr;

            uint64_t joints_inverse_bind_matrices_array_length;
            float* joints_inverse_bind_matrices_array;

            mat4_t transform_mat = mat4_from_quat_vec_vec(transform_qvv);


            // vertex positions array
            str_i = str_find_substr(dae_str, "positions-array\" count=\"");
            str_i += strlen("positions-array\" count=\"");
            vertices_positions_array_length = atoi(&(dae_str[str_i]));
            str_i += str_find_substr(&(dae_str[str_i]), "\">");
            str_i += strlen("\">");
            vertices_positions_array = str_to_float_array(&(dae_str[str_i]), vertices_positions_array_length);
            if (vertices_positions_array == NULL){
                goto clean_and_return;
            }
            
            // vertex normals array
            str_i = str_find_substr(dae_str, "normals-array\" count=\"");
            str_i += strlen("normals-array\" count=\"");
            vertices_normals_array_length = atoi(&(dae_str[str_i]));
            str_i += str_find_substr(&(dae_str[str_i]), "\">");
            str_i += strlen("\">");
            vertices_normals_array = str_to_float_array(&(dae_str[str_i]), vertices_normals_array_length);
            if (vertices_normals_array == NULL){
                goto clean_and_return;
            }
            
            // vertex texcoords array
            str_i = str_find_substr(dae_str, "map-0-array\" count=\"");
            str_i += strlen("map-0-array\" count=\"");
            vertices_texcoords_array_length = atoi(&(dae_str[str_i]));
            str_i += str_find_substr(&(dae_str[str_i]), "\">");
            str_i += strlen("\">");
            vertices_texcoords_array = str_to_float_array(&(dae_str[str_i]), vertices_texcoords_array_length);
            if (vertices_texcoords_array == NULL){
                goto clean_and_return;
            }
            
            // vertex joint wheights and vertex joint ids arrays
            // joint wheights array
            str_i = str_find_substr(dae_str, "skin-weights-array\" count=\"");
            str_i += strlen("skin-weights-array\" count=\"");
            joint_wheights_array_length = atoi(&(dae_str[str_i]));
            str_i += str_find_substr(&(dae_str[str_i]), "\">");
            str_i += strlen("\">");
            joint_wheights_array = str_to_float_array(&(dae_str[str_i]), joint_wheights_array_length);
            if (joint_wheights_array == NULL){
                goto clean_and_return;
            }
            // joints data count array
            str_i = str_find_substr(dae_str, "<vertex_weights count=\"");
            str_i += strlen("<vertex_weights count=\"");
            joints_data_count_array_length = atoi(&(dae_str[str_i]));
            str_i += str_find_substr(&(dae_str[str_i]), "<vcount>");
            str_i += strlen("<vcount>");
            joints_data_count_array = str_to_int_array(&(dae_str[str_i]), joints_data_count_array_length);
            if (joints_data_count_array == NULL){
                goto clean_and_return;
            }
            // joints data array
            joints_data_array_length = 0;
            for (i = 0; i < joints_data_count_array_length; i++) {
                joints_data_array_length += joints_data_count_array[i];
            }
            joints_data_array_length *= 2;
            str_i += str_find_substr(&(dae_str[str_i]), "<v>");
            str_i += strlen("<v>");
            joints_data_array = str_to_int_array(&(dae_str[str_i]), joints_data_array_length);
            if (joints_data_array == NULL){
                goto clean_and_return;
            }
            // vertex joint wheights and vertex joint ids arrays
            vertices_joint_ids_array = malloc(sizeof(int32_t)*joints_data_count_array_length*3);
            vertices_joint_wheights_array = malloc(sizeof(float)*joints_data_count_array_length*3);
            if (vertices_joint_ids_array == NULL || vertices_joint_wheights_array == NULL){
                goto clean_and_return;
            }
            joint_data_index = 0;
            for (i = 0; i < joints_data_count_array_length; i++) {
                joint_data_count = joints_data_count_array[i];
                if (joint_data_count < 3) {
                    for (j = 0; j < 3; j++) {
                        if (j < joint_data_count) {
                            vertices_joint_ids_array[i*3 + j]      = joints_data_array[joint_data_index*2];
                            vertices_joint_wheights_array[i*3 + j] = joint_wheights_array[joints_data_array[joint_data_index*2 + 1]];
                            joint_data_index += 1;
                        }else {
                            vertices_joint_ids_array[i*3 + j]      = 0;
                            vertices_joint_wheights_array[i*3 + j] = 0;
                        }
                    }
                }else {
                    // initial values
                    joint_data_wheight_max1_index = -1;
                    joint_data_wheight_max1 = 0;
                    joint_data_wheight_max2_index = -1;
                    joint_data_wheight_max2 = 0;
                    joint_data_wheight_max3_index = -1;
                    joint_data_wheight_max3 = 0;


                    // find 3 biggest wheight values
                    for (j = 0; j < joint_data_count; j++) {
                        joint_data_wheight_tmp_index = joint_data_index + j;
                        joint_data_wheight_tmp = joint_wheights_array[joints_data_array[joint_data_wheight_tmp_index*2 + 1]];

                        if (joint_data_wheight_tmp > joint_data_wheight_max3) {
                            joint_data_wheight_max3 = joint_data_wheight_tmp;
                            joint_data_wheight_max3_index = joint_data_wheight_tmp_index;
                            
                            if (joint_data_wheight_max3 > joint_data_wheight_max2) {
                                joint_data_wheight_tmp        = joint_data_wheight_max2;
                                joint_data_wheight_tmp_index  = joint_data_wheight_max2_index;
                                joint_data_wheight_max2       = joint_data_wheight_max3;
                                joint_data_wheight_max2_index = joint_data_wheight_max3_index;
                                joint_data_wheight_max3       = joint_data_wheight_tmp;
                                joint_data_wheight_max3_index = joint_data_wheight_tmp_index;

                                if (joint_data_wheight_max2 > joint_data_wheight_max1) {
                                    joint_data_wheight_tmp        = joint_data_wheight_max1;
                                    joint_data_wheight_tmp_index  = joint_data_wheight_max1_index;
                                    joint_data_wheight_max1       = joint_data_wheight_max2;
                                    joint_data_wheight_max1_index = joint_data_wheight_max2_index;
                                    joint_data_wheight_max2       = joint_data_wheight_tmp;
                                    joint_data_wheight_max2_index = joint_data_wheight_tmp_index;
                                }
                            }
                        }
                    }

                    // set data
                    if (joint_data_wheight_max1_index >= 0) {
                        vertices_joint_ids_array[i*3    ]      = joints_data_array[joint_data_wheight_max1_index*2];
                        vertices_joint_wheights_array[i*3    ] = joint_wheights_array[joints_data_array[joint_data_wheight_max1_index*2 + 1]];
                    }else {
                        vertices_joint_ids_array[i*3    ] = 0;
                        vertices_joint_wheights_array[i*3    ] = 0;
                    }
                    if (joint_data_wheight_max2_index >= 0) {
                        vertices_joint_ids_array[i*3 + 1]      = joints_data_array[joint_data_wheight_max2_index*2];
                        vertices_joint_wheights_array[i*3 + 1] = joint_wheights_array[joints_data_array[joint_data_wheight_max2_index*2 + 1]];
                    }else {
                        vertices_joint_ids_array[i*3 + 1] = 0;
                        vertices_joint_wheights_array[i*3 + 1] = 0;
                    }
                    if (joint_data_wheight_max3_index >= 0) {
                        vertices_joint_ids_array[i*3 + 2]      = joints_data_array[joint_data_wheight_max3_index*2];
                        vertices_joint_wheights_array[i*3 + 2] = joint_wheights_array[joints_data_array[joint_data_wheight_max3_index*2 + 1]];
                    }else {
                        vertices_joint_ids_array[i*3 + 2] = 0;
                        vertices_joint_wheights_array[i*3 + 2] = 0;
                    }

                    joint_data_index += joint_data_count;
                }

                vertex_joint_wheights_total = vertices_joint_wheights_array[i*3] + vertices_joint_wheights_array[i*3 + 1] + vertices_joint_wheights_array[i*3 + 2];
                if (vertex_joint_wheights_total != 0) {
                    vertices_joint_wheights_array[i*3    ] /= vertex_joint_wheights_total;
                    vertices_joint_wheights_array[i*3 + 1] /= vertex_joint_wheights_total;
                    vertices_joint_wheights_array[i*3 + 2] /= vertex_joint_wheights_total;
                }
            }
            

            // find triangles_count
            triangles_count = 0;
            str_i = 0;
            while (1) {
                dstr_i = str_find_substr(&(dae_str[str_i]), "<triangles");
                str_i += dstr_i;
                if (dstr_i == -1) {
                    break;
                }
                str_i += str_find_substr(&(dae_str[str_i]), "count=\"");
                str_i += strlen("count=\"");
                triangles_count += atoi(&(dae_str[str_i]));
            }

            // set all values
            indices_array = malloc(sizeof(uint32_t)*triangles_count*3);
            vbo_vertices_position_arr = malloc(sizeof(float)*triangles_count*3*3);
            vbo_vertices_texcoord_arr = malloc(sizeof(float)*triangles_count*3*2);
            vbo_vertices_normal_arr = malloc(sizeof(float)*triangles_count*3*3);
            vbo_vertices_joint_id_arr = malloc(sizeof(int32_t)*triangles_count*3*3);
            vbo_vertices_joint_wheight_arr = malloc(sizeof(float)*triangles_count*3*3);
            if (
                indices_array == NULL ||
                vbo_vertices_position_arr == NULL ||
                vbo_vertices_texcoord_arr == NULL ||
                vbo_vertices_normal_arr == NULL ||
                vbo_vertices_joint_id_arr == NULL ||
                vbo_vertices_joint_wheight_arr == NULL
            ) {
                goto clean_and_return;
            }

            str_i = 0;
            indice_index = 0;
            while (1) {
                dstr_i = str_find_substr(&(dae_str[str_i]), "<triangles");
                str_i += dstr_i;
                if (dstr_i == -1) {
                    break;
                }
                str_i += str_find_substr(&(dae_str[str_i]), "count=\"");
                str_i += strlen("count=\"");
                current_triangles_count = atoi(&(dae_str[str_i]));
                str_i += str_find_substr(&(dae_str[str_i]), "<p>");
                str_i += strlen("<p>");
                triangles_data_arr = str_to_int_array(&(dae_str[str_i]), current_triangles_count*3*3);
                if (triangles_data_arr == NULL) {
                    goto clean_and_return;
                }

                for (current_triangle_index = 0; current_triangle_index < current_triangles_count; current_triangle_index++) {
                    for (i = 0; i < 3; i++) {
                        vertex_i   = triangles_data_arr[current_triangle_index*3*3 + i*3    ];
                        normal_i   = triangles_data_arr[current_triangle_index*3*3 + i*3 + 1];
                        texcoord_i = triangles_data_arr[current_triangle_index*3*3 + i*3 + 2];
                        
                        vec3 = (vec3_t){
                            .x = vertices_positions_array[vertex_i*3    ],
                            .y = vertices_positions_array[vertex_i*3 + 1],
                            .z = vertices_positions_array[vertex_i*3 + 2]
                        };
                        vec3 = vec_scale_rotate_translate(vec3, transform_qvv);
                        vbo_vertices_position_arr[(indice_index+i)*3    ] = vec3.x;
                        vbo_vertices_position_arr[(indice_index+i)*3 + 1] = vec3.y;
                        vbo_vertices_position_arr[(indice_index+i)*3 + 2] = vec3.z;
                        
                        vec3 = (vec3_t){
                            .x = vertices_normals_array[normal_i*3    ],
                            .y = vertices_normals_array[normal_i*3 + 1],
                            .z = vertices_normals_array[normal_i*3 + 2]
                        };
                        vec3 = rotate_vector(vec3, transform_qvv.rot);
                        vbo_vertices_normal_arr[  (indice_index+i)*3    ] = vec3.x;
                        vbo_vertices_normal_arr[  (indice_index+i)*3 + 1] = vec3.y;
                        vbo_vertices_normal_arr[  (indice_index+i)*3 + 2] = vec3.z;
                        
                        vbo_vertices_texcoord_arr[(indice_index+i)*2    ] = vertices_texcoords_array[texcoord_i*2    ];
                        vbo_vertices_texcoord_arr[(indice_index+i)*2 + 1] = vertices_texcoords_array[texcoord_i*2 + 1];
                        
                        vbo_vertices_joint_id_arr[(indice_index+i)*3    ]      = vertices_joint_ids_array[vertex_i*3    ];
                        vbo_vertices_joint_id_arr[(indice_index+i)*3 + 1]      = vertices_joint_ids_array[vertex_i*3 + 1];
                        vbo_vertices_joint_id_arr[(indice_index+i)*3 + 2]      = vertices_joint_ids_array[vertex_i*3 + 2];
                        vbo_vertices_joint_wheight_arr[(indice_index+i)*3    ] = vertices_joint_wheights_array[vertex_i*3    ];
                        vbo_vertices_joint_wheight_arr[(indice_index+i)*3 + 1] = vertices_joint_wheights_array[vertex_i*3 + 1];
                        vbo_vertices_joint_wheight_arr[(indice_index+i)*3 + 2] = vertices_joint_wheights_array[vertex_i*3 + 2];
                    }
                    
                    // // normal cull face
                    // indices_array[indice_index    ] = indice_index;
                    // indices_array[indice_index + 1] = indice_index + 1;
                    // indices_array[indice_index + 2] = indice_index + 2;
                    
                    // reverse cull face
                    indices_array[indice_index    ] = indice_index + 1;
                    indices_array[indice_index + 1] = indice_index;
                    indices_array[indice_index + 2] = indice_index + 2;

                    indice_index += 3;
                }
            }

            
            vbo_data_t vbo_datas_arr[5] = {
                {
                    .data_arr_size = sizeof(float)*triangles_count*3*3,
                    .data_arr = (void*)vbo_vertices_position_arr,
                    .size = 3,
                    .type = GL_FLOAT,
                    .stride = 3*sizeof(float),
                    .divisor = 0
                },
                {
                    .data_arr_size = sizeof(float)*triangles_count*3*2,
                    .data_arr = (void*)vbo_vertices_texcoord_arr,
                    .size = 2,
                    .type = GL_FLOAT,
                    .stride = 2*sizeof(float),
                    .divisor = 0
                },
                {
                    .data_arr_size = sizeof(float)*triangles_count*3*3,
                    .data_arr = (void*)vbo_vertices_normal_arr,
                    .size = 3,
                    .type = GL_FLOAT,
                    .stride = 3*sizeof(float),
                    .divisor = 0
                },
                {
                    .data_arr_size = sizeof(int32_t)*triangles_count*3*3,
                    .data_arr = (void*)vbo_vertices_joint_id_arr,
                    .size = 3,
                    .type = GL_INT,
                    .stride = 3*sizeof(int32_t),
                    .divisor = 0
                },
                {
                    .data_arr_size = sizeof(float)*triangles_count*3*3,
                    .data_arr = (void*)vbo_vertices_joint_wheight_arr,
                    .size = 3,
                    .type = GL_FLOAT,
                    .stride = 3*sizeof(float),
                    .divisor = 0
                }
            };

            mesh = generate_mesh(vbo_datas_arr, 5, indices_array, triangles_count*3);


            // mesh->joints
            str_i = str_find_substr(dae_str, "skin-joints-array\" count=\"");
            str_i += strlen("skin-joints-array\" count=\"");
            mesh->joints_amount = atoi(&(dae_str[str_i]));
            str_i += str_find_substr(&(dae_str[str_i]), ">");
            str_i += strlen(">");
            mesh->joints = malloc(sizeof(joint_t)*mesh->joints_amount);
            if (mesh->joints == NULL) {
                // give up on joints :(
                goto clean_and_return;
            }
            // joint names
            for (i = 0; i < mesh->joints_amount; i++) {
                mesh->joints[i].index = i;

                dstr_i = int64min(str_find_substr(&(dae_str[str_i]), " "), str_find_substr(&(dae_str[str_i]), "</Name_array>"));
                mesh->joints[i].name = malloc(sizeof(char)*(dstr_i+1));
                if (mesh->joints[i].name != NULL) {
                    memcpy(mesh->joints[i].name, &(dae_str[str_i]), dstr_i);
                    mesh->joints[i].name[dstr_i] = '\0';
                }
                str_i += dstr_i+1;
            }
            // joint inverse bind matrices
            str_i = str_find_substr(dae_str, "skin-bind_poses-array\" count=\"");
            str_i += strlen("skin-bind_poses-array\" count=\"");
            joints_inverse_bind_matrices_array_length = atoi(&(dae_str[str_i]));
            str_i += str_find_substr(&(dae_str[str_i]), ">");
            str_i += strlen(">");
            joints_inverse_bind_matrices_array = str_to_float_array(&(dae_str[str_i]), joints_inverse_bind_matrices_array_length);
            if (joints_inverse_bind_matrices_array == NULL) {
                goto clean_and_return;
            }
            for (i = 0; i < mesh->joints_amount; i++) {
                memcpy(mesh->joints[i].inverse_bind_transform_mat.mat, &(joints_inverse_bind_matrices_array[i*16]), sizeof(float)*16);
            }
            // joint hierarchy
            mesh_from_collada_dae_joint_hierarchy(dae_str, mesh);

            
            goto clean_and_return;
            clean_and_return: {
                if (dae_str != NULL)                            {free(dae_str);}
                if (vertices_positions_array != NULL)           {free(vertices_positions_array);}
                if (vertices_normals_array != NULL)             {free(vertices_normals_array);}
                if (vertices_texcoords_array != NULL)           {free(vertices_texcoords_array);}
                if (vertices_joint_ids_array != NULL)           {free(vertices_joint_ids_array);}
                if (vertices_joint_wheights_array != NULL)      {free(vertices_joint_wheights_array);}
                if (joints_data_count_array != NULL)            {free(joints_data_count_array);}
                if (joints_data_array != NULL)                  {free(joints_data_array);}
                if (joint_wheights_array != NULL)               {free(joint_wheights_array);}
                if (indices_array != NULL)                      {free(indices_array);}
                if (vbo_vertices_position_arr != NULL)          {free(vbo_vertices_position_arr);}
                if (vbo_vertices_texcoord_arr != NULL)          {free(vbo_vertices_texcoord_arr);}
                if (vbo_vertices_normal_arr != NULL)            {free(vbo_vertices_normal_arr);}
                if (vbo_vertices_joint_id_arr != NULL)          {free(vbo_vertices_joint_id_arr);}
                if (vbo_vertices_joint_wheight_arr != NULL)     {free(vbo_vertices_joint_wheight_arr);}
                if (triangles_data_arr != NULL)                 {free(triangles_data_arr);}
                if (joints_inverse_bind_matrices_array != NULL) {free(joints_inverse_bind_matrices_array);}
                
                return mesh;
            }
        }
        mesh_t* mesh_from_collada_dae(const char* dae_file_path) {
            quat_vec_vec_t transform_qvv = (quat_vec_vec_t){
                .rot = (quat_t){
                    .w = 1,
                    .x = 0,
                    .y = 0,
                    .z = 0
                },
                .pos = (vec3_t){
                    .x = 0,
                    .y = 0,
                    .z = 0,
                },
                .scale = (vec3_t){
                    .x = 1,
                    .y = 1,
                    .z = 1,
                }
            };
            return mesh_from_collada_dae_ext(dae_file_path, transform_qvv);
        }
        animation_t* animation_from_collada_dae_ext(const char* dae_file_path, joint_t* joints, uint32_t joints_amount, quat_vec_vec_t transform_qvv) {
            if (animations_amount >= ANIMATIONS_MAX_AMOUNT) {
                return NULL;
            }

            char* dae_str;
            uint64_t file_length = load_file_contents(&dae_str,dae_file_path);
            if (dae_str == NULL) {
                return NULL;
            }
            
            animation_t* anim;

            uint32_t i;

            int64_t str_i;
            int64_t dstr_i;

            char* name;
            int32_t joint_i;

            float* key_frames_time_stamp;

            uint32_t key_frames_transform_matrices_array_length;
            float* key_frames_transform_matrices_array;

            mat4_t transform_mat = mat4_from_quat_vec_vec(transform_qvv);

            
            anim = malloc(sizeof(animation_t));
            if (anim == NULL) {
                printf("malloc failed at animation generation.\n");
                free(dae_str);
                return NULL;
            }

            anim->joints_amount = joints_amount;
            anim->joints_key_frames = calloc(1,sizeof(joint_key_frame_t)*joints_amount);

            // for every joint
            str_i = 0;
            while (1) {
                // find joint name
                dstr_i = str_find_substr(&(dae_str[str_i]), "<float_array id=\"Armature_ArmatureAction_001_");
                if (dstr_i == -1) {
                    break;
                }
                str_i += dstr_i;
                str_i += strlen("<float_array id=\"Armature_ArmatureAction_001_");
                dstr_i = str_find_substr(&(dae_str[str_i]), "_pose_matrix-input-array\" count=\"");
                name = malloc(sizeof(char)*(dstr_i+1));
                memcpy(name, &(dae_str[str_i]), dstr_i);
                name[dstr_i] = '\0';
                // find joint index from name
                joint_i = -1;
                for (i = 0; i < joints_amount; i++) {
                    if (strcmp(name, joints[i].name) == 0) {
                        joint_i = i;
                        break;
                    }
                }
                free(name);

                if (joint_i == -1) {
                    printf("could not find joint \"%s\"\n", name);
                    continue;
                }

                // find joints' keyframes amount
                str_i += dstr_i;
                str_i += strlen("_pose_matrix-input-array\" count=\"");
                anim->joints_key_frames[joint_i].key_frames_amount = atoi(&(dae_str[str_i]));
                anim->joints_key_frames[joint_i].key_frames = malloc(sizeof(key_frame_t)*anim->joints_key_frames[joint_i].key_frames_amount);

                str_i += str_find_substr(&(dae_str[str_i]), "\">");
                key_frames_time_stamp = str_to_float_array(&(dae_str[str_i]), anim->joints_key_frames[joint_i].key_frames_amount);

                for (i = 0; i < anim->joints_key_frames[joint_i].key_frames_amount; i++) {
                    anim->joints_key_frames[joint_i].key_frames[i].time_stamp = key_frames_time_stamp[i];
                }

                free(key_frames_time_stamp);

                // find joints' transforms
                str_i += str_find_substr(&(dae_str[str_i]), "_pose_matrix-output-array\" count=\"");
                str_i += strlen("_pose_matrix-output-array\" count=\"");
                key_frames_transform_matrices_array_length = atoi(&(dae_str[str_i]));
                str_i += str_find_substr(&(dae_str[str_i]), "\">");
                str_i += strlen("\">");
                key_frames_transform_matrices_array = str_to_float_array(&(dae_str[str_i]), key_frames_transform_matrices_array_length);
                for (i = 0; i < anim->joints_key_frames[joint_i].key_frames_amount; i++) {
                    memcpy(
                        anim->joints_key_frames[joint_i].key_frames[i].joint_local_transform.mat,
                        &(key_frames_transform_matrices_array[i*16]), sizeof(float)*16
                    );

                    // apply transform
                    if (joint_i == 0) {
                        anim->joints_key_frames[joint_i].key_frames[i].joint_local_transform = mat4_mul(
                            anim->joints_key_frames[joint_i].key_frames[i].joint_local_transform,
                            transform_mat
                        );
                    }

                    anim->joints_key_frames[joint_i].key_frames[i].joint_local_transform_qvv = quat_vec_vec_from_mat4(anim->joints_key_frames[joint_i].key_frames[i].joint_local_transform);
                }
                free(key_frames_transform_matrices_array);
            }

            // find local matrix for joints without animation
            for (joint_i = 0; joint_i < joints_amount; joint_i++) {
                if (anim->joints_key_frames[joint_i].key_frames_amount != 0) {
                    continue;
                }

                str_i = str_find_substr(dae_str, "<visual_scene");
                str_i += str_find_substr(&(dae_str[str_i]), joints[joint_i].name);
                str_i += str_find_substr(&(dae_str[str_i]), "<matrix");
                str_i += str_find_substr(&(dae_str[str_i]), ">");
                str_i += strlen(">");
                key_frames_transform_matrices_array = str_to_float_array(&(dae_str[str_i]), 16);
                
                anim->joints_key_frames[joint_i].key_frames_amount = 1;
                anim->joints_key_frames[joint_i].key_frames = malloc(sizeof(key_frame_t)*anim->joints_key_frames[joint_i].key_frames_amount);

                memcpy(
                    anim->joints_key_frames[joint_i].key_frames[0].joint_local_transform.mat,
                    key_frames_transform_matrices_array, sizeof(float)*16
                );
                
                // apply transform
                if (joint_i == 0) {
                    anim->joints_key_frames[joint_i].key_frames[0].joint_local_transform = mat4_mul(
                        anim->joints_key_frames[joint_i].key_frames[0].joint_local_transform,
                        transform_mat
                    );
                }
                
                anim->joints_key_frames[joint_i].key_frames[0].joint_local_transform_qvv = quat_vec_vec_from_mat4(anim->joints_key_frames[joint_i].key_frames[0].joint_local_transform);

                free(key_frames_transform_matrices_array);
            }

            free(dae_str);

            *((uint64_t*)&anim->animation_index) = animations_amount;
            animations_list[animations_amount] = anim;
            animations_amount += 1;
            
            return anim;
        }
        animation_t* animation_from_collada_dae(const char* dae_file_path, joint_t* joints, uint32_t joints_amount) {
            quat_vec_vec_t transform_qvv = (quat_vec_vec_t){
                .rot = (quat_t){
                    .w = 1,
                    .x = 0,
                    .y = 0,
                    .z = 0
                },
                .pos = (vec3_t){
                    .x = 0,
                    .y = 0,
                    .z = 0,
                },
                .scale = (vec3_t){
                    .x = 1,
                    .y = 1,
                    .z = 1,
                }
            };
            return animation_from_collada_dae_ext(dae_file_path, joints, joints_amount, transform_qvv);
        }
        // translates keyframes to achive local transform mat
        mat4_t static get_joint_local_transform_mat_at_time(animation_t* anim, uint32_t joint_i, float time_stamp) {
            joint_key_frame_t* joint_key_frame = &(anim->joints_key_frames[joint_i]);

            if (joint_key_frame->key_frames_amount == 0) {
                return (mat4_t){
                    .mat = {
                        1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1
                    }
                };
            }
            
            // under minimum time stamp
            if (joint_key_frame->key_frames_amount == 1 || time_stamp < joint_key_frame->key_frames[0].time_stamp) {
                return joint_key_frame->key_frames[0].joint_local_transform;
            }
            
            uint32_t key_frame_prev = 0;
            for (uint32_t i = 0; i < joint_key_frame->key_frames_amount; i++) {
                if (time_stamp < joint_key_frame->key_frames[i].time_stamp) {
                    break;
                }
                key_frame_prev = i;
            }

            // over maximum time stamp
            if (key_frame_prev == joint_key_frame->key_frames_amount - 1) {
                return joint_key_frame->key_frames[joint_key_frame->key_frames_amount - 1].joint_local_transform;
            }

            quat_vec_vec_t qvv1 = joint_key_frame->key_frames[key_frame_prev].joint_local_transform_qvv;
            quat_vec_vec_t qvv2 = joint_key_frame->key_frames[key_frame_prev + 1].joint_local_transform_qvv;
            float t_linear =
                (time_stamp - joint_key_frame->key_frames[key_frame_prev].time_stamp) /
                (joint_key_frame->key_frames[key_frame_prev + 1].time_stamp - joint_key_frame->key_frames[key_frame_prev].time_stamp);

            quat_vec_vec_t qvv_interpolated;
            qvv_interpolated.rot = quat_slerp(qvv1.rot, qvv2.rot, t_linear);
            qvv_interpolated.pos.x = qvv1.pos.x*(1-t_linear) + qvv2.pos.x*(t_linear);
            qvv_interpolated.pos.y = qvv1.pos.y*(1-t_linear) + qvv2.pos.y*(t_linear);
            qvv_interpolated.pos.z = qvv1.pos.z*(1-t_linear) + qvv2.pos.z*(t_linear);
            qvv_interpolated.scale.x = qvv1.scale.x*(1-t_linear) + qvv2.scale.x*(t_linear);
            qvv_interpolated.scale.y = qvv1.scale.y*(1-t_linear) + qvv2.scale.y*(t_linear);
            qvv_interpolated.scale.z = qvv1.scale.z*(1-t_linear) + qvv2.scale.z*(t_linear);

            return mat4_from_quat_vec_vec(qvv_interpolated);
        }
        void draw_mesh(mesh_t* mesh) {
            draw_mesh_instanced(mesh, 1);
        }
        void draw_mesh_instanced(mesh_t* mesh, uint32_t instance_count) {
            // if mesh has joints -> set all joint_transform_matrices to identity mat4
            if (mesh->joints_amount > 0) {
                float* joints_transform_matrices = malloc(sizeof(float)*16*mesh->joints_amount);
                if (joints_transform_matrices != NULL) {
                    for (uint32_t joint_i = 0; joint_i < mesh->joints_amount; joint_i++) {
                        joints_transform_matrices[joint_i*16    ] = 1;
                        joints_transform_matrices[joint_i*16 + 1] = 0;
                        joints_transform_matrices[joint_i*16 + 2] = 0;
                        joints_transform_matrices[joint_i*16 + 3] = 0;
                        
                        joints_transform_matrices[joint_i*16 + 4] = 0;
                        joints_transform_matrices[joint_i*16 + 5] = 1;
                        joints_transform_matrices[joint_i*16 + 6] = 0;
                        joints_transform_matrices[joint_i*16 + 7] = 0;
                        
                        joints_transform_matrices[joint_i*16 + 8] = 0;
                        joints_transform_matrices[joint_i*16 + 9] = 0;
                        joints_transform_matrices[joint_i*16 + 10] = 1;
                        joints_transform_matrices[joint_i*16 + 11] = 0;
                        
                        joints_transform_matrices[joint_i*16 + 12] = 0;
                        joints_transform_matrices[joint_i*16 + 13] = 0;
                        joints_transform_matrices[joint_i*16 + 14] = 0;
                        joints_transform_matrices[joint_i*16 + 15] = 1;
                    }
                    glUniformMatrix4fv(shaders_list[current_shader]->u_joint_matrices_loc, mesh->joints_amount, GL_FALSE, joints_transform_matrices);
                    free(joints_transform_matrices);
                }
            }

            // draw mesh
            if (instance_count == 1) {
                // draw not instanced
                glBindVertexArray(mesh->vao);
                glDrawElements(GL_TRIANGLES, mesh->indices_count, GL_UNSIGNED_INT, mesh->indices_array);
            }else {
                glBindVertexArray(mesh->vao);
                glDrawElementsInstanced(GL_TRIANGLES, mesh->indices_count, GL_UNSIGNED_INT, mesh->indices_array, instance_count);
            }
        }
        void draw_mesh_animated(mesh_t* mesh, animation_t* anim, float time_stamp) {
            draw_mesh_animated_instanced(mesh, anim, time_stamp, 1);
        }
        void draw_mesh_animated_instanced(mesh_t* mesh, animation_t* anim, float time_stamp, uint32_t instance_count) {
            if (mesh->joints_amount != anim->joints_amount) {
                draw_mesh(mesh);
                return;
            }

            uint32_t joint_i;
            joint_t* joint;
            mat4_t final_joint_transform_mat;
            float* final_joints_transform_matrices = malloc(sizeof(float)*16*anim->joints_amount);

            for (joint_i = 0; joint_i < anim->joints_amount; joint_i++) {
                joint = &(mesh->joints[joint_i]);
                if (joint->parent == joint_i) {
                    joint->model_transform_mat = get_joint_local_transform_mat_at_time(anim, joint_i, time_stamp);
                }else {
                    joint->model_transform_mat = mat4_mul(
                        mesh->joints[joint->parent].model_transform_mat,
                        get_joint_local_transform_mat_at_time(anim, joint_i, time_stamp)
                    );
                }
                
                final_joint_transform_mat = mat4_mul(joint->model_transform_mat, joint->inverse_bind_transform_mat);

                memcpy(&(final_joints_transform_matrices[joint_i*16]), final_joint_transform_mat.mat, sizeof(float)*16);
            }
            
            glUniformMatrix4fv(shaders_list[current_shader]->u_joint_matrices_loc, anim->joints_amount, GL_FALSE, final_joints_transform_matrices);

            free(final_joints_transform_matrices);

            // draw mesh
            if (instance_count == 1) {
                // draw not instanced
                glBindVertexArray(mesh->vao);
                glDrawElements(GL_TRIANGLES, mesh->indices_count, GL_UNSIGNED_INT, mesh->indices_array);
            }else {
                glBindVertexArray(mesh->vao);
                glDrawElementsInstanced(GL_TRIANGLES, mesh->indices_count, GL_UNSIGNED_INT, mesh->indices_array, instance_count);
            }
        }
        void destroy_mesh(mesh_t* mesh) {
            meshes_amount -= 1;

            // move (meshes_list[meshes_amount]) to position (mesh->mesh_index) at (meshes_list)
            mesh_t* last_mesh = meshes_list[meshes_amount];
            *((uint64_t*)&last_mesh->mesh_index) = mesh->mesh_index;
            meshes_list[mesh->mesh_index] = last_mesh;

            clean_mesh(mesh);
        }
        void destroy_animation(animation_t* anim) {
            animations_amount -= 1;

            // move (animations_list[animations_amount]) to position (anim->animation_index) at (animations_list)
            animation_t* last_anim = animations_list[animations_amount];
            *((uint64_t*)&last_anim->animation_index) = anim->animation_index;
            animations_list[anim->animation_index] = last_anim;

            clean_animation(anim);
        }
        void clean_mesh(mesh_t* mesh) {
            glDeleteBuffers(mesh->vbos_amount, mesh->vbos);
            glDeleteBuffers(1, &mesh->vao);
            free(mesh->vbos);
            free(mesh->indices_array);
            if (mesh->joints != NULL) {
                for (uint32_t i = 0; i < mesh->joints_amount; i++) {
                    if (mesh->joints[i].name != NULL) {
                        free(mesh->joints[i].name);
                    }
                }
                free(mesh->joints);
            }
            free(mesh);
            return;
        }
        void clean_animation(animation_t* anim) {
            for (uint32_t i = 0; i < anim->joints_amount; i++) {
                if (anim->joints_key_frames[i].key_frames != NULL) {
                    free(anim->joints_key_frames[i].key_frames);
                }
            }
            free(anim->joints_key_frames);
            free(anim);
        }
    // </meshes and animations>

    // <camera_t>
        camera_t* camera_create(float x, float y, float z,
                                    float rx, float ry, float rz,
                                    float width, float height, float depth,
                                    float near, float far,
                                    uint8_t is_prespective, float fov,
                                    float viewport_x, float viewport_y, float viewport_w, float viewport_h) {
            if (cameras_amount >= CAMERAS_MAX_AMOUNT){
                return NULL;
            }
            camera_t* camera = malloc(sizeof(camera_t));
            if (camera == NULL){
                return NULL;
            }

            camera->active = 1;

            camera->is_prespective = (is_prespective > 0);

            camera->x = x;
            camera->y = y;
            camera->z = z;

            camera->rx = rx;
            camera->ry = ry;
            camera->rz = rz;

            camera->width = width;
            camera->height = height;
            camera->depth = depth;

            camera->near = near;
            camera->far = far;
            camera->fov = fov;

            camera->viewport_x = viewport_x;
            camera->viewport_y = viewport_y;
            camera->viewport_w = viewport_w;
            camera->viewport_h = viewport_h;

            *((uint64_t*)(&camera->camera_index)) = cameras_amount;
            cameras_list[cameras_amount] = camera;
            cameras_amount += 1;
            return camera;
        }
        void camera_use(camera_t* camera) {
            current_camera = camera->camera_index;
            camera_update_fbo_viewport(camera);
        }
        void camera_update_fbo_viewport(camera_t* camera) {
            glViewport(
                camera->viewport_x,
                camera->viewport_y,
                camera->viewport_w,
                camera->viewport_h
            );
        }
        void camera_destroy(camera_t* camera) {
            if (current_camera != -1){
                printf("cannot destroy a camera during render period!\n");
                return;
            }

            cameras_amount -= 1;

            // move (cameras_list[cameras_amount]) to position (camera->camera_index) at (cameras_list)
            camera_t* last_cameras = cameras_list[cameras_amount];
            *((uint64_t*)&last_cameras->camera_index) = camera->camera_index;
            cameras_list[camera->camera_index] = last_cameras;

            camera_clean(camera);
        }
        void camera_clean(camera_t* camera) {
            free(camera);
            return;
        }
        void camera_update_world_view_projection_matrix(camera_t* camera) {
            #define cx camera->x
            #define cy camera->y
            #define cz camera->z
            #define crx camera->rx
            #define cry camera->ry
            #define crz camera->rz
            #define cn camera->near
            #define cf camera->far
            #define cw camera->width
            #define ch camera->height

            const mat4_t rotation_x_matrix = (mat4_t){
                .mat = {
                    1, 0,         0,        0,
                    0, cos(crx), -sin(crx), 0,
                    0, sin(crx),  cos(crx), 0,
                    0, 0,         0,        1
                }
            };
            const mat4_t rotation_y_matrix = (mat4_t){
                .mat = {
                     cos(cry), 0, sin(cry), 0,
                     0,        1, 0,        0,
                    -sin(cry), 0, cos(cry), 0,
                     0,        0, 0,        1
                }
            };
            const mat4_t rotation_z_matrix = (mat4_t){
                .mat = {
                    cos(crz), -sin(crz), 0, 0,
                    sin(crz),  cos(crz), 0, 0,
                    0,         0,        1, 0,
                    0,         0,        0, 1
                }
            };

            const mat4_t world_matrix = (mat4_t){
                .mat = {
                    1, 0, 0, -cx,
                    0, 1, 0, -cy,
                    0, 0, 1, -cz,
                    0, 0, 0, 1
                }
            };
            
            mat4_t projection_matrix;
            if (camera->is_prespective) {
                const float _s_ = 1/tan(camera->fov*M_PI*0.5/180);
                projection_matrix  = (mat4_t){
                    .mat = {
                        _s_*ch/(cw+ch), 0,              0,           0,
                        0,              _s_*cw/(cw+ch), 0,           0,
                        0,              0,              cf/(cf-cn), -cf*cn/(cf-cn),
                        0,              0,              1,           0
                    }
                };
            }else{
                projection_matrix  = (mat4_t){
                    .mat = {
                        2.0/cw, 0,      0,            0,
                        0,      2.0/ch, 0,            0,
                        0,      0,      2.0/(cf-cn), -(cf+cn)/(cf-cn),
                        0,      0,      0,            1
                    }
                };
            }
            
            // rotation_x_matrix
            // rotation_y_matrix
            // rotation_z_matrix
            
            // projection_matrix
            // world_matrix
            // view_matrix

            mat4_t view_matrix = mat4_mul(mat4_mul(rotation_z_matrix, rotation_x_matrix), rotation_y_matrix);

            camera->world_view_projection_matrix = mat4_mul(mat4_mul(projection_matrix, view_matrix), world_matrix);

            #undef cx
            #undef cy
            #undef cz
            #undef crx
            #undef cry
            #undef crz
            #undef cn
            #undef cf
            #undef cw
            #undef ch
            return;
        }
    // </camera_t>

    // <matrices vectors and quaternions>
        vec3_t vec3_mul(vec3_t vec1, vec3_t vec2) {
            return (vec3_t){
                .x = vec1.x*vec2.x,
                .y = vec1.y*vec2.y,
                .z = vec1.z*vec2.z
            };
        }
        vec3_t vec3_add(vec3_t vec1, vec3_t vec2) {
            return (vec3_t){
                .x = vec1.x+vec2.x,
                .y = vec1.y+vec2.y,
                .z = vec1.z+vec2.z
            };
        }
        float dot_product(vec3_t vec1, vec3_t vec2) {
            return (vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z);
        }
        vec3_t cross_product(vec3_t vec1, vec3_t vec2) {
            return (vec3_t){
                .x = vec1.y*vec2.z - vec1.z*vec2.y,
                .y = vec1.z*vec2.x - vec1.x*vec2.z,
                .z = vec1.x*vec2.y - vec1.y*vec2.x
            };
        }
        
        mat4_t mat4_from_mat3(mat3_t mat) {
            return (mat4_t){
                .mat = {
                    mat.mat[0], mat.mat[1], mat.mat[2], 0,
                    mat.mat[3], mat.mat[4], mat.mat[5], 0,
                    mat.mat[6], mat.mat[7], mat.mat[8], 0,
                    0,          0,          0,          1
                }
            };
        }
        // discards coloumn 3 and row 3
        mat3_t mat3_from_mat4(mat4_t mat) {
            return (mat3_t){
                mat.mat[0], mat.mat[1], mat.mat[2],
                mat.mat[4], mat.mat[5], mat.mat[6],
                mat.mat[8], mat.mat[9], mat.mat[10]
            };
        }
        //  assumes:
        //      mat1 width  = mat2 height = m
        //      mat3 width  = mat2 width  = n
        //      mat3 height = mat1 height = i
        void mat_mul(float* mat1, float* mat2, float* restrict mat3, uint8_t m, uint8_t i, uint8_t n) {
            // https://en.wikipedia.org/wiki/Matrix_multiplication
            for (uint8_t j = 0; j < i; j++) {
                for (uint8_t k = 0; k < n; k++) {
                    float sum = 0;
                    for (uint8_t l = 0; l < m; l++) {
                        sum += mat1[j*m+l]*mat2[l*n+k];
                    }
                    mat3[j*n+k] = sum;
                }
            }
            return;
        }
        mat3_t mat3_mul(mat3_t mat1, mat3_t mat2) {
            mat3_t res_mat;
            mat_mul(mat1.mat, mat2.mat, res_mat.mat, 3, 3, 3);
            return res_mat;
        }
        mat4_t mat4_mul(mat4_t mat1, mat4_t mat2) {
            mat4_t res_mat;
            mat_mul(mat1.mat, mat2.mat, res_mat.mat, 4, 4, 4);
            return res_mat;
        }
        vec3_t vec3_mul_by_mat3(vec3_t vec, mat3_t mat) {
            float vec_mat[3] = {
                vec.x,
                vec.y,
                vec.z
            };

            float res_mat[3];
            mat_mul(mat.mat, vec_mat, res_mat, 3, 3, 1);
            
            return (vec3_t){
                .x = res_mat[0],
                .y = res_mat[1],
                .z = res_mat[2]
            };
        }
        mat3_t mat3_mul_by_scalar(mat3_t mat, float scalar) {
            mat3_t res;
            for (uint8_t i = 0; i < 9; i++) {
                res.mat[i] = mat.mat[i]*scalar;
            }
            return res;
        }
        mat4_t mat4_mul_by_scalar(mat4_t mat, float scalar) {
            mat4_t res;
            for (uint8_t i = 0; i < 16; i++) {
                res.mat[i] = mat.mat[i]*scalar;
            }
            return res;
        }
        //  assumes:
        //      mat1 width  = mat2 width  = mat3 width  = w
        //      mat1 height = mat3 height = mat3 height = h 
        void mat_add(float* mat1, float* mat2, float* mat3, uint8_t w, uint8_t h) {
            for (uint32_t i = 0; i < w*h; i++) {
                mat3[i] = mat1[i]+mat2[i];
            }
            return;
        }
        mat3_t mat3_add(mat3_t mat1, mat3_t mat2) {
            mat3_t res_mat;
            mat_add(mat1.mat, mat2.mat, res_mat.mat, 3, 3);
            return res_mat;
        }
        mat4_t mat4_add(mat4_t mat1, mat4_t mat2) {
            mat4_t res_mat;
            mat_add(mat1.mat, mat2.mat, res_mat.mat, 4, 4);
            return res_mat;
        }
        // mat must be of size 3x3
        mat3_t mat3_from_axis_angle(vec3_t axis, float ang) {
            float cosa = cos(ang);
            float sina = sin(ang);
            float omca = (1-cosa); // 1-cos(a)

            float xy = axis.x*axis.y;
            float xz = axis.x*axis.z;
            float yz = axis.y*axis.z;
            
            return (mat3_t){
                .mat = {
                    cosa+axis.x*axis.x*omca, xy*omca-axis.z*sina,     xz*omca+axis.y*sina,
                    xy*omca+axis.z*sina,     cosa+axis.y*axis.y*omca, yz*omca-axis.x*sina,
                    xz*omca-axis.y*sina,     yz*omca+axis.x*sina,     cosa+axis.z*axis.z*omca
                }
            };
        }

        quat_t quat_multiply(quat_t q1, quat_t q2) {
            return (quat_t){
                .w = (q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z),
                .x = (q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y),
                .y = (q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x),
                .z = (q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w)
            };
        }
        quat_t quat_conjugate(quat_t q) {
            return (quat_t){
                .w = q.w,
                .x = -q.x,
                .y = -q.y,
                .z = -q.z
            };
        }
        quat_t quat_slerp(quat_t q1, quat_t q2, float t) {
            // https://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
            
            // Calculate angle between them
            float cos_half_theta = q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;
            
            // if qa=qb or qa=-qb then theta = 0 and we can return qa
            if (cos_half_theta >= 1) {
                return q1;
            }

	        // Calculate temporary values
            float half_theta = acos(cos_half_theta);
            float sin_half_theta = sin(half_theta);
            
            // if theta = 180 degrees then result is not fully defined
            // we could rotate around any axis normal to qa or qb
            if (fabs(sin_half_theta) < 0.001){ // fabs is floating point absolute
                return (quat_t) {
                    .w = (q1.w*0.5 + q2.w*0.5),
                    .x = (q1.x*0.5 + q2.x*0.5),
                    .y = (q1.y*0.5 + q2.y*0.5),
                    .z = (q1.z*0.5 + q2.z*0.5)
                };
            }

            float ratio_a = sin((1 - t) * half_theta) / sin_half_theta;
            float ratio_b = sin(t * half_theta) / sin_half_theta; 
            //calculate Quaternion.
            return (quat_t) {
                .w = (q1.w*ratio_a + q2.w*ratio_b),
                .x = (q1.x*ratio_a + q2.x*ratio_b),
                .y = (q1.y*ratio_a + q2.y*ratio_b),
                .z = (q1.z*ratio_a + q2.z*ratio_b)
            };
        }
        quat_t quat_from_axis_angle(vec3_t vec, float ang) {
            float sina = sin(ang*0.5);
            float i_mag = 1/sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
            return (quat_t){
                .w = cos(ang*0.5),
                .x = vec.x*i_mag*sina,
                .y = vec.y*i_mag*sina,
                .z = vec.z*i_mag*sina,
            };
        }
        quat_t quat_from_euler_angles(float roll, float pitch, float yaw) {
            float sinroll = sin(roll*0.5);
            float cosroll = cos(roll*0.5);
            float sinpitch = sin(pitch*0.5);
            float cospitch = cos(pitch*0.5);
            float sinyaw = sin(yaw*0.5);
            float cosyaw = cos(yaw*0.5);
            return (quat_t){
                .w = cosroll*cospitch*cosyaw + sinroll*sinpitch*sinyaw,
                .x = sinroll*cospitch*cosyaw - cosroll*sinpitch*sinyaw,
                .z = cosroll*sinpitch*cosyaw + sinroll*cospitch*sinyaw,
                .y = cosroll*cospitch*sinyaw - sinroll*sinpitch*cosyaw
            };
        }
        quat_t quat_from_axis_angles_yzx(float rx, float ry, float rz) {
            quat_t qx = (quat_t){
                .w = cos(rx*0.5),
                .x = sin(rx*0.5),
                .y = 0,
                .z = 0
            };
            quat_t qy = (quat_t){
                .w = cos(ry*0.5),
                .x = 0,
                .y = sin(ry*0.5),
                .z = 0
            };
            quat_t qz = (quat_t){
                .w = cos(rz*0.5),
                .x = 0,
                .y = 0,
                .z = sin(rz*0.5)
            };
            return quat_multiply(qy,quat_multiply(qz,qx));
        }
        vec3_t rotate_vector(vec3_t vec, quat_t q) {
            quat_t q_vec = (quat_t){
                .w = 0,
                .x = vec.x,
                .y = vec.y,
                .z = vec.z
            };
            quat_t q_res = quat_multiply(quat_multiply(q,q_vec),quat_conjugate(q));
            return (vec3_t){
                .x = q_res.x,
                .y = q_res.y,
                .z = q_res.z
            };
        }
        vec3_t vec_scale_rotate_translate(vec3_t vec, quat_vec_vec_t qvv) {
            return vec3_add(rotate_vector(vec3_mul(vec, qvv.scale),qvv.rot),qvv.pos);
        }
        mat3_t rot_mat3_from_quat(quat_t q) {
            float wx = 2*q.w*q.x;
            float wy = 2*q.w*q.y;
            float wz = 2*q.w*q.z;
            float xx = 2*q.x*q.x;
            float xy = 2*q.x*q.y;
            float xz = 2*q.x*q.z;
            float yy = 2*q.y*q.y;
            float yz = 2*q.y*q.z;
            float zz = 2*q.z*q.z;

            return (mat3_t){
                .mat = {
                    1-(yy+zz), xy-wz,     xz+wy,
                    xy+wz,     1-(xx+zz), yz-wx,
                    xz-wy,     yz+wx,     1-(xx+yy)
                }
            };
        }
        quat_t quat_from_rot_mat3(mat3_t mat) {
            float w = sqrt(1 + mat.mat[0] + mat.mat[4] + mat.mat[8])*0.5;
            float iw4 = 1/(4*w);

            return (quat_t){
                .w = w,
                .x = (mat.mat[7] - mat.mat[5])*iw4,
                .y = (mat.mat[2] - mat.mat[6])*iw4,
                .z = (mat.mat[3] - mat.mat[1])*iw4,
            };
        }
        quat_vec_vec_t quat_vec_vec_from_mat4(mat4_t mat) {
            return (quat_vec_vec_t){
                .pos = (vec3_t){
                    .x = mat.mat[3],
                    .y = mat.mat[7],
                    .z = mat.mat[11]
                },
                .scale = (vec3_t){
                    .x = sqrt(mat.mat[0]*mat.mat[0] + mat.mat[4]*mat.mat[4] + mat.mat[8]*mat.mat[8]),
                    .y = sqrt(mat.mat[1]*mat.mat[1] + mat.mat[5]*mat.mat[5] + mat.mat[9]*mat.mat[9]),
                    .z = sqrt(mat.mat[2]*mat.mat[2] + mat.mat[6]*mat.mat[6] + mat.mat[10]*mat.mat[10])
                },
                .rot = quat_from_rot_mat3(mat3_from_mat4(mat))
            };
        }
        mat4_t mat4_from_quat_vec_vec(quat_vec_vec_t qvv) {
            mat4_t rot = mat4_from_mat3(rot_mat3_from_quat(qvv.rot));
            mat4_t pos = (mat4_t){
                .mat = {
                    1, 0, 0, qvv.pos.x,
                    0, 1, 0, qvv.pos.y,
                    0, 0, 1, qvv.pos.z,
                    0, 0, 0, 1
                }
            };
            mat4_t scale = (mat4_t){
                .mat = {
                    qvv.scale.x, 0, 0, 0,
                    0, qvv.scale.y, 0, 0,
                    0, 0, qvv.scale.z, 0,
                    0, 0, 0, 1
                }
            };

            return mat4_mul(mat4_mul(pos, rot), scale);
        }

        void print_mat3(mat3_t mat) {
            printf(
                "[\n\t%f\t%f\t%f\n\t%f\t%f\t%f\n\t%f\t%f\t%f\n]\n",
                mat.mat[0], mat.mat[1], mat.mat[2],
                mat.mat[3], mat.mat[4], mat.mat[5],
                mat.mat[6], mat.mat[7], mat.mat[8]
            );
        }
        void print_mat4(mat4_t mat) {
            printf(
                "[\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n]\n",
                mat.mat[0],  mat.mat[1],  mat.mat[2],  mat.mat[3],
                mat.mat[4],  mat.mat[5],  mat.mat[6],  mat.mat[7],
                mat.mat[8],  mat.mat[9],  mat.mat[10], mat.mat[11],
                mat.mat[12], mat.mat[13], mat.mat[14], mat.mat[15]
            );
        }
        void print_quat_vec_vec(quat_vec_vec_t qvv) {
            printf(
                "{\n\trot :\t%f\t%f\t%f\t%f\n\tscale :\t%f\t%f\t%f\n\tpos :\t%f\t%f\t%f\n}\n",
                qvv.rot.w,
                qvv.rot.x,
                qvv.rot.y,
                qvv.rot.z,
                qvv.scale.x,
                qvv.scale.y,
                qvv.scale.z,
                qvv.pos.x,
                qvv.pos.y,
                qvv.pos.z
            );
        }
    // </matrices vectors and quats>

    // <simple draw module>
        void simple_draw_module_set_color(float r, float g, float b, float a) {
            simple_draw_module_color_r = r;
            simple_draw_module_color_g = g;
            simple_draw_module_color_b = b;
            simple_draw_module_color_a = a;
            return;
        }
        void simple_draw_module_draw_rect(float x, float y, float w, float h) {
            int64_t last_shader = current_shader;

            glDisable(GL_CULL_FACE);

            shader_use((shader_t*)simple_draw_module_cube_shader);
            glUniform3f(simple_draw_module_cube_shader->uniform_locations[0], x, y, 0);
            glUniform3f(simple_draw_module_cube_shader->uniform_locations[1], w, h, 0);
            glUniform4f(simple_draw_module_cube_shader->uniform_locations[2],
                simple_draw_module_color_r,
                simple_draw_module_color_g,
                simple_draw_module_color_b,
                simple_draw_module_color_a
           );
            draw_mesh((mesh_t*)simple_draw_module_rectangle_mesh);

            glEnable(GL_CULL_FACE);

            shader_use(shaders_list[last_shader]);
            return;
        }
        void simple_draw_module_draw_cube(float x, float y, float z, float w, float h, float d) {
            int64_t last_shader = current_shader;

            glDisable(GL_CULL_FACE);

            shader_use((shader_t*)simple_draw_module_cube_shader);
            glUniform3f(simple_draw_module_cube_shader->uniform_locations[0], x, y, z);
            glUniform3f(simple_draw_module_cube_shader->uniform_locations[1], w, h, d);
            glUniform4f(simple_draw_module_cube_shader->uniform_locations[2],
                simple_draw_module_color_r,
                simple_draw_module_color_g,
                simple_draw_module_color_b,
                simple_draw_module_color_a
            );
            draw_mesh((mesh_t*)simple_draw_module_cube_mesh);

            glEnable(GL_CULL_FACE);

            shader_use(shaders_list[last_shader]);
            return;
        }
        void simple_draw_module_draw_ball(float x, float y, float z, float r) {
            int64_t last_shader = current_shader;

            glDisable(GL_CULL_FACE);

            shader_use((shader_t*)simple_draw_module_cube_shader);
            glUniform3f(simple_draw_module_cube_shader->uniform_locations[0], x, y, z);
            glUniform3f(simple_draw_module_cube_shader->uniform_locations[1], r, r, r);
            glUniform4f(simple_draw_module_cube_shader->uniform_locations[2],
                simple_draw_module_color_r,
                simple_draw_module_color_g,
                simple_draw_module_color_b,
                simple_draw_module_color_a
           );
            draw_mesh((mesh_t*)simple_draw_module_ball_mesh);

            glEnable(GL_CULL_FACE);

            shader_use(shaders_list[last_shader]);
            return;
        }
    // </simple draw module>
    
// </functions>