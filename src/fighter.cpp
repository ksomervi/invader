/*! \file fighter.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-29
 */

#include "fighter.h"
#include<iostream>

fighter::fighter() {
  _lives = DEFAULT_LIVES;
  _max_health = 100;
  _health = _max_health;
  _healing_time = 0;
  _healing = false;
  _mines = NULL;
  _mine_bm = NULL;
  _fire_delay = 0;
};

fighter::~fighter() {
  if (_mines) {
    if (_mine_bm) {
      al_destroy_bitmap(_mine_bm);
      _mine_bm = NULL;
    }
    for (auto &m: *_mines) {
      if (m) {
        m->bitmap(NULL);
        delete m;
      }
    }

    delete _mines;
  }
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


int fighter::health() {
  return _health;
}

int fighter::max_health() {
  return _max_health;
}

void fighter::add_health(int h) {
  _health += h;

  if (_health < 0) {
    _health = 0;
    std::cout << "player died! restart level" << std::endl;
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


void fighter::take_hit(int damage) {
  add_health(-1*damage);
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
  if (_healing) {
    if (_healing_time > 0) {
      _healing_time--;
    }
    else {
      this->add_health(2);
    }
  }

  if (_fire_delay) {
    _fire_delay--;
  }

}//end fighter::update()

bool fighter::ready_weapons(const int &max) {
  _mines = new weapons();
  //mine_bm = env->get_sprite("mine");

  if (!_mine_bm) {
    ALLEGRO_STATE state;
    al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
    _mine_bm = al_create_bitmap(SPRITE_SIZE, SPRITE_SIZE);

    al_set_target_bitmap(_mine_bm);
    al_clear_to_color(LIGHT_YELLOW);

    float line_width = 8.2;
    al_draw_circle(SPRITE_SIZE/2, SPRITE_SIZE/2, (SPRITE_SIZE - line_width)/2,
        RED, line_width);
    al_draw_filled_triangle(5, 5, SPRITE_SIZE-5, 5, SPRITE_SIZE/2,
        SPRITE_SIZE-5, BLUE);
    al_draw_filled_triangle(5, SPRITE_SIZE-5, SPRITE_SIZE-5, SPRITE_SIZE-5,
        SPRITE_SIZE/2, 5, BLUE);

    al_convert_mask_to_alpha(_mine_bm, LIGHT_YELLOW);
    //al_set_target_backbuffer(display);
    al_restore_state(&state);
  }

  basic_object *m = NULL;
  for (int i=0; i<max; i++) {
    m = new basic_object();
    m->bitmap(_mine_bm);
    _mines->add(m);
  }

  return true;
}//end fighter::ready_weapons()

bool fighter::fire_weapon() {
  if (_fire_delay == 0) {
    if (_mines->deploy(_loc)) {
      _fire_delay = 30;
      return true;
    }
  }
  return false;
}//end fighter::fire_weapon()

int fighter::max_weapons() {
  return _mines->count();
}//end fighter::max_weapons()

_pool& fighter::get_deployed_mines() {
  return _mines->get_active();
}

void fighter::redraw() {
  _mines->redraw();

  basic_object::redraw();
}

