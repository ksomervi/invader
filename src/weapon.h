/** \file
 *
 */
#ifndef WEAPON_H
#define WEAPON_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>

#include "base_object.h"
#include "entity_store.h"

class weapon {

  private:
    entity_store *_cache;
    int _fire_delay;
    int _cfg_delay;
    int _range;

    //ALLEGRO_BITMAP *_bm;
    ALLEGRO_SAMPLE *_deploy_sound;

  public:
    weapon();

    ~weapon();

    bool init(base_object *proto, const int &max);
    void clear_active();
    int count();
    void delay(const int&);
    int delay(void);
    bool fire(const point_2d&);
    _pool& get_active();
    void range(const int&);
    int range();
    bool ready();

    void redraw();

    void sound(ALLEGRO_SAMPLE*);
    ALLEGRO_SAMPLE *sound();

    void update();

};//end class weapon

#endif //!defined(WEAPON_H)

