/*! \file entity.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-27
 */

#include "entity.h"

entity::entity() {
}//end entity

entity::~entity() {
}

void entity::update() {
  // Use internal control component
  if (_active) {
    if (_loc.y() < 0) {
      _active = false;
    }
    else {
      _loc += _vel;
    }
  }
}

