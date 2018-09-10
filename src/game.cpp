/*! \file game.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-29
 */

#include "game.h"

#include <random>
#include <iostream>
using std::cerr;
using std::endl;
#include <string>
using std::string;

#include "level_1.h"

game::game() {
  display = NULL;
  //event_queue = NULL;
  //timer = NULL;
  title_font = NULL;
  font = NULL;

  hero = NULL;

  hits = 0;
  total_foes = 0;

  _log = new logger(logger::NOTE);

}//end game::game()


ALLEGRO_DISPLAY *game::get_display() {
  return display;
}

ALLEGRO_FONT *game::get_font(int f=1) {
  switch(f) {
    case 0:
      return title_font;
      break;

    case 1:
    default:
      return font;
      break;
  };
}

fighter* game::get_player() {
  return hero;
}

bool game::init() {

  _log->note("Initializing game...");

  if(!al_init()) {
    _log->error("failed to initialize allegro!");
    return false;
  }

  if (load_options(CONFIG_FILE)) {
    _log->note("config file loaded");
  }

  _init_fonts();

  al_init_image_addon();
  al_init_primitives_addon();

  if (!al_install_audio()) {
    _log->error("failed to initialize audio!");
    return false;
  }

  if (!al_init_acodec_addon()){
    _log->error("failed to initialize audio codecs!");
    return false;
  }

  if (!al_reserve_samples(4)) {
    _log->error("failed to reserve samples!");
    return false;
  }


  if(!al_install_keyboard()) {
    _log->error("failed to initialize the keyboard!");
    return false;
  }

  display = al_create_display(SCREEN_W, SCREEN_H);
  if(!display) {
    _log->error("failed to create display!");
    return false;
  }

  hero = new fighter();
  if (!hero->create_bitmap(SPRITE_SIZE, SPRITE_SIZE)) {
  //if (!hero->create_bitmap("sprites/hero.png"))
    _log->error("failed to create hero bitmap!");
    return false;
  }
  al_set_target_bitmap(hero->bitmap());
  al_clear_to_color(al_map_rgb(255, 174, 0));

  al_set_target_backbuffer(display);

  /*
  event_queue = al_create_event_queue();
  if(!event_queue) {
    cerr << "failed to create event_queue!" << endl;
    delete hero;
    return false;
  }

  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  */

  al_clear_to_color(al_map_rgb(0,0,0));

  al_flip_display();

  return true;
}//end game::init()


void game::play() {
  level_1 *l = new level_1();
  l->play(this);

  delete l;
}


void game::print_score() {
  float kill_eff = float(hits)*100.0/total_foes;
  cerr  << "So how did you do?" << endl;
  cerr  << "  Total foes: " << total_foes << endl;
  cerr  << "  Total hits: " << hits << " (eff: ";
  cerr .precision(2);
  cerr  << std::fixed << kill_eff << "%)" << endl;
}//end game::print_score()

bool game::load_options(const char* filename) {
  _cfg = al_load_config_file(CONFIG_FILE);
  return (_cfg != NULL);
}

const char* game::option(const char* section, const char* key) {
  return al_get_config_value(_cfg, section, key);
}//end game::option


ALLEGRO_SAMPLE* game::get_sound(const char* option_key) {
  string path = option("MEDIA", option_key);
  cerr  << "Loading sound: " << "MEDIA::" << option_key
    << ":" << path << endl;
  ALLEGRO_SAMPLE *s = al_load_sample(path.c_str());
  return s;
}//end game::get_sound()

ALLEGRO_BITMAP* game::get_sprite(const char* option_key) {
  string path = option("SPRITES", option_key);
  cerr  << "Loading bitmap: " << "SPRITES::" << option_key
    << ":" << path << endl;
  ALLEGRO_BITMAP *bm = al_load_bitmap(path.c_str());
  return bm;
}//end game::get_sprite()

bool game::_init_fonts() {
  al_init_font_addon(); // initialize the font addon
  al_init_ttf_addon(); // initialize the ttf (True Type Font) addon

  title_font = al_load_ttf_font(option("FONT", "fontpath"),
                                atoi(option("FONT", "title_fontsize")), 0);
  if (!title_font) {
    cerr << "Failed to load title font: " << option("FONT", "fontpath") << endl;
    return false;
  }

  //font = al_load_ttf_font(FONTNAME, SECONDARY_FONTSIZE, 0);
  font = al_load_ttf_font(option("FONT", "fontpath"),
                          atoi(option("FONT", "secondary_fontsize")), 0);
  if (!font) {
    cerr << "Failed to load secondary font: " << option("FONT", "fontpath") << endl;
    return false;
  }

  return true;
}//end game::_init_fonts()

void game::end() {
  if (hero) {
    delete hero;
  }

  al_destroy_config(_cfg);
  al_destroy_font(title_font);
  al_destroy_font(font);

  if (display) {
    al_destroy_display(display);
  }

  return;
}//end game::end()

