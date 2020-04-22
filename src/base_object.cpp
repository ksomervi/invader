/*! \file base_object.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-27
 */

#include "base_object.h"

#include <cmath>

base_object::base_object() : _loc(), _vel() {
  _id = 0;
  _bm = nullptr;
  _active = false;
  _ctrl = nullptr;
}//end base_object

base_object::base_object(const base_object &other) : _loc(other._loc), _vel(other._vel) {
  _id = other._id;
  _bm = other._bm;
  _active = other._active;
  _ctrl = other._ctrl;
  _min_bounds = other._min_bounds;
  _max_bounds = other._max_bounds;
}//end base_object

base_object::~base_object() {
}

void base_object::controller(base_controller *c) {
  _ctrl = c;
}

base_controller* base_object::controller() {
  return _ctrl;
}

void base_object::bound(const point_2d &min, const point_2d &max) {
  _min_bounds = min;
  _max_bounds = max;
}

void base_object::min_bounds(const point_2d &min) {
  _min_bounds = min;
}

point_2d base_object::min_bounds() {
  return _min_bounds;
}

void base_object::max_bounds(const point_2d &max) {
  _max_bounds = max;
}

point_2d base_object::max_bounds() {
  return _max_bounds;
}

float base_object::x() {
  return _loc.x();
}

float base_object::y() {
  return _loc.y();
}

float base_object::w() {
  if (_bm) {
    return al_get_bitmap_width(_bm);
  }
  return 0.0;
}

float base_object::h() {
  if (_bm) {
    return al_get_bitmap_height(_bm);
  }
  return 0.0;
}

void base_object::x(float v) {
  _loc.x(v);
}

void base_object::y(float v) {
  _loc.y(v);
}

float base_object::cx() {
  return _loc.x() + (w()/2);
}

float base_object::cy() {
  return _loc.y() + (h()/2);
}

void base_object::redraw(const float &rotation) {
  float c_x = w()/2;
  float c_y = h()/2;

  float dx = _loc.x() + c_x;
  float dy = _loc.y() + c_y;

  al_draw_rotated_bitmap(_bm, c_x, c_y, dx, dy, rotation, 0);
}

void base_object::draw(const point_2d &p) {
  _loc = p;
  al_draw_bitmap(_bm, _loc.x(), _loc.y(), 0);
}

bool base_object::create_bitmap(const char *filename) {
  _bm = al_load_bitmap(filename);
  return _bm != NULL;
}

bool base_object::create_bitmap(float w, float h) {
  _bm = al_create_bitmap(w, h);
  return _bm != NULL;
}

void base_object::bitmap(ALLEGRO_BITMAP* bm) {
  _bm = bm;
}

ALLEGRO_BITMAP* base_object::bitmap() {
  return _bm;
}

bool base_object::active() {
  return _active;
}

void base_object::active(bool a) {
  _active = a;
}

bool base_object::collides(base_object *o) {
  float dx = fabs(_loc.x() - o->_loc.x());
  float dy = fabs(_loc.y() - o->_loc.y());
  float yr = h();
  float xr = w();
  if (_loc.y() > o->_loc.y()) {
    yr = o->h();
  }
  if (_loc.x() > o->_loc.x()) {
    xr = o->w();
  }

  if ((dx<xr) and (dy<yr)) {
    return true;
  }
  return false;
}

void base_object::location(const point_2d& l) {
  _loc = l;
}

point_2d base_object::location() {
  return _loc;
}

void base_object::velocity(const point_2d& v) {
  _vel = v;
}

point_2d base_object::velocity() {
  return _vel;
}

void base_object::move(point_2d p) {
  _loc += p;
}//end

void base_object::move_to(point_2d p) {
  _loc = p;
}//end

void base_object::id(const int &i) {
  _id = i;
}

int base_object::id() {
  return _id;
}

