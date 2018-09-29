/*! \file game.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-29
 */

#include "game.h"
#include "level_1.h"

game::game() {

  hits = 0;
  total_foes = 0;

  _rm = new resource_manager();
  _log = _rm->get_logger();

}//end game::game()

game::~game() {
  delete _rm;
}

bool game::init() {

  _log->note("Initializing game...");

  if(!al_init()) {
    _log->error("failed to initialize allegro!");
    return false;
  }

  al_init_image_addon();
  al_init_primitives_addon();

  if(!al_install_keyboard()) {
    _log->error("failed to initialize the keyboard!");
    return false;
  }

  if (!_rm->init_resources(load_options(CONFIG_FILE))) {
    _log->error("failed to initialize resource_manager!");
    return false;
  }

  al_set_target_backbuffer(_rm->get_display());

  al_clear_to_color(BLACK);

  al_flip_display();

  return true;
}//end game::init()


void game::play() {
  level_1 *l = new level_1();
  l->play(_rm);

  delete l;
}

ALLEGRO_CONFIG* game::load_options(const char* filename) {
  _cfg = al_load_config_file(CONFIG_FILE);
  return _cfg;
}

void game::end() {
}//end game::end()

