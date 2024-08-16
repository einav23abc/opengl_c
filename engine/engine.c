#include "engine.h"

__attribute__((weak)) uint32_t init();
__attribute__((weak)) void update();
__attribute__((weak)) void render();
__attribute__((weak)) void handle_event();
__attribute__((weak)) void clean();


const SDL_Event event;

const float TARGET_FRAME_DELAY;

uint8_t keys[SDL_NUM_SCANCODES]; // ticks since key pressed; 0 if released

uint8_t running;
uint32_t frame_start_time;
const uint32_t delta_time;
const float delta_frames;
const uint32_t time;
const float frames;
#ifdef DEBUG_MODE
static uint32_t acum_frames_time;
static uint32_t acum_frames_amount;
#endif

const SDL_GLContext context;

SDL_Window* window;
int32_t window_width;
int32_t window_height;
int32_t window_drawable_width;
int32_t window_drawable_height;

const shader_t* default_shader;


#include "./def_tex_surf.c"
const texture_t* default_texture;


// overload for `main`
int APIENTRY WinMain (HINSTANCE p1, HINSTANCE p2, LPSTR p3, int p4) {
    main(0, NULL);
}

int32_t main(int32_t argc, char** argv) {
    // hide console
    #ifndef DEBUG_MODE
    #ifndef DEBUG_SOFT_MODE
    FreeConsole();
    #endif
    #endif

    #ifdef DEBUG_MODE
    if (init == NULL) printf("init() does not exist\n");
    if (render == NULL) printf("render() does not exist\n");
    if (update == NULL) printf("update() does not exist\n");
    if (handle_event == NULL) printf("handle_event() does not exist\n");
    if (clean == NULL) printf("clean() does not exist\n");
    #endif
    
    uint32_t init_result = engine_init();
    if (init_result != 0) return init_result;
    
    *((float*)&TARGET_FRAME_DELAY) = (1000/TARGET_FRAME_RATE);
    *((uint32_t*)&delta_time) = 0;
    *((float*)&delta_frames)  = 0;
    *((uint32_t*)&time) = 0;
    *((float*)&frames)  = 0;
    #ifdef DEBUG_MODE
    acum_frames_time = 0;
    acum_frames_amount = 0;
    #endif

    running = 1;
    while(running){
        frame_start_time = SDL_GetTicks();

        while (SDL_PollEvent((SDL_Event*)&event)) engine_handle_event();
        
        engine_update();
        engine_render();
        
        *((uint32_t*)&delta_time) = SDL_GetTicks() - frame_start_time;
        *((float*)&delta_frames)  = ((float)delta_time)/TARGET_FRAME_DELAY;
        *((uint32_t*)&time) += delta_time;
        *((float*)&frames)   = ((float)time)/TARGET_FRAME_DELAY;
        
        #ifdef DEBUG_MODE
        acum_frames_amount += 1;
        acum_frames_time += delta_time;
        if (acum_frames_time >= 1000) {
            printf("Avrage fps in the last seccond:%d\n", 1000*acum_frames_amount/acum_frames_time);
            acum_frames_amount = 0;
            acum_frames_time = 0;
        }
        #endif
    }
    
    engine_clean();

    #ifdef DEBUG_MODE
    printf("ended sucessfully");
    #endif
    return 0;
}

