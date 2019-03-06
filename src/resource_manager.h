#ifndef INVADER_RESOURCE_MANAGER_H
#define INVADER_RESOURCE_MANAGER_H

#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

class resource_manager;

#include "logger.h"
#include "fighter.h"
#include "random_generator.h"

enum fonttype { NONE, TITLE, TEXT, SMALL };

class resource_manager {
  private:
    logger *_log;
    ALLEGRO_CONFIG *_cfg;
    ALLEGRO_DISPLAY *_display;
    ALLEGRO_FONT *_title_font;
    ALLEGRO_FONT *_textfont;
    //enum rtype { NONE, PLAYER, AUDIO, TEXTURE, FONT, DISPLAY };

    random_generator *_rand_gen;
    fighter *_player;
    static resource_manager &_rm;

    bool _init_display();
    bool _init_audio();
    bool _init_fonts();

  public:
    resource_manager();
    ~resource_manager();
    bool init_resources(ALLEGRO_CONFIG*);
    const char* option(const char*, const char*);

    logger *get_logger();
    ALLEGRO_DISPLAY *get_display();
    ALLEGRO_FONT *get_font(fonttype=NONE);
    fighter* get_player();
    ALLEGRO_SAMPLE* get_sound(const char*);
    ALLEGRO_SAMPLE* get_sound(const char*, const char*);
    ALLEGRO_BITMAP* get_sprite(const char*);
    ALLEGRO_BITMAP* get_sprite(const char*, const char*);

    random_generator* get_random_generator();

    //bool init();

};//end class resource_manager

#endif //!defined(INVADER_RESOURCE_MANAGER_H)
