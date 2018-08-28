#ifndef BASIC_OBJECT_H
#define BASIC_OBJECT_H

#include <allegro5/allegro.h>
#include "defines.h"
#include "point_2d.h"

class basic_object {
  private:
    float _w;
    float _h;
    //float _x;
    //float _y;
    point_2d _loc;

    //float _velx;
    //float _vely;
    point_2d _vel;

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

    void location(const point_2d&); 
    point_2d location(); 

    void velocity(const point_2d&); 
    point_2d velocity(); 

    void velocity_x(float); 
    float velocity_x(void); 

    void velocity_y(float); 
    float velocity_y(void); 

    void move(float, float);
    void move(point_2d);
    void move_to(float, float);
    void move_to(point_2d);

};//end class basic_object

#endif
