/*! \file controller.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-09-15
 */

#include "controller.h"

controller::controller() {
  /*
  for (auto &k: _key) {
    k = false;
  }
  */

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
  cerr << "handling event..." << endl;

  if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {

    _key_pressed[ev.keyboard.keycode] = true;

    switch(ev.keyboard.keycode) {

      case ALLEGRO_KEY_UP:
        _key[KEY_UP] = true;
        break;

      case ALLEGRO_KEY_DOWN:
        _key[KEY_DOWN] = true;
        break;

      case ALLEGRO_KEY_LEFT:
        _key[KEY_LEFT] = true;
        break;

      case ALLEGRO_KEY_RIGHT:
        _key[KEY_RIGHT] = true;
        break;

      case ALLEGRO_KEY_SPACE:
        _key[KEY_SPACE] = true;
        break;
    }//end switch(ev.keyboard.keycode)
    return true;
  }//end else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
  else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
    _key_pressed[ev.keyboard.keycode] = false;
    switch(ev.keyboard.keycode) {
      case ALLEGRO_KEY_UP:
        _key[KEY_UP] = false;
        break;

      case ALLEGRO_KEY_DOWN:
        _key[KEY_DOWN] = false;
        break;

      case ALLEGRO_KEY_LEFT:
        _key[KEY_LEFT] = false;
        break;

      case ALLEGRO_KEY_RIGHT:
        _key[KEY_RIGHT] = false;
        break;

      case ALLEGRO_KEY_F1:
        cerr << "Help" << endl;
        break;

      case ALLEGRO_KEY_SPACE:
        _key[KEY_SPACE] = false;
        break;

      case ALLEGRO_KEY_P:
        /*
        if (al_get_timer_started(timer)) {
          al_stop_timer(timer);
        }
        else {
          al_resume_timer(timer);
        }
        */
        break;

      case ALLEGRO_KEY_ESCAPE:
        //quit(true);
        break;
    }//end switch(ev.keyboard.keycode)
    return true;
  }//end else if(ev.type == ALLEGRO_EVENT_KEY_UP)
 
  return false;
}//end end controller::handle_event()


point_2d controller::direction() {
  return point_2d(left()+right(), up()+down());
}

float controller::up() {
  if (_key_pressed[ALLEGRO_KEY_UP]) {
    cerr << "up" << endl;
    return -1.0;
  }
  return 0.0;
}

float controller::down() {
  if (_key_pressed[ALLEGRO_KEY_DOWN]) {
    cerr << "down" << endl;
    return 1.0;
  }
  return 0.0;
}


float controller::left() {
  if (_key_pressed[ALLEGRO_KEY_LEFT]) {
    cerr << "left" << endl;
    return -1.0;
  }
  return 0.0;
}


float controller::right() {
  if (_key_pressed[ALLEGRO_KEY_RIGHT]) {
    cerr << "right" << endl;
    return 1.0;
  }
  return 0.0;
}

bool controller::fire() {
  return _key_pressed[ALLEGRO_KEY_SPACE];
}

bool controller::quit() {
  return _key_pressed[ALLEGRO_KEY_ESCAPE];
}//end base_level::quit()


bool controller::pause_event() {
  return _key_pressed[ALLEGRO_KEY_P];
}//end base_level::pause()


