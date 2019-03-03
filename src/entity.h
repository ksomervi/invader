#ifndef ENTITY_H
#define ENTITY_H

#include <allegro5/allegro.h>
#include "base_object.h"


class entity : public base_object {
  private:
    int _age;

  public:
    static const int UNLIMITED = INT_MIN;
    entity();
    entity(const entity&);
    base_object* clone() override;
    ~entity();

    void update() override;

    int age();
    void age(const int&);

};//end class entity

#endif
