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
  _loc.x((SCREEN_W - SPRITE_SIZE) / 2.0);
  _loc.y((SCREEN_H - SPRITE_SIZE) / 2.0);

  _vel = point_2d(0.0, 0.0);

  _bm = NULL;
  _active = false;
}//end basic_object

basic_object::~basic_object() {
}

void basic_object::bound(const point_2d &min, const point_2d &max) {
  std::cerr << " bound: (" << min.x() << ", " << min.y()
    << ") - ("<< max.x() << ", " << max.y() << ")" << std::endl;

  _min_bounds = min;
  _max_bounds = max;
}

float basic_object::x() {
  return _loc.x();
}

float basic_object::y() {
  return _loc.y();
}

float basic_object::w() {
  if (_bm) {
    return al_get_bitmap_width(_bm);
  }
  return 0.0;
}

float basic_object::h() {
  if (_bm) {
    return al_get_bitmap_height(_bm);
  }
  return 0.0;
}

void basic_object::x(float v) {
  _loc.x(v);
}

void basic_object::y(float v) {
  _loc.y(v);
}

void basic_object::redraw(const float &rotation) {
  float cx = w()/2;
  float cy = h()/2;

  float dx = _loc.x() + cx;
  float dy = _loc.y() + cy;

  al_draw_rotated_bitmap(_bm, cx, cy, dx, dy, rotation, 0);
}

void basic_object::draw(const point_2d &p) {
  _loc = p;
  al_draw_bitmap(_bm, _loc.x(), _loc.y(), 0);
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

void basic_object::bitmap(ALLEGRO_BITMAP* bm) {
  _bm = bm;
  _w = 0.0;
  _h = 0.0;
  if (_bm) {
    _w = al_get_bitmap_width(_bm);
    _h = al_get_bitmap_height(_bm);
  }
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
  float dx = fabs(_loc.x() - o->_loc.x());
  float dy = fabs(_loc.y() - o->_loc.y());
  float yr = _h;
  float xr = _w;
  if (_loc.y() > o->_loc.y()) {
    yr = o->_h;
  }
  if (_loc.x() > o->_loc.x()) {
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

void basic_object::location(const point_2d& l) {
  _loc = l;
}

point_2d basic_object::location() {
  return _loc;
}

void basic_object::velocity(const point_2d& v) {
  _vel = v;
}

point_2d basic_object::velocity() {
  return _vel;
}

void basic_object::velocity_x(float vx) {
  _vel.x(vx);
}//end

float basic_object::velocity_x(void) {
  return _vel.x();
}//end

void basic_object::velocity_y(float vy) {
  _vel.y(vy);
}//end

float basic_object::velocity_y(void) {
  return _vel.y();
}//end

void basic_object::move(point_2d p) {
  _loc += p;
}//end

void basic_object::move_to(point_2d p) {
  _loc = p;
}//end

void basic_object::id(const int &i) {
  _id = i;
}

int basic_object::id() {
  return _id;
}

