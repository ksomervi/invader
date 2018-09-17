#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <allegro5/allegro.h>
#include <map>

#include <iostream>
using std::cerr;
using std::endl;

#include "point_2d.h"

class controller{
  private:
    enum CTRL_KEYS {
      KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE
    };

    bool _key[5];
    std::map<int, bool> _key_pressed;
    point_2d _dir;


  public:
    controller();
    ~controller();

    void init();
    bool handle_event(ALLEGRO_EVENT &);

    point_2d direction();
    float up();
    float down();
    float left();
    float right();
    bool fire();

    bool pause_event();
    bool quit();

};//end class controller

#endif //!defined(CONTROLLER_H)

