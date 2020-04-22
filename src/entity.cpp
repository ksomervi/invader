/*! \file entity.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-27
 */

#include "entity.h"

entity::entity() {
  _age = UNLIMITED;
}//end entity

entity::entity(const entity &e) : base_object(e) {
  _age = e._age;
}//end entity

entity::~entity() {
}

base_object* entity::clone() {
  return new entity(*this);
}//end clone()

void entity::update() {
  // Use internal control component
  if (_age) {
    if (_active) {
      if (_ctrl) {
        _ctrl->update(this);
      }
      move(_vel);

      if (_loc.y() < _min_bounds.y()) {
        _active = false;
      }
    }//end if (_active)
    if (_age != UNLIMITED) {
      _age--;
    }
  }//end if (_age)
  else {
    if (_active) {
      _active = false;
    }
  }
}//end entity::update()

void entity::age(const int &a) {
  _age = a;
}

int entity::age() {
  return _age;
}

