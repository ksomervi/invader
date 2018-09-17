/*! \file fighter.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-29
 */

#include "fighter.h"
#include<iostream>

fighter::fighter() {
  _lives = DEFAULT_LIVES;
  _max_health = 100;
  _health = _max_health;
  _healing_time = 0;
  _healing = false;
};

fighter::~fighter() {
}

int fighter::lives() {
  return _lives;
}

void fighter::lives(int l) {
  _lives = l;
}

void fighter::add_lives(int l) {

  if (l < 0) {
    if (_lives == 0) {
      return;
    }
    _health = _max_health;
  }

  _lives += l;
}


int fighter::health() {
  return _health;
}

int fighter::max_health() {
  return _max_health;
}

void fighter::add_health(int h) {
  _health += h;

  if (_health < 0) {
    _health = 0;
    std::cout << "player died! restart level" << std::endl;
  }
  else if (_health < _max_health) {
    _healing = true;
    _healing_time = 60;
  }
  else {
    _healing = false;
    if (_health > _max_health) {
      _health = _max_health;
    }
  }
}//end fighter::add_health()


void fighter::take_hit(int damage) {
  add_health(-1*damage);
}


float fighter::percent_health() {
  return float(_health) / _max_health;
}

void fighter::bound(const point_2d &min, const point_2d &max) {
  std::cerr << " bound: (" << min.x() << ", " << min.y()
    << ") - ("<< max.x() << ", " << max.y() << ")" << std::endl;

  _min_bounds = min;
  _max_bounds = max;
}

void fighter::move(const point_2d &offset) {
  point_2d next_loc(location() + offset);

  if (next_loc.x() < _min_bounds.x()) {
    next_loc.x(_min_bounds.x());
  }
  if (next_loc.x() > _max_bounds.x()) {
    next_loc.x(_max_bounds.x());
  }
  if (next_loc.y() < _min_bounds.y()) {
    next_loc.y(_min_bounds.y());
  }
  if (next_loc.y() > _max_bounds.y()) {
    next_loc.y(_max_bounds.y());
  }

  location(next_loc);
}//end

void fighter::update() {
  if (_healing) {
    if (_healing_time > 0) {
      _healing_time--;
    }
    else {
      this->add_health(2);
    }
  }
}//end update()

