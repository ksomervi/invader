/** \file weapon.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2019-02-24
 */


#include "weapon.h"
#include "entity.h"

#include<iostream>
using std::cerr;
using std::endl;

weapon::weapon() {
  _cache = new entity_store();
  //_bm = nullptr;
  _fire_delay = 0;
  _cfg_delay = 0;
  _range = 0;
  _deploy_sound = nullptr;
}

//weapon::weapon(base_object *proto, const int &max, const int &delay): _fire_delay(delay) { }

weapon::~weapon() {
  if (!_cache->empty()) {
    //TODO: delete the mine controller
    //mine_controller * mc = _cache->front()->controller();
    //delete mc;
    //TODO: where is the weapon bitmap deallocated?
    
    for (auto &e: *_cache) {
      if (e) {
        e->bitmap(nullptr);
        e->controller(nullptr);
        delete e;
      }
      e = nullptr;
    }
  }//end (!_cache->empty())

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
      return true;
    }
  }
  return false;
}//end weapon::fire()

_pool& weapon::get_active() {
  return _cache->get_active();
}

bool weapon::init(base_object *proto, const int &max) {
  for (int i=0; i<max; i++) {
    _cache->add(proto->clone());
  }
  return (_cache->count() == max);
}//end weapon::init()

void weapon::range(const int &r) {
  _range = r;
}//end weapon::range(const int&)

int weapon::range() {
  return _range;
}//end weapon::range()

void weapon::redraw() {
  _cache->redraw();
}

void weapon::sound(ALLEGRO_SAMPLE *sample) {
  _deploy_sound = sample;
}

ALLEGRO_SAMPLE *weapon::sound() {
  return _deploy_sound;
}

void weapon::update() {
  if (_fire_delay) {
    _fire_delay--;
  }
  _cache->update();
}

