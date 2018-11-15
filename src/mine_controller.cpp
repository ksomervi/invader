/*! \file mine_controller.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-09-30
 */

#include "mine_controller.h"

mine_controller::mine_controller() : _mv_count(0) {
  _x_side = false;
  _rg = new random_generator();
}

mine_controller::~mine_controller() {
  delete _rg;
}

void mine_controller::init() {

}

bool mine_controller::handle_event(ALLEGRO_EVENT &ev) {
  return false;
}

point_2d mine_controller::direction() {
  if (!_mv_count) {
    _mv_count = _rg->random_int(10,30);

    if (_x_side) {
      if (_dir.x() < 0.0) {
        _dir.x(1.0);
      }
      else {
        _dir.x(-1.0);
      }
    }
    else {
      if (_dir.y() < 0.0) {
        _dir.y(1.0);
      }
      else {
        _dir.y(-1.0);
      }
    }
    _x_side = !_x_side;
  }//end if (!_mv_count)
  _mv_count--;

  return _dir;
}//end mine_controller::direction()

void mine_controller::update(base_object *o) {
  _dir = o->velocity();

  o->velocity(direction());
}//end mine_controller::update(base_object *)

