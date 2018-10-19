/*! \file level_configuration.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-10-18
 */

#include "level_configuration.h"

level_configuration::level_configuration() : _waves() {
  _diff = NORMAL;
};//end level_configuration()

level_configuration::level_configuration(const int &level, 
    const vector<int> &v,
    const Difficulty &d) : _id(level), _waves(v), _diff(d) {

};//end level_configuration()

void level_configuration::level(const int &level) {
  _id = level;
};//end level(const int&)

int level_configuration::level() {
  return _id;
};//end level()

void level_configuration::enemy_waves(const vector<int> &v) {
  _waves = v;
};//end enemy_waves()

vector<int>& level_configuration::enemy_waves() {
  return _waves;
};//end enemy_waves()

int level_configuration::enemy_wave(const size_t &w) {
  if (w < _waves.size()) {
    return _waves[w];
  }
  return 0;
};//end enemy_waves()

Difficulty level_configuration::difficulty() {
  return _diff;
};//end difficulty()

void level_configuration::difficulty(const Difficulty &d) {

};//end difficulty(const Difficulty &)


