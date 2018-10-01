/*! \file player_controller.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-09-15
 */

#include "fighter.h"
#include "player_controller.h"

player_controller::player_controller() {
  init();
}

player_controller::~player_controller() {

}

void player_controller::init() {
  for (int i=0; i<ALLEGRO_KEY_MAX; i++) {
    _key_pressed[i] = false;
  }
}//end player_controller::init()

bool player_controller::handle_event(ALLEGRO_EVENT &ev) {

  switch(ev.type) {
    case ALLEGRO_EVENT_KEY_DOWN:
    case ALLEGRO_EVENT_KEY_CHAR:
      _key_pressed[ev.keyboard.keycode] = true;
      break;

    case ALLEGRO_EVENT_KEY_UP:
      _key_pressed[ev.keyboard.keycode] = false;
      break;

    default:
      return false;
  }//end switch (ev.type)

  return true;
}//end player_controller::handle_event()

void player_controller::update(basic_object *o) {
  if (_key_pressed[ALLEGRO_KEY_SPACE]) {
    ((fighter*)o)->fire_weapon();
  }
}//end player_controller::update(basic_object *o)

point_2d player_controller::direction() {
  return point_2d(left()+right(), up()+down());
}//end player_controller::direction()

float player_controller::up() {
  if (_key_pressed[ALLEGRO_KEY_UP]) {
    return -1.0;
  }
  return 0.0;
}//end player_controller::up()

float player_controller::down() {
  if (_key_pressed[ALLEGRO_KEY_DOWN]) {
    return 1.0;
  }
  return 0.0;
}//end player_controller::down()


float player_controller::left() {
  if (_key_pressed[ALLEGRO_KEY_LEFT]) {
    return -1.0;
  }
  return 0.0;
}//end player_controller::left()


float player_controller::right() {
  if (_key_pressed[ALLEGRO_KEY_RIGHT]) {
    return 1.0;
  }
  return 0.0;
}//end player_controller::right()

bool player_controller::fire() {
  return _key_pressed[ALLEGRO_KEY_SPACE];
}//end player_controller::fire()

