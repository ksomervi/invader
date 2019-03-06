#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include <allegro5/allegro.h>

#include "defines.h"

class base_object;

#include "base_controller.h"
#include "point_2d.h"

class base_object {
  private:
    int _id;

    ALLEGRO_BITMAP *_bm;

  protected:
    base_controller * _ctrl;
    point_2d _loc;
    point_2d _vel;
    bool _active;
    point_2d _min_bounds;
    point_2d _max_bounds;

  public:
    base_object();
    base_object(const base_object&);

    virtual ~base_object() = 0;

    virtual base_object *clone() = 0;
    void controller(base_controller*);
    base_controller* controller();

    virtual void update() = 0;

    void redraw(const float& =0.0);
    void draw(const point_2d&);
    void bound(const point_2d&, const point_2d&);
    void min_bounds(const point_2d&);
    point_2d min_bounds();
    void max_bounds(const point_2d&);
    point_2d max_bounds();

    float w();
    float h();
    float x();
    float y();
    void x(float);
    void y(float);
    float cx();
    float cy();

    bool active();
    void active(bool);

    void bitmap(ALLEGRO_BITMAP*);
    ALLEGRO_BITMAP* bitmap();
    bool create_bitmap(float, float);
    bool create_bitmap(const char*);

    //Perhaps this should be a friend of the class?
    bool collides(base_object*);

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


    void id(const int&);
    int id();

};//end class base_object

#endif //!defined(BASE_OBJECT_H)
