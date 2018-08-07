/*! \file basic_object.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-27
 */

#include "basic_object.h"

#include <cmath>
#include <iostream>
using std::cout;
using std::endl;

basic_object::basic_object() {
  _w = 0.0;
  _h = 0.0;
  //Initial location, middle of the screen
   _x = (SCREEN_W - SPRITE_SIZE) / 2.0;
   _y = (SCREEN_H - SPRITE_SIZE) / 2.0;

   _velx = 0.0;
   _vely = 0.0;

   _bm = NULL;
}//end basic_object


basic_object::basic_object(float x, float y, float vx, float vy) {
  _w = 0.0;
  _h = 0.0;
  //Initial location, middle of the screen
  _x = x;
  _y = y;
  _velx = vx;
  _vely = vy;

  _bm = NULL;
}//end basic_object


basic_object::~basic_object() {
  if (_bm) {
    al_destroy_bitmap(_bm);
  }
}

float basic_object::x() {
  return _x;
}

float basic_object::y() {
  return _y;
}

void basic_object::x(float v) {
  _x = v;
}

void basic_object::y(float v) {
  _y = v;
}

void basic_object::redraw() {
  al_draw_bitmap(_bm, _x, _y, 0);
}

void basic_object::draw(float x, float y) {
  _x = x;
  _y = y;
  al_draw_bitmap(_bm, _x, _y, 0);
}

bool basic_object::create_bitmap(const char *filename) {
  _bm = al_load_bitmap(filename);
  if (_bm) {
    _w = al_get_bitmap_width(_bm);
    _h = al_get_bitmap_height(_bm);
    return true;
  }

  return false;
}

bool basic_object::create_bitmap(float w, float h) {
  _h = h;
  _w = w;
  _bm = al_create_bitmap(w, h);
  return _bm != NULL;
}

ALLEGRO_BITMAP* basic_object::bitmap() {
  return _bm;
}

bool basic_object::active() {
  return _active;
}

void basic_object::active(bool a) {
  _active = a;
}

bool basic_object::collides(basic_object *o) {
  float dx = fabs(_x - o->_x);
  float dy = fabs(_y - o->_y);
  float yr = _h;
  float xr = _w;
  if (_y > o->_y) {
    yr = o->_h;
  }
  if (_x > o->_x) {
    xr = o->_w;
  }

  if ((dx<xr) and (dy<yr)) {
#if 0
    cout << "collision:" << endl << " this("
      << "w:" << _w << ",h:" << _h << ",x:" << _x << ",y:" << _y 
      << ")" << endl;
    cout << " that("
      << "w:" << o->_w << ",h:" << o->_h << ",x:" << o->_x << ",y:" << o->_y
      << ")" << endl;
    cout << "dx:" << dx << " xr:" << xr << "   dy:" << dy << " yr:" << yr 
      << endl;
#endif

    return true;
  }
  return false;
}

