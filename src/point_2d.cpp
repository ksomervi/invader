/*! \file point_2d.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-08-27
 */

#include "point_2d.h"

point_2d::point_2d() {
  _x = 0.0;
  _y = 0.0;
}

point_2d::point_2d(float x, float y):_x(x), _y(y) {
}

point_2d::point_2d(const point_2d & p) {
  _x = p._x;
  _y = p._y;
}

void point_2d::x(float v) {
  _x = v;
}

void point_2d::y(float v) {
  _y = v;
}

float point_2d::x() const {
  return _x;
}

float point_2d::y() const {
  return _y;
}

bool point_2d::operator==(const point_2d& p) {
  return ((_x == p._x) && (_y == p._y));
}

bool point_2d::operator!=(const point_2d& p) {
  return !(*this == p);
}

point_2d& point_2d::operator=(const point_2d& p) {
  if(this != &p) {
    _x = p._x;
    _y = p._y;
  }
  return *this;
}

point_2d& point_2d::operator+=(const point_2d& p) {
  _x += p._x;
  _y += p._y;
  return *this;
}

point_2d& point_2d::operator-=(const point_2d& p) {
  _x -= p._x;
  _y -= p._y;
  return *this;
}

point_2d operator+(point_2d lhs, const point_2d& rhs) {
  lhs += rhs;
  return lhs;
}

point_2d operator-(point_2d lhs, const point_2d& rhs) {
  lhs -= rhs;
  return lhs;
}

