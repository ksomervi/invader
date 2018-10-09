#ifndef MINE_CONTROLLER_H
#define MINE_CONTROLLER_H

#include <allegro5/allegro.h>

#include <map>
#include <random>

#include "controller.h"

class mine_controller : public controller {
  private:
    int _mv_count;
    bool _x_side;
    std::default_random_engine generator;

    bool handle_event(ALLEGRO_EVENT &) override;

  public:
    mine_controller();
    ~mine_controller();

    void init() override;

    point_2d direction() override;
    void update(basic_object *) override;

};//end class mine_controller

#endif //!defined(MINE_CONTROLLER_H)

