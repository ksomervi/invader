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
  _bm = nullptr;
}//end graphic_component

graphic_component::graphic_component(ALLEGRO_BITMAP *bm) {
  _bm = bm;
}//end graphic_component

graphic_component::~graphic_component() {
  if (_bm) {
    al_destroy_bitmap(_bm);
  }
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


void graphic_component::draw(const float &x, const float &y, const float &rot) {
  draw(point_2d(x, y), rot);
}


void graphic_component::draw(const point_2d &p, const float &rot) {
  float cx = w()/2;
  float cy = h()/2;

  float dx = p.x() + cx;
  float dy = p.y() + cy;

  al_draw_rotated_bitmap(_bm, cx, cy, dx, dy, rot, 0);
}

bool graphic_component::create_bitmap(const char *filename) {
  _bm = al_load_bitmap(filename);
  return _bm != NULL;
}

bool graphic_component::create_bitmap(float w, float h) {
  _bm = al_create_bitmap(w, h);
  return _bm != NULL;
}

void graphic_component::destroy_bitmap() {
  al_destroy_bitmap(_bm);
  _bm = nullptr;
  return;
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

