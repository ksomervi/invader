/** \file weapon.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2019-02-24
 */

#include "weapon.h"
#include "entity.h"

#include "iostream"
using std::cerr;
using std::endl;

weapon::weapon() {
  _cache = new entity_store();
  _graphic = new graphic_component();
  _fire_delay = 0;
  _cfg_delay = 0;
  _range = 0;
  _deploy_sound = nullptr;
}

weapon::~weapon() {
  if (!_cache->empty()) {
    base_controller *bc = (*_cache->begin())->controller();
    if (bc) {
      delete bc;
    }
    
    for (auto &e: *_cache) {
      if (e) {
        e->graphic(nullptr);
        e->controller(nullptr);
        delete e;
      }
      e = nullptr;
    }
  }//end (!_cache->empty())

  _graphic->destroy_bitmap();
  delete _graphic;
  delete _cache;
}

void weapon::clear_active() {
  _cache->clear_active();
}//end clear_active()

int weapon::count() {
  return _cache->count();
}

void weapon::delay(const int &d) {
  _cfg_delay = d;
}

int weapon::delay() {
  return _cfg_delay;
}

bool weapon::fire(const point_2d &p) {
  if (_fire_delay == 0) {
    entity *w = dynamic_cast<entity*>(_cache->deploy(p));
    if (w) {
      if (_deploy_sound) {
        al_play_sample(_deploy_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
      }
      if (_range) {
        w->age(_range); //Limit how long it is active
      }
      _fire_delay = _cfg_delay;
      _log->debug("   ...  firing weapon");
      return true;
    }
  }
  return false;
}//end weapon::fire()

_pool& weapon::get_active() {
  return _cache->get_active();
}

ALLEGRO_BITMAP* weapon::_default_mine() {
    ALLEGRO_STATE state;
    al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);

    ALLEGRO_BITMAP* bm = al_create_bitmap(SPRITE_SIZE, SPRITE_SIZE);

    al_set_target_bitmap(bm);
    al_clear_to_color(LIGHT_YELLOW);

    float line_width = 8.2;
    al_draw_circle(SPRITE_SIZE/2, SPRITE_SIZE/2, (SPRITE_SIZE - line_width)/2,
        RED, line_width);
    al_draw_filled_triangle(5, 5, SPRITE_SIZE-5, 5, SPRITE_SIZE/2,
        SPRITE_SIZE-5, BLUE);
    al_draw_filled_triangle(5, SPRITE_SIZE-5, SPRITE_SIZE-5, SPRITE_SIZE-5,
        SPRITE_SIZE/2, 5, BLUE);

    al_convert_mask_to_alpha(bm, LIGHT_YELLOW);
    al_restore_state(&state);
    return bm;
}

bool weapon::init(resource_manager *rm, const char *sel) {
  _log = rm->get_logger();
  _log->debug("Initializing " + string(sel));
  const char *label = rm->option(sel, "label");
  if (!label) {
    return false;
  }

  _log->debug("  - weapon class: " + string(label));

  int max_count = 5;
  int delay = _mine_delay;

  entity *proto = new entity();
  if (strcmp(label, "mine") == 0) {
    proto->controller(new mine_controller());
    max_count = 5;
    delay = _mine_delay;
  }
  else if (strcmp(label, "blaster") == 0) {
    //proto->controller(nullptr); // This is the default
    proto->velocity(point_2d(0.0, -8.0));
    max_count = 20;
    delay = _blaster_delay;
  }

  ALLEGRO_BITMAP *bm = rm->get_sprite(label);

  if (!bm) {
    if (strcmp(label, "mine") == 0) {
      bm = _default_mine();
    }
    else if (strcmp(label, "blaster") == 0) {
      bm = al_create_bitmap(4, 5);
      ALLEGRO_STATE state;
      al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
      al_set_target_bitmap(bm);
      al_clear_to_color(LIGHT_BLUE);
      al_restore_state(&state);
      //bm = default_blaster();
    }
  }

  _graphic = new graphic_component(bm);
  proto->graphic(_graphic);

  const char *opt = rm->option(sel, "count");
  if (opt) {
    max_count = atoi(opt);
  }

  for (int i=0; i<max_count; i++) {
    _cache->add(proto->clone());
  }

  opt = rm->option(sel, "delay");
  if (opt) {
    _log->debug(string("setting ") + string(label) + " delay: " + opt);
    delay = atoi(opt);
  }
  this->delay(delay);
  _deploy_sound = rm->get_sound(label);
  if (!_deploy_sound) {
    _log->error("failed to load sound file for " + string(label));
  }
  this->sound(_deploy_sound);
  opt = rm->option(sel, "range");
  if (opt) {
    this->range(atoi(opt));
  }

  return (_cache->count() == max_count);
}//end weapon::init()

void weapon::range(const int &r) {
  _range = r;
}//end weapon::range(const int&)

int weapon::range() {
  return _range;
}//end weapon::range()

void weapon::redraw() {
  _cache->redraw();
}//end weapon::redraw()

void weapon::sound(ALLEGRO_SAMPLE *sample) {
  _deploy_sound = sample;
}//end weapon::sound(ALLEGRO_SAMPLE *)

ALLEGRO_SAMPLE *weapon::sound() {
  return _deploy_sound;
}//end weapon::sound()

void weapon::update() {
  if (_fire_delay) {
    _fire_delay--;
  }
  _cache->update();
}//end weapon::update()
