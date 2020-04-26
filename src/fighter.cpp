/*! \file fighter.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-29
 */

#include <cstdio>

#include "fighter.h"

#include <iostream>
using std::cerr;
using std::endl;

//#include "mine_controller.h"

fighter::fighter() {
  _lives = DEFAULT_LIVES;
  _max_health = 100;
  _health = _max_health;
  _healing_time = 0;
  _healing = false;
  _mines = nullptr;
  //_mine_bm = nullptr;
  _blaster = nullptr;
  //_blaster_bm = nullptr;
  _sel_delay = 0;
  _m_st = STILL;
  _rot = 0.0;

  _ctrl = new player_controller();
  _gc = new graphic_component();
};

fighter::~fighter() {
  if (_mines) {
    // FIXME: Delete allocated mine_controller
    // BODY: The controller for mines is not deleted. Add code to delete the
    //      controller at the right location
    //if (_mine_bm) {
      //al_destroy_bitmap(_mine_bm);
      //_mine_bm = nullptr;
    //}
    //mines have a shared controller
    delete _mines;
  }

  if (_blaster) {
    //if (_blaster_bm) {
      //al_destroy_bitmap(_blaster_bm);
      //_blaster_bm = nullptr;
    //}
    delete _blaster;
  }

  delete _ctrl;

  if (_deploy_sound) {
    al_destroy_sample(_deploy_sound);
  }

  delete _gc;
}

bool fighter::handle_event(ALLEGRO_EVENT &ev) {
  return _ctrl->handle_event(ev);
}

int fighter::lives() {
  return _lives;
}

void fighter::lives(int l) {
  _lives = l;
}

void fighter::add_lives(int l) {

  if (l < 0) {
    if (_lives == 0) {
      return;
    }
    _health = _max_health;
  }

  _lives += l;
}

bool fighter::is_alive() {
  return (_health > 0);
}

int fighter::health() {
  return _health;
}

int fighter::max_health() {
  return _max_health;
}

void fighter::max_health(const int &mh) {
  _max_health = mh;
}

void fighter::add_health(int h) {
  _health += h;

  if (_health < 0) {
    _health = 0;
    _log->note("player died! restart level");
  }
  else if (_health < _max_health) {
    _healing = true;
    _healing_time = 60;
  }
  else {
    _healing = false;
    if (_health > _max_health) {
      _health = _max_health;
    }
  }
}//end fighter::add_health()

void fighter::set_logger(logger *l) {
  _log = l;
}//end set_logger(logger*)

void fighter::take_hit(int damage) {
  add_health(-1*damage);
  _log->debug("hit: " + std::to_string(_health));
}


float fighter::percent_health() {
  return float(_health) / _max_health;
}

void fighter::move(const point_2d &offset) {
  point_2d next_loc(location() + offset);
  if (next_loc.x() < _min_bounds.x()) {
    next_loc.x(_min_bounds.x());
  }
  if (next_loc.x() > _max_bounds.x()) {
    next_loc.x(_max_bounds.x());
  }
  if (next_loc.y() < _min_bounds.y()) {
    next_loc.y(_min_bounds.y());
  }
  if (next_loc.y() > _max_bounds.y()) {
    next_loc.y(_max_bounds.y());
  }

  location(next_loc);
}//end

void fighter::update() {
  float max_vx = 5.0;
  float max_vy = 5.0;
  float rot_inc = 0.05;
  float v_inc = 0.5;
  point_2d g(0.0, 1.0);

  _mines->update();
  _blaster->update();

  _ctrl->update(this);

  point_2d m = _ctrl->direction();

  _vel += m;

  if (m.x() < -0.1) {
    if (_rot > -0.75) {
      _rot -= rot_inc;
    }

    if (_vel.x() < -max_vx) {
      _vel.x(-max_vx);
    }
    _m_st = LEFT;
  }
  else if (m.x() > 0.1) {
    if (_rot < 0.75) {
      _rot += rot_inc;
    }
    if (_vel.x() > max_vx) {
      _vel.x(max_vx);
    }
    _m_st = RIGHT;
  }
  else {
    if (_rot < 0.0) {
      _rot += rot_inc;
    }
    else if (_rot > 0.0) {
      _rot -= rot_inc;
    }
    if (_vel.x() > 0.0) {
      _vel.x(_vel.x()-(v_inc/2));
    }
    else if (_vel.x() < 0.0) {
      _vel.x(_vel.x()+(v_inc/2));
    }
    _m_st = STILL;
  }

  if (m.y() < -0.1) {
    if (_vel.y() < -max_vy) {
      _vel.y(-max_vy);
    }
  }
  else if (m.y() > 0.1) {
    if (_vel.y() > max_vy) {
      _vel.y(max_vy);
    }
  }
  else {
    if (_vel.y() > g.y()) {
      _vel.y(_vel.y()-v_inc);
    }
    else if (_vel.y() < g.y()) {
      _vel.y(_vel.y()+v_inc);
    }
  }

  move(_vel);

  if (_healing) {
    if (_healing_time > 0) {
      _healing_time--;
    }
    else {
      this->add_health(2);
    }
  }

  if (_sel_delay) {
    _sel_delay--;
  }

}//end fighter::update()

bool fighter::init(resource_manager *rm) {
  ALLEGRO_BITMAP *bm = rm->get_sprite("hero");
  if (!bm) {
    if (!_gc->create_bitmap(SPRITE_SIZE, SPRITE_SIZE)) {
      _log->error("failed to create hero bitmap!");
      return false;
    }
    ALLEGRO_STATE state;
    al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
    //Setup and mockup sprite
    al_set_target_bitmap(bm);
    al_clear_to_color(ORANGE);
    al_restore_state(&state);
  }
  _gc->bitmap(bm);

  if (! ready_weapons(rm)) {
    return false;
  }

  _ctrl->init();
  _log->note("Fighter initialized");

  return true;
}//end fighter::init()

// TODO: refactor to function that initializes a single weapon
bool fighter::ready_weapons(resource_manager *rm) {

  _log->debug("Preparing weapons...");

  _mines = new weapon();
  _mines->init(rm, "WEAPON 1");
  cerr << "mines inited" << endl;

  _blaster = new weapon();
  _blaster->init(rm, "WEAPON 0");

  _arsenal[primary] = _blaster;
  _arsenal[secondary] = _mines;

  return true;
}//end fighter::ready_weapons()

bool fighter::fire_weapon(const weapon_select &ws) {
  return _arsenal[ws]->fire(_loc + point_2d((w()-6)/2, h()/2));
}//end fighter::fire_weapon()

void fighter::swap_weapons() {
  if (_sel_delay == 0) {
    _sel_delay = 20;
    weapon *tmp = _arsenal[primary];
    _arsenal[primary] = _arsenal[secondary];
    _arsenal[secondary] = tmp;
  }
}//end fighter::next_weapon()


void fighter::clear_weapons() {
  _mines->clear_active();
}


int fighter::max_weapons() {
  return _mines->count();
}//end fighter::max_weapons()

_pool& fighter::get_deployed_mines() {
  return _mines->get_active();
}

_pool& fighter::get_deployed_blasts() {
  return _blaster->get_active();
}

void fighter::redraw() {
  _mines->redraw();
  _blaster->redraw();

  if (_health == 0) {
    return;
  }

  base_object::redraw(_rot);
}

