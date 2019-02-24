#ifndef INVADER_DEFINES_H
#define INVADER_DEFINES_H

#define FPS 60.0

#define DEFAULT_LIVES 4

#define SPRITE_SIZE 36

//TODO: Make the screen dimensions configuration option
#define SCREEN_W  640
#define SCREEN_H  480

#define RESOURCE_PATH  "resources/"
#define CONFIG_FILE  "resources/invader.cfg"

#define TITLE_Y 4

#define GAME_TITLE "INVADER!"

#define BLACK          al_map_rgb(0, 0, 0)
#define WHITE          al_map_rgb(255, 255, 255)
#define RED            al_map_rgb(207,  25,  25) //cf1919
#define BLUE           al_map_rgb( 72,  94, 130)
#define GREEN          al_map_rgb(153, 204,   0) //99cc00 - brokenlogo green
#define GREEN2         al_map_rgb( 37, 196,  23)
#define ORANGE         al_map_rgb(255, 174,   0)
#define ORANGE2        al_map_rgb(243, 156,  18) //f39c12 - orange

#define LIGHT_YELLOW   al_map_rgb(255, 255, 204) //ffffcc - light yellow / off-white
#define LIGHT_GREY     al_map_rgb(209, 209, 209) //d1d1d1 - light grey
#define LIGHT_BLUE     al_map_rgb(187, 222, 251) //bbdefb - light blue

#endif //!defined(INVADER_DEFINES_H)

