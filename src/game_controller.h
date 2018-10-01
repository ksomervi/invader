#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <allegro5/allegro.h>
#include <map>

#include "controller.h"
#include "point_2d.h"

class game_controller : public controller {
  private:

    std::map<int, bool> _key_pressed;
    point_2d _dir;

    point_2d direction() override;
    void update(basic_object *) override;

  public:
    game_controller();
    ~game_controller();

    void init();
    bool handle_event(ALLEGRO_EVENT &) override;

    /*
    float up();
    float down();
    float left();
    float right();
    bool fire();
    */

    bool pause_event();
    bool quit();

};//end class game_controller

#endif //!defined(GAME_CONTROLLER_H)

