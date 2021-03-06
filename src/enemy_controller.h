#ifndef ENEMY_CONTROLLER_H
#define ENEMY_CONTROLLER_H

#include <allegro5/allegro.h>

#include <map>

#include "base_controller.h"
#include "random_generator.h"

class enemy_controller : public base_controller {
  private:
    int _mv_count;
    random_generator *_rg;

    bool handle_event(ALLEGRO_EVENT &) override;

  public:
    enemy_controller();
    ~enemy_controller();

    void init() override;

    point_2d direction() override;
    void update(base_object *) override;

};//end class enemy_controller

#endif //!defined(ENEMY_CONTROLLER_H)