uint32_t backend_init() {
    // init sdl
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Failed to initialize SDL\n");
        return 1;
    }

    // load default opengl dynamic library
    if (SDL_GL_LoadLibrary(NULL) != 0) {
        printf("Failed to dynamically load an OpenGL library\n");
        SDL_Quit();
        return 1;
    }

    // set context attributes - OpenGL 4.5 context (should be core)
    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) != 0) {
        printf("error\n");
        SDL_Quit();
        return 1;
    }
    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5) != 0) {
        printf("error\n");
        SDL_Quit();
        return 1;
    }
    if(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0) {
        printf("error\n");
        SDL_Quit();
        return 1;
    }
    if(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0) {
        printf("error\n");
        SDL_Quit();
        return 1;
    }

    // create window
    window = SDL_CreateWindow(
        WINDOW_TITLE,
        WINDOW_START_X,
        WINDOW_START_Y,
        WINDOW_START_WIDTH,
        WINDOW_START_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (window == NULL) {
        SDL_Quit();
        printf("Failed to create window\n");
        return 1;
    }

    // init rendering context
    *((SDL_GLContext*)&context) = SDL_GL_CreateContext(window);
    if (context == NULL) {
        printf("Failed to create GL context\n");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // make context current (should be current anyway)
    if (SDL_GL_MakeCurrent(window, context) != 0) {
        printf("Failed to make context current\n");
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // retrieve GL functions
    if (gladLoadGLLoader(SDL_GL_GetProcAddress) == 0) {
        printf("Failed to retrieve GL functions\n");
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    #ifdef DEBUG_MODE
    // Check OpenGL properties
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));
    #endif

    return 0;
}

uint32_t engine_init() {
    engine_config_init();

    if (backend_init() != 0) {
        printf("Failed to init backend\n");
        return 1;
    }
    
    // get window sizes
    SDL_GetWindowSize(window, &window_width, &window_height);
    SDL_GL_GetDrawableSize(window, &window_drawable_width, &window_drawable_height);

    // audio init
    audio_init();

    // default_shader
    #include "./default_sh.c"
    default_shader = create_shader(
        (const char**)&default_vert,
        (const char**)&default_frag,
        "in_vertex_position\0in_vertex_texcoord", 2,
        "", 0
    );
    if (default_shader == NULL){
        printf("error\n");
        return 1;
    }

    // default_texture
    default_texture = load_texture_from_surface(default_texture_surface);
    if (default_texture == NULL) {
        printf("error\n");
        return 1;
    }

    // simple draw module
    if (simple_draw_module_init() != 0) {
        printf("Failed to init simple-draw-module\n");
        return 1;
    }

    // init
    if (init != NULL) {
        uint32_t game_init_res = init();
        if (game_init_res != 0) {
            printf("init() failed with %u\n", game_init_res);
            engine_clean();
            return game_init_res;
        }
    }

    return 0;
}

void engine_handle_event() {
    SDL_Scancode scancode;

    switch(event.type) {
        case SDL_QUIT:
            running = 0;
            break;
        
        case SDL_KEYDOWN:
            scancode = event.key.keysym.scancode;
            if (scancode < SDL_NUM_SCANCODES && keys[scancode] == 0) keys[scancode] = 1;
            break;

        case SDL_KEYUP:
            scancode = event.key.keysym.scancode;
            if (scancode < SDL_NUM_SCANCODES) keys[scancode] = 0;
            break;
        
        case SDL_WINDOWEVENT:
            switch(event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    SDL_GetWindowSize(window, &window_width, &window_height);
                    SDL_GL_GetDrawableSize(window, &window_drawable_width, &window_drawable_height);
                    break;
            }
            break;
    }
    
    // <handle_event>
        if (handle_event != NULL)
            handle_event();
    // </handle_event>
    return;
}

void engine_update() {
    // update
    if (update != NULL)
        update();
    
    // keys - count ticks since press
    for (int32_t i = 0; i < SDL_NUM_SCANCODES; i++) {
        if (keys[i]) {
            keys[i]++;
            // avoid integer overflow
            if (keys[i] == 0) keys[i]--;
        }
    }
}

void engine_render() {
    // clear window
    use_default_fbo();
    glClearColor(BACKGROUND_COLOR_R, BACKGROUND_COLOR_G, BACKGROUND_COLOR_B, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // set all shaders to have not been updated by any camera yet
    for (uint64_t s = 0; s < shaders_amount; s++) shaders_list[s]->wvp_mat_camera_index = -1;

    // update every active camera's world_view_projection_matrix
    for (uint64_t c = 0; c < cameras_amount; c++) {
        // skip non active cameras
        if (cameras_list[c]->active == 0) continue;
        
        update_camera_wvp_mat(cameras_list[c]);
    }
    
    use_shader((shader_t*)default_shader);
    
    // game's render function
    if (render != NULL)
        render();
    
    // not in render period
    current_camera = -1;
    
    // show drawn image
    SDL_GL_SwapWindow(window);
}

void engine_clean() {
    // game clean
    if (clean) clean();
    
    clean_shaders();
    clean_textures();
    clean_meshes();
    clean_animations();
    clean_cameras();
    clean_fbos();
    audio_clean();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}