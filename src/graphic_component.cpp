/*! \file graphic_component.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-27
 */

#include "graphic_component.h"

#include <cmath>
#include <iostream>
using std::cout;
using std::endl;

graphic_component::graphic_component() {
  //Initial location, middle of the screen
  _loc.x((SCREEN_W - SPRITE_SIZE) / 2.0);
  _loc.y((SCREEN_H - SPRITE_SIZE) / 2.0);

  _bm = NULL;
}//end graphic_component

graphic_component::~graphic_component() {
  if (_bm) {
    al_destroy_bitmap(_bm);
  }
}

float graphic_component::x() {
  return _loc.x();
}

float graphic_component::y() {
  return _loc.y();
}

float graphic_component::w() {
  if (_bm) {
    return al_get_bitmap_width(_bm);
  }
  return 0.0;
}

float graphic_component::h() {
  if (_bm) {
    return al_get_bitmap_height(_bm);
  }
  return 0.0;
}

void graphic_component::x(float v) {
  _loc.x(v);
}

void graphic_component::y(float v) {
  _loc.y(v);
}

void graphic_component::draw(const float &rotation) {
  float cx = w()/2;
  float cy = h()/2;

  float dx = _loc.x() + cx;
  float dy = _loc.y() + cy;

  al_draw_rotated_bitmap(_bm, cx, cy, dx, dy, rotation, 0);
}

void graphic_component::draw(const point_2d &p, const float &rot) {
  _loc = p;
  draw(rot);
}

bool graphic_component::create_bitmap(const char *filename) {
  _bm = al_load_bitmap(filename);
  return _bm != NULL;
}

bool graphic_component::create_bitmap(float w, float h) {
  _bm = al_create_bitmap(w, h);
  return _bm != NULL;
}

void graphic_component::bitmap(ALLEGRO_BITMAP* bm) {
  _bm = bm;
}

ALLEGRO_BITMAP* graphic_component::bitmap() {
  return _bm;
}

/*
bool graphic_component::collides(graphic_component *g) {
  float dx = fabs(_loc.x() - g->_loc.x());
  float dy = fabs(_loc.y() - g->_loc.y());
  float yr = h();
  float xr = w();
  if (_loc.y() > g->_loc.y()) {
    yr = g->h();
  }
  if (_loc.x() > g->_loc.x()) {
    xr = g->w();
  }

  return ((dx<xr) and (dy<yr));
}
*/

void graphic_component::location(const point_2d& l) {
  _loc = l;
}

point_2d graphic_component::location() {
  return _loc;
}

void graphic_component::move(point_2d p) {
  _loc += p;
}//end

void graphic_component::move_to(point_2d p) {
  _loc = p;
}//end

