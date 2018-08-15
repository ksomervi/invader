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
};

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

  if (_health > _max_health) {
    _health = _max_health;
  }
}

float fighter::percent_health() {
  return float(_health) / _max_health;
}

