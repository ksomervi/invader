/*! \file controller.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-09-15
 */

#include "controller.h"

controller::controller() {
  init();
}

controller::~controller() {

}

void controller::init() {
  for (int i=0; i<ALLEGRO_KEY_MAX; i++) {
    _key_pressed[i] = false;
  }
}//end controller::init()

bool controller::handle_event(ALLEGRO_EVENT &ev) {

  switch(ev.type) {
    case ALLEGRO_EVENT_KEY_DOWN:
      _key_pressed[ev.keyboard.keycode] = true;
      break;

    case ALLEGRO_EVENT_KEY_UP:
      _key_pressed[ev.keyboard.keycode] = false;
      break;

    default:
      return false;
  }//end switch (ev.type)
    
  return true;
}//end controller::handle_event()


point_2d controller::direction() {
  return point_2d(left()+right(), up()+down());
}//end controller::direction()

float controller::up() {
  if (_key_pressed[ALLEGRO_KEY_UP]) {
    return -1.0;
  }
  return 0.0;
}//end controller::up()

float controller::down() {
  if (_key_pressed[ALLEGRO_KEY_DOWN]) {
    return 1.0;
  }
  return 0.0;
}//end controller::down()


float controller::left() {
  if (_key_pressed[ALLEGRO_KEY_LEFT]) {
    return -1.0;
  }
  return 0.0;
}//end controller::left()


float controller::right() {
  if (_key_pressed[ALLEGRO_KEY_RIGHT]) {
    return 1.0;
  }
  return 0.0;
}//end contreoller::right()

bool controller::fire() {
  return _key_pressed[ALLEGRO_KEY_SPACE];
}//end controller::fire()

bool controller::quit() {
  return _key_pressed[ALLEGRO_KEY_ESCAPE];
}//end base_level::quit()


bool controller::pause_event() {
  return _key_pressed[ALLEGRO_KEY_P];
}//end base_level::pause()


