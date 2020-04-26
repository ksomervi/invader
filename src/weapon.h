/** \file weapon.h
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2020-04-25
 */
#ifndef WEAPON_H
#define WEAPON_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_primitives.h>

class weapon;

#include "base_object.h"
#include "entity_store.h"
#include "mine_controller.h"
//#include "logger.h"
#include "resource_manager.h"

class weapon {

  private:
    entity_store *_cache;
    int _fire_delay;
    int _cfg_delay;
    int _range;

    //Shared graphics for the entities stored
    graphic_component * _graphic;
    ALLEGRO_SAMPLE *_deploy_sound;

    const int _mine_delay = 30;
    const int _blaster_delay = 10;

    ALLEGRO_BITMAP* _default_mine();

    logger *_log;

  public:
    weapon();

    ~weapon();

    bool init(resource_manager*, const char*);
    void clear_active();
    int count();
    void delay(const int&);
    int delay(void);
    bool fire(const point_2d&);
    _pool& get_active();

    void range(const int&);
    int range();

    void redraw();

    void sound(ALLEGRO_SAMPLE*);
    ALLEGRO_SAMPLE *sound();

    void update();

};//end class weapon

#endif //!defined(WEAPON_H)
