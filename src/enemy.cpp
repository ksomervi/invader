/*! \file enemy.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-09-24
 */

#include "enemy.h"

void enemy::update() {
  move(velocity());
  if(y() > _max_bounds.y()) {
    //Deactivate foe when it moves off screen
    active(false);
  }
}

