#ifndef GRAPHIC_COMPONENT_H
#define GRAPHIC_COMPONENT_H

#include <allegro5/allegro.h>
#include "defines.h"
#include "point_2d.h"

class graphic_component {
  private:
    float _w;
    float _h;

  protected:
    ALLEGRO_BITMAP *_bm;
    point_2d _loc;

  public:
    graphic_component();
    virtual ~graphic_component();
    void draw(const float& =0.0);
    void draw(const point_2d&, const float& =0.0);
    float w();
    float h();
    float x();
    float y();
    void x(float);
    void y(float);

    void bitmap(ALLEGRO_BITMAP*);
    ALLEGRO_BITMAP* bitmap();
    bool create_bitmap(float, float);
    bool create_bitmap(const char*);

    void location(const point_2d&);
    point_2d location();

    void move(point_2d);
    void move_to(point_2d);

};//end class graphic_component

#endif //!defined(GRAPHIC_COMPONENT_H)
