#ifndef BASE_CONTROLLER_H
#define BASE_CONTROLLER_H

#include <allegro5/allegro.h>
#include <map>

class base_controller;
#include "base_object.h"
#include "point_2d.h"

class base_controller {
  private:

  protected:
    point_2d _dir;

  public:
    base_controller();
    virtual ~base_controller() = 0;

    virtual void init();
    virtual bool handle_event(ALLEGRO_EVENT &) = 0;

    virtual point_2d direction() = 0;
    virtual void update(base_object *) = 0;

};//end class base_controller

#endif //!defined(BASE_CONTROLLER_H)

