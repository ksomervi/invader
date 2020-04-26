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

  public:
    graphic_component();
    graphic_component(ALLEGRO_BITMAP*);
    //graphic_component(const char*);

    virtual ~graphic_component();

    void draw(const float&, const float&, const float& =0.0);
    void draw(const point_2d&, const float& =0.0);
    float w();
    float h();

    void bitmap(ALLEGRO_BITMAP*);
    ALLEGRO_BITMAP* bitmap();
    bool create_bitmap(float, float);
    bool create_bitmap(const char*);
    void destroy_bitmap();

};//end class graphic_component

#endif //!defined(GRAPHIC_COMPONENT_H)
