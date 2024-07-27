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
                draw_with_mesh((mesh_t*)screen_quad_mesh);
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

    // <mesh_t>
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
                glVertexAttribPointer(i, vbo_datas_arr[i].size, vbo_datas_arr[i].type, GL_FALSE, vbo_datas_arr[i].stride, (void*)0);
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
        mesh_t* generate_mesh_from_wavefront_obj(const char* obj_file_path) {
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
                            indices_array[polygon_index*3 + line_space_count] = unique_vertex_index;
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
        void draw_with_mesh(mesh_t* mesh) {
            glBindVertexArray(mesh->vao);
            glDrawElements(GL_TRIANGLES, mesh->indices_count, GL_UNSIGNED_INT, mesh->indices_array);
            return;
        }
        void draw_instanced_with_mesh(mesh_t* mesh, uint32_t instance_count) {
            glBindVertexArray(mesh->vao);
            glDrawElementsInstanced(GL_TRIANGLES, mesh->indices_count, GL_UNSIGNED_INT, mesh->indices_array, instance_count);
        }
        void destroy_mesh(mesh_t* mesh) {
            meshes_amount -= 1;

            // move (meshes_list[meshes_amount]) to position (mesh->mesh_index) at (meshes_list)
            mesh_t* last_mesh = meshes_list[meshes_amount];
            *((uint64_t*)&last_mesh->mesh_index) = mesh->mesh_index;
            meshes_list[mesh->mesh_index] = last_mesh;

            clean_mesh(mesh);
        }
        void clean_mesh(mesh_t* mesh) {
            //printf("\tdeleting mesh->vbos buffers with %u vbos_amount\n", mesh->vbos_amount);
            glDeleteBuffers(mesh->vbos_amount, mesh->vbos);
            //printf("\tdeleting mesh->vao buffer\n");
            glDeleteBuffers(1, &mesh->vao);
            //printf("\tfreeing mesh->vbos\n");
            free(mesh->vbos);
            //printf("\tfreeing mesh->indices_array\n");
            free(mesh->indices_array);
            //printf("\tfreeing mesh\n");
            free(mesh);
            return;
        }
    // </mesh_t>

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

            camera->world_view_projection_matrix = matrix_create((uvec2_t){4,4});

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
            free(camera->world_view_projection_matrix.mat);
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

            const mat_t rotation_x_matrix = (mat_t){
                .size = (uvec2_t){
                    .x = 4,
                    .y = 4
                },
                .mat = (float[16]){
                    1, 0,         0,        0,
                    0, cos(crx), -sin(crx), 0,
                    0, sin(crx),  cos(crx), 0,
                    0, 0,         0,        1
                }
            };
            
            const mat_t rotation_y_matrix = (mat_t){
                .size = (uvec2_t){
                    .x = 4,
                    .y = 4
                },
                .mat = (float[16]){
                     cos(cry), 0, sin(cry), 0,
                     0,        1, 0,        0,
                    -sin(cry), 0, cos(cry), 0,
                     0,        0, 0,        1
                }
            };
            
            const mat_t rotation_z_matrix = (mat_t){
                .size = (uvec2_t){
                    .x = 4,
                    .y = 4
                },
                .mat = (float[16]){
                    cos(crz), -sin(crz), 0, 0,
                    sin(crz),  cos(crz), 0, 0,
                    0,         0,        1, 0,
                    0,         0,        0, 1
                }
            };
            
            const mat_t world_matrix = (mat_t){
                .size = (uvec2_t){
                    .x = 4,
                    .y = 4
                },
                .mat = (float[16]){
                    1, 0, 0, -cx,
                    0, 1, 0, -cy,
                    0, 0, 1, -cz,
                    0, 0, 0, 1
                }
            };

            mat_t projection_matrix;
            if (camera->is_prespective) {
                const float _s_ = 1/tan(camera->fov*M_PI*0.5/180);
                projection_matrix  = (mat_t){
                    .size = (uvec2_t){
                        .x = 4,
                        .y = 4
                    },
                    .mat = (float[16]){
                        _s_*ch/(cw+ch), 0,              0,           0,
                        0,              _s_*cw/(cw+ch), 0,           0,
                        0,              0,              cf/(cf-cn), -cf*cn/(cf-cn),
                        0,              0,              1,           0
                    }
                };
            }else{
                projection_matrix  = (mat_t){
                    .size = (uvec2_t){
                        .x = 4,
                        .y = 4
                    },
                    .mat = (float[16]){
                        2.0/cw, 0,      0,            0,
                        0,      2.0/ch, 0,            0,
                        0,      0,      2.0/(cf-cn), -(cf+cn)/(cf-cn),
                        0,      0,      0,            1
                    }
                };
            }

            mat_t view_matrix = matrix_create((uvec2_t){4,4});

            mat_t ping_pong_matrix1 = matrix_create((uvec2_t){4,4});
            mat_t ping_pong_matrix2 = matrix_create((uvec2_t){4,4});
            
            // rotation_x_matrix
            // rotation_y_matrix
            // rotation_z_matrix
            
            // projection_matrix
            // world_matrix
            // view_matrix

            matrix_multiply((mat_t*)&rotation_x_matrix, (mat_t*)&rotation_y_matrix, &ping_pong_matrix1);
            matrix_multiply((mat_t*)&rotation_z_matrix, &ping_pong_matrix1, &view_matrix);

            matrix_multiply((mat_t*)&view_matrix, (mat_t*)&world_matrix, &ping_pong_matrix1);
            matrix_multiply(&projection_matrix, &ping_pong_matrix1, &ping_pong_matrix2);

            memcpy(camera->world_view_projection_matrix.mat, ping_pong_matrix2.mat, sizeof(float)*16);

            free(view_matrix.mat);
            free(ping_pong_matrix1.mat);
            free(ping_pong_matrix2.mat);

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

    // <matrices>
        mat_t inline matrix_create(uvec2_t size) {
            return
            (
                (mat_t){
                    .size = size,
                    .mat = calloc(1,sizeof(float)*size.y*size.x)
                }
           );
        }
        void matrix_multiply(mat_t* mat1, mat_t* mat2, mat_t* restrict mat3) {
            //  assumes:
            //      mat1->size.x == mat2->size.y
            //      mat3->size.x == mat2->size.x
            //      mat3->size.y == mat1->size.y
            
            for (uint32_t i = 0; i < mat3->size.y; i++) {
                for (uint32_t j = 0; j < mat3->size.x; j++) {
                    float sum = 0;
                    for (uint32_t k = 0; k < mat1->size.x; k++) {
                        sum += mat1->mat[i*mat1->size.x+k]*mat2->mat[k*mat2->size.x+j];
                    }
                    mat3->mat[i*mat3->size.x+j] = sum;
                }
            }

            return;
        }
        void matrix_multiply_by_scalar(mat_t* mat1, float scalar, mat_t* mat2) {
            //  assumes:
            //      mat1->size == mat2->size

            for (uint32_t y = 0; y < mat1->size.y; y++) {
                for (uint32_t x = 0; y < mat1->size.x; x++) {
                    mat2->mat[y*mat1->size.x+x] = mat1->mat[y*mat1->size.x+x]*scalar;
                }
            }
            return;
        }
        void matrix_add(mat_t* mat1, mat_t* mat2, mat_t* mat3) {
            //  assumes:
            //      mat1->size == mat2->size
            //      mat2->size == mat3->size

            for (uint32_t y = 0; y < mat1->size.y; y++) {
                for (uint32_t x = 0; y < mat1->size.x; x++) {
                    mat3->mat[y*mat1->size.x+x] = mat1->mat[y*mat1->size.x+x] + mat2->mat[y*mat1->size.x+x];
                }
            }
            return;
        }
    // </matrices>

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
            draw_with_mesh((mesh_t*)simple_draw_module_rectangle_mesh);

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
            draw_with_mesh((mesh_t*)simple_draw_module_cube_mesh);

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
            draw_with_mesh((mesh_t*)simple_draw_module_ball_mesh);

            glEnable(GL_CULL_FACE);

            shader_use(shaders_list[last_shader]);
            return;
        }
    // </simple draw module>
    
// </functions>