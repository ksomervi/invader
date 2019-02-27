

#include "entity_store.h"
#include <iostream>
using std::cerr;
using std::endl;

entity_store::entity_store() {
  _next_id = 0;
  _store.clear();
  _active.clear();
  _log = nullptr;
}

/*
class entity_store::entity_store(int sz) {
  cerr << "Initializing store..." << endl;

  int cnt = 0;

  foe_bm = env->get_sprite("creeper");

  if (!foe_bm) {
    cerr << "failed to load bitmap for foe" << endl;
    foe_bm = al_create_bitmap(SPRITE_SIZE, SPRITE_SIZE);

    al_set_target_bitmap(foe_bm);
    al_clear_to_color(al_map_rgb(37, 196, 23));
    al_set_target_backbuffer(display);
  }

  for (auto& e: _pool) {
    cerr << ".";
    cnt++;
    e = new base_object();
    e->bitmap(foe_bm);
    e->y(SCREEN_H); // default to off screen
  }
  cerr << endl;
  return true;

}
*/

entity_store::~entity_store() {
}

void entity_store::set_logger(logger *l) {
  _log = l;
}

_pool& entity_store::get_active() {
  return _active;
}

void entity_store::clear_active() {
  auto it = _active.begin();
  while (it != _active.end()) {
    (*it)->active(false);
    it = _active.erase(it);
  }
}

int entity_store::count() {
  return _store.size();
}

void entity_store::add(base_object* o) {
  _store.push_back(o);
}

int entity_store::deployed() {
  return _next_id;
}

base_object* entity_store::deploy(const float &x, const float &y) {
  return deploy(point_2d(x, y));
}

base_object* entity_store::deploy(const point_2d &p) {
  for (auto &f: _store) {
    if (f->active() == false) {
      f->move_to(p);
      f->id(++_next_id);
      f->active(true);
      if (_log) {
        _log->debug("foe[" + std::to_string(_next_id) + "] starting at " 
            + std::to_string(p.x()));
      }
      _active.push_back(f);
      if (_log) {
        _log->debug("active enemy: " + std::to_string(_active.size()));
      }
      return f;
    }
  }

  if (_log) {
    _log->debug("all foes active");
  }
  return nullptr;
}

void entity_store::update() {
  base_object *o = nullptr;

  for (auto it=_active.begin(); it!=_active.end();) {
    o = *it;
    o->update();

    if (o->active()) {
      it++;
    }
    else {
      it = _active.erase(it);
    }
  }
}

void entity_store::redraw() {
  for (auto &e: _active) {
    e->redraw();
  }
}

bool entity_store::collides(base_object *other) {
  base_object *o = nullptr;

  for (auto it=_active.begin(); it!=_active.end();) {
    o = *it;

    if (o->collides(other)) {
      _log->debug("collided with enemy: " + std::to_string(o->id()));
      o->active(false);
      it = _active.erase(it);
      return true;
    }
    else {
      it++;
    }
  }
  return false;
}

int entity_store::check_collisions(_pool *other) {
  int rv = 0;
  base_object *o = nullptr;

  for (auto it=other->begin(); it!=other->end();) {
    o = *it;
    if (collides(o)) {
      rv++;
      o->active(false);
      it = other->erase(it);
    }
    else {
      it++;
    }
  }//end for (auto it=other->_active.begin(); it!=other->_active.end();)
  return rv;
}

_pool::iterator entity_store::begin() {
  return _store.begin();
}

_pool::iterator entity_store::end() {
  return _store.end();
}

