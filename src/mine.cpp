/*! \file mine.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-27
 */

#include "mine.h"

mine::mine() {
  _age = -1;
  _ctrl = NULL;
}//end mine

mine::~mine() {
}

void mine::update() {
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
    _age--;
  }//end if (_age)
  else {
    this->active(false);
  }
}

void mine::age(const int &a) {
  _age = a;
}

int mine::age() {
  return _age;
}

