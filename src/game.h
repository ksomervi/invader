#ifndef INVADER_GAME_H
#define INVADER_GAME_H

#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "fighter.h"

class game {
  private:
    ALLEGRO_CONFIG *_cfg;
    ALLEGRO_DISPLAY *display;
    //ALLEGRO_EVENT_QUEUE *event_queue;
    //ALLEGRO_TIMER *timer;
    ALLEGRO_FONT *title_font;
    ALLEGRO_FONT *font;

    fighter *hero;
    //armada foes;

    int hits;
    int total_foes;

    bool _init_fonts();

  public:
    game();
    ~game();
    bool load_options(const char*);
    const char* option(const char*, const char*);

    ALLEGRO_DISPLAY *get_display();
    ALLEGRO_FONT *get_font(int);
    fighter* get_player();
    ALLEGRO_SAMPLE* get_sound(const char*);
    ALLEGRO_BITMAP* get_sprite(const char*);

    bool init();
    void play();
    void print_score();
    void end();

};//end game

#endif //!defined(INVADER_GAME_H)
