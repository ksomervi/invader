/*! \file random_generator.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-11-13
 */


#include "random_generator.h"

random_generator::random_generator() {
}//end random_generator

int random_generator::random_int(const int &l, const int &h) {
  std::uniform_int_distribution<int> int_distribution(l, h);
  return int_distribution(_generator);
}

float random_generator::random_float(const float &l, const float &h) {
  std::uniform_real_distribution<float> float_distribution(l, h);
  return float_distribution(_generator);
}

