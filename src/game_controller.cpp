/*! \file game_controller.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-09-15
 */

#include "game_controller.h"

game_controller::game_controller() {
  //init();
}

game_controller::~game_controller() {

}

void game_controller::init() {
  for (int i=0; i<ALLEGRO_KEY_MAX; i++) {
    _key_pressed[i] = false;
  }
}//end game_controller::init()

bool game_controller::handle_event(ALLEGRO_EVENT &ev) {
  switch(ev.type) {
    case ALLEGRO_EVENT_KEY_DOWN:
      _key_pressed[ev.keyboard.keycode] = true;
      return ((ev.keyboard.keycode == ALLEGRO_KEY_P)
          || (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE));
      break;

    case ALLEGRO_EVENT_KEY_UP:
      _key_pressed[ev.keyboard.keycode] = false;
      return ((ev.keyboard.keycode == ALLEGRO_KEY_P)
          || (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE));
      break;

    default:
      return false;
  }//end switch (ev.type)

  return false;
}//end game_controller::handle_event()

void game_controller::update(base_object *o) { }

point_2d game_controller::direction() {
  return _dir;
}//end game_controller::direction()

bool game_controller::quit() {
  return _key_pressed[ALLEGRO_KEY_ESCAPE];
}//end game_controller::quit()

bool game_controller::pause_event() {
  return _key_pressed[ALLEGRO_KEY_P];
}//end game_controller::pause()

