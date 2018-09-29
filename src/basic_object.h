#ifndef BASIC_OBJECT_H
#define BASIC_OBJECT_H

#include <allegro5/allegro.h>
#include "defines.h"
#include "point_2d.h"

class basic_object {
  private:
    float _w;
    float _h;
    int _id;

    ALLEGRO_BITMAP *_bm;

  protected:
    point_2d _loc;
    point_2d _vel;
    bool _active;
    point_2d _min_bounds;
    point_2d _max_bounds;

  public:
    basic_object();
    basic_object(float, float, float, float);
    virtual ~basic_object();
    void redraw(const float& =0.0);
    void draw(const point_2d&);
    void bound(const point_2d&, const point_2d&);
    float w();
    float h();
    float x();
    float y();
    void x(float);
    void y(float);

    bool active();
    void active(bool);

    void bitmap(ALLEGRO_BITMAP*);
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

    void move(point_2d);
    void move_to(point_2d);

    virtual void update();

    void id(const int&);
    int id();

};//end class basic_object

#endif
