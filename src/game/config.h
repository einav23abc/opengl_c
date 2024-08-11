// #define DEBUG_MODE

// game defines
#define _PLAYERS_MAX_AMOUNT_ (4)
#define _SERVER_HOST_PLAYER_ID_ (_PLAYERS_MAX_AMOUNT_-1) /*the last player ID possible*/

#define _UPDATE_PACKET_FRAME_INTERVALS_ (1)


#define _OUTPORT_WIDTH_ (320*3)
#define _OUTPORT_HEIGHT_ (240*3)

#define _OUTPORT_BACKGROUND_COLOR_R_ (0.2)
#define _OUTPORT_BACKGROUND_COLOR_G_ (0.2)
#define _OUTPORT_BACKGROUND_COLOR_B_ (0.3)


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


#define _CLIENTS_MAX_AMOUNT_ (_PLAYERS_MAX_AMOUNT_-1) /*the last player ID is the server host*/

#define _PACKET_MAX_LENGTH_ (48*_PLAYERS_MAX_AMOUNT_)