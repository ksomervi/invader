/*! \file enemy_controller.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-09-30
 */

#include "enemy_controller.h"

enemy_controller::enemy_controller() : _mv_count(0) {
  _rg = new random_generator();
}

enemy_controller::~enemy_controller() {
  delete _rg;
}

void enemy_controller::init() {

}

bool enemy_controller::handle_event(ALLEGRO_EVENT &ev) {
  return false;
}

point_2d enemy_controller::direction() {
  if (!_mv_count) {
    _mv_count = _rg->random_int(40,120);

    if (_dir.x() < 0.0) {
      _dir.x(1.0);
    }
    else {
      _dir.x(-1.0);
    }
  }
  _mv_count--;

  return _dir;
}//end enemy_controller::direction()

void enemy_controller::update(base_object *o) {
  _dir = o->velocity();

  o->velocity(direction());
}//end enemy_controller::update(base_object *)

