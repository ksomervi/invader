#ifndef BASIC_OBJECT_H
#define BASIC_OBJECT_H

#include <allegro5/allegro.h>
#include "defines.h"

class basic_object {
  private:
    float _w;
    float _h;
    float _x;
    float _y;

    float _velx;
    float _vely;
    bool _active;
    ALLEGRO_BITMAP *_bm;

  public:
    basic_object();
    basic_object(float, float, float, float);
    ~basic_object();
    void redraw();
    void draw(float, float);
    float x();
    float y();
    void x(float);
    void y(float);

    bool active();
    void active(bool);

    ALLEGRO_BITMAP* bitmap();
    bool create_bitmap(float, float);
    bool create_bitmap(const char*);

    //Perhaps this should be a friend of the class?
    bool collides(basic_object*);

};//end class basic_object

#endif
