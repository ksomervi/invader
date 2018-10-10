#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <allegro5/allegro.h>
#include <map>

#include "base_controller.h"
#include "point_2d.h"

class player_controller : public base_controller {
  private:

    std::map<int, bool> _key_pressed;

  public:
    player_controller();
    ~player_controller();

    void init();
    bool handle_event(ALLEGRO_EVENT &) override;

    point_2d direction();
    float up();
    float down();
    float left();
    float right();
    bool fire();

    void update(base_object *) override;

};//end class player_controller

#endif //!defined(PLAYER_CONTROLLER_H)

