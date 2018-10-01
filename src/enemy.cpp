/*! \file enemy.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-09-24
 */

#include <random>

#include "enemy.h"

#include <iostream>

enemy::enemy() {
  _ctrl = new enemy_controller();
}

enemy::~enemy() {
  delete _ctrl;
}

void enemy::update() {
  _ctrl->update(this);
  point_2d v = velocity();
  move(v);

  if (_loc.x() > _max_bounds.x()) {
    _loc.x(_max_bounds.x());
    v.x(-v.x());
  }
  else if (_loc.x() < _min_bounds.x()) {
    _loc.x(_min_bounds.x());
    v.x(-v.x());
  }
  velocity(v);

  if(y() > _max_bounds.y()) {
    //Deactivate foe when it moves off screen
    active(false);
  }
}

