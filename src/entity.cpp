/*! \file entity.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-27
 */

#include "entity.h"

entity::entity() {
  _ctrl = NULL;
}//end entity

entity::~entity() {
}

void entity::update() {
  // Use internal control component
  if (_active) {
    if (_ctrl) {
      _ctrl->update(this);
    }
    move(_vel);

    if (_loc.y() < _min_bounds.y()) {
      _active = false;
    }
  }
}

