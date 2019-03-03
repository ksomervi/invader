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
  int max_levels = atoi(_rm->option(NULL, "max_levels"));
  _log->note("max levels: " + std::to_string(max_levels));
  vector<int> init_foes = {3, 3, 4};
  vector<int> foes;
  level_1 *l = new level_1();
  level_configuration config;

  int final_wave = 0;
  int wave = 0;
  int level = 1;
  bool playing = true;

  while (playing and (level<=max_levels)) {
    foes.clear();
    final_wave = 0;
    for (auto &f: init_foes) {
      wave = level * f;
      foes.push_back(wave);
      final_wave += wave;
    }
    init_foes.push_back(wave);
    foes.push_back(final_wave);

    config.enemy_waves(foes);
    config.level(level);

    if (l->play(_rm, &config)){
      // Level completed
      level++;
      // Increase players max_health
      fighter *f =  _rm->get_player();
      int max_health = f->max_health();
      f->max_health(max_health+10);
      f->add_health(10);
    }
    else if (l->quit()) {
      playing = false;
    }
  }//end while (playing and (level<=max_levels))

  delete l;
}

ALLEGRO_CONFIG* game::load_options(const char* filename) {
  _cfg = al_load_config_file(CONFIG_FILE);
  return _cfg;
}

void game::end() {
}//end game::end()

