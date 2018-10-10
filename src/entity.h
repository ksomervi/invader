#ifndef ENTITY_H
#define ENTITY_H

#include <allegro5/allegro.h>
#include "base_object.h"

class entity : public base_object {
  private:

  public:
    entity();
    ~entity();

    void update() override;

};//end class entity

#endif
