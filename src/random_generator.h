#ifndef _RANDOM_GENERATOR_H
#define _RANDOM_GENERATOR_H

#include <random>

class random_generator {
  private:
    std::default_random_engine _generator;

  public:
    random_generator();

    int random_int(const int&, const int&);
    float random_float(const float&, const float&);
}; //end class random_generator

#endif //!defined(_RANDOM_GENERATOR_H)
