#ifndef ENTITY_STORE_H
#define ENTITY_STORE_H

#include <cstdint>
#include <deque>
#include <random>

#include "base_object.h"

using _pool = std::deque< base_object* >;

class entity_store {
  private:
    _pool _store;
    _pool _active;
    int _next_id;

  public:
    entity_store();
    ~entity_store();

    _pool& get_active();
    int count();

    void add(base_object*);
    int deployed();
    void clear_active();
    bool deploy(const float &, const float &);
    bool deploy(const point_2d&);

    void update();
    bool collides(base_object *);
    int check_collisions(_pool *);
    void redraw();

    _pool::iterator begin();
    _pool::iterator end();

};//end class entity_store

#endif //!defined(ENTITY_STORE_H)

