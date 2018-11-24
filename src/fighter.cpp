/*! \file fighter.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-29
 */

#include "fighter.h"
#include<iostream>
using std::cerr;
using std::endl;

#include "mine.h"
#include "mine_controller.h"

fighter::fighter() {
  _lives = DEFAULT_LIVES;
  _max_health = 100;
  _health = _max_health;
  _healing_time = 0;
  _healing = false;
  _mines = NULL;
  _mine_bm = NULL;
  _blaster = NULL;
  _blaster_bm = NULL;
  _fire_delay = 0;
  _sel_delay = 0;
  _m_st = STILL;
  _rot = 0.0;
  _deploy_sound = NULL;

  _ctrl = new player_controller();
};

fighter::~fighter() {
  if (_mines) {
    // FIXME: Delete allocated mine_controllers
    // BODY: The controller for mines is not deleted. Add code to delete the
    //      controller at the right location
    if (_mine_bm) {
      al_destroy_bitmap(_mine_bm);
      _mine_bm = NULL;
    }
    for (auto &m: *_mines) {
      if (m) {
        m->bitmap(NULL);
        delete m;
      }
      m = NULL;
    }

    delete _mines;
  }

  if (_blaster) {
    if (_blaster_bm) {
      al_destroy_bitmap(_blaster_bm);
      _blaster_bm = NULL;
    }
    for (auto &b: *_blaster) {
      if (b) {
        b->bitmap(NULL);
        delete b;
      }
      b = NULL;
    }

    delete _blaster;
  }

  delete _ctrl;
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
  cerr << "hit: " << _health << endl;
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

  if (_fire_delay) {
    _fire_delay--;
  }
  if (_sel_delay) {
    _sel_delay--;
  }

}//end fighter::update()

bool fighter::init(resource_manager *rm) {
  ALLEGRO_BITMAP *bm = rm->get_sprite("hero");
  if (!bm) {
    if (!create_bitmap(SPRITE_SIZE, SPRITE_SIZE)) {
      cerr << "failed to create hero bitmap!" << endl;
      return false;
    }
    ALLEGRO_STATE state;
    al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
    //Setup and mockup sprite
    al_set_target_bitmap(bitmap());
    al_clear_to_color(ORANGE);
    al_restore_state(&state);
  }
  else {
    bitmap(bm);
  }

  entity proto;
  proto.bitmap(rm->get_sprite("mine"));
  proto.controller(new mine_controller());

  //_blaster_bm = rm->get_sprite("blaster");

  if (! ready_weapons(&proto, 5)) {
    return false;
  }

  proto.bitmap(NULL);

  _deploy_sound = rm->get_sound("mine");
  if (!_deploy_sound) {
    cerr << "failed to load sound file" << endl;
  }

  _ctrl->init();

  return true;
}//end fighter::init()

bool fighter::ready_weapons(base_object *proto, const int &max) {
  _mines = new weapons();
  _active_wpn = _mines;

  if (!proto->bitmap()) {
    ALLEGRO_STATE state;
    al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);

    proto->bitmap(al_create_bitmap(SPRITE_SIZE, SPRITE_SIZE));

    al_set_target_bitmap(proto->bitmap());
    al_clear_to_color(LIGHT_YELLOW);

    float line_width = 8.2;
    al_draw_circle(SPRITE_SIZE/2, SPRITE_SIZE/2, (SPRITE_SIZE - line_width)/2,
        RED, line_width);
    al_draw_filled_triangle(5, 5, SPRITE_SIZE-5, 5, SPRITE_SIZE/2,
        SPRITE_SIZE-5, BLUE);
    al_draw_filled_triangle(5, SPRITE_SIZE-5, SPRITE_SIZE-5, SPRITE_SIZE-5,
        SPRITE_SIZE/2, 5, BLUE);

    al_convert_mask_to_alpha(_mine_bm, LIGHT_YELLOW);
    al_restore_state(&state);
  }

  base_object *m = NULL;
  for (int i=0; i<max; i++) {
    m = new mine();
    m->bitmap(proto->bitmap());
    m->controller(proto->controller());
    _mines->add(m);
  }
  m = NULL;

  _blaster = new weapons();
  if (!_blaster_bm) {
    _blaster_bm = al_create_bitmap(6, 6);
    ALLEGRO_STATE state;
    al_store_state(&state, ALLEGRO_STATE_TARGET_BITMAP);
    al_set_target_bitmap(_blaster_bm);
    al_clear_to_color(LIGHT_BLUE);
    al_restore_state(&state);
  }

  for (int i=0; i<20; i++) {
    m = new entity();
    m->bitmap(_blaster_bm);
    m->velocity(point_2d(0.0, -8.0));
    _blaster->add(m);
  }

  return true;
}//end fighter::ready_weapons()

bool fighter::fire_weapon() {
  if (_fire_delay == 0) {
    base_object * b = _active_wpn->deploy(_loc + point_2d((w()-6)/2, h()/2));
    //if (_active_wpn->deploy(_loc + point_2d((w()-6)/2, h()/2))) {
    if (b) {
      al_play_sample(_deploy_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
      if (_active_wpn == _mines) {
        dynamic_cast<mine*>(b)->age(300); //Limit how long a mine is active
        _fire_delay = _mine_delay;
      }
      else {
        _fire_delay = _blaster_delay;
      }
      return true;
    }
  }
  return false;
}//end fighter::fire_weapon()

void fighter::next_weapon() {
  if (_sel_delay == 0) {
    _sel_delay = 20;
    if (_active_wpn != _mines) {
      _active_wpn = _mines;
      cerr << "mines active!" << endl;
    }
    else {
      _active_wpn = _blaster;
      cerr << "blaster active!" << endl;
    }
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

