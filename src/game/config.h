#define DEBUG_MODE
// #define NETFRAME_DEBUG


// engine configs
#define _WINDOW_TITLE_ "My title in engine config"
#define _WINDOW_START_X_ (100)
#define _WINDOW_START_Y_ (50)
#define _WINDOW_START_WIDTH_ (320*3)
#define _WINDOW_START_HEIGHT_ (240*3)

#define _TARGET_FRAME_RATE_ (60)


#define _BACKGROUND_COLOR_R_ (0.1)
#define _BACKGROUND_COLOR_G_ (0.1)
#define _BACKGROUND_COLOR_B_ (0.1)


#define _CLIENTS_MAX_AMOUNT_ (3) /*The maximum player cound is (_CLIENTS_MAX_AMOUNT_+1) beacuse the server is also a player*/

#define _PACKET_MAX_LENGTH_ (48*(_CLIENTS_MAX_AMOUNT_+1))