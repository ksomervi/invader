#ifndef ENTITY_H
#define ENTITY_H

#include <allegro5/allegro.h>
#include "basic_object.h"

class entity : public basic_object {
  private:

  public:
    entity();
    ~entity();

    void update() override;

};//end class entity

#endif
