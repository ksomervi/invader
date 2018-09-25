#ifndef FIGHTER_H
#define FIGHTER_H

#include "basic_object.h"

//#define DEFAULT_LIVES 4

class fighter: public basic_object {
  private:
    int _lives;
    int _health;
    int _max_health;
    int _healing_time;
    bool _healing;

  public:
    fighter();
    ~fighter();
    int lives();
    void lives(int);
    void add_lives(int);

    int health();
    int max_health();
    void add_health(int);
    void take_hit(int);
    float percent_health();

    void update();
    void move(const point_2d&);

};//end class fighter
#endif //!defined(FIGHTER_H)

