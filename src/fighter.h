#ifndef FIGHTER_H
#define FIGHTER_H

#include "basic_object.h"

#define DEFAULT_LIVES 3

class fighter: public basic_object {
  private:
    int _lives;
    int _health;
    int _max_health;

  public:
    fighter();
    int lives();
    void lives(int);
    void add_lives(int);

    int health();
    int max_health();
    void add_health(int);
    float percent_health();

};//end class fighter
#endif //!defined(FIGHTER_H)

