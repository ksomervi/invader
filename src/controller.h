#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <allegro5/allegro.h>
#include <map>

class controller;
#include "basic_object.h"
#include "point_2d.h"

class controller {
  private:

  protected:
    point_2d _dir;

  public:
    controller();
    virtual ~controller() = 0;

    virtual void init();
    virtual bool handle_event(ALLEGRO_EVENT &) = 0;

    virtual point_2d direction() = 0;
    virtual void update(basic_object *) = 0;

};//end class controller

#endif //!defined(CONTROLLER_H)

