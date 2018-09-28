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

#include "logger.h"
#include "resource_manager.h"

class game {
  private:
    logger *_log;
    resource_manager *_rm;
    ALLEGRO_CONFIG *_cfg;

    int hits;
    int total_foes;

  public:
    game();
    ~game();
    ALLEGRO_CONFIG* load_options(const char*);

    bool init();
    void play();
    void end();

};//end game

#endif //!defined(INVADER_GAME_H)
