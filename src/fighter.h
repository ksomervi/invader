#ifndef FIGHTER_H
#define FIGHTER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

class fighter;
#include "entity.h"
#include "basic_object.h"
#include "player_controller.h"
#include "entity_store.h"
#include "resource_manager.h"

class fighter: public basic_object {
  private:
    using weapons = entity_store;
    enum move_state { STILL, LEFT, RIGHT };
    move_state _m_st;
    float _rot;
    ALLEGRO_SAMPLE *_deploy_sound;

    int _lives;
    int _health;
    int _max_health;
    int _healing_time;
    bool _healing;

    weapons *_mines;
    ALLEGRO_BITMAP *_mine_bm;
    int _fire_delay;

  public:
    fighter();
    ~fighter();
    bool init(resource_manager *);
    void redraw();

    int lives();
    void lives(int);
    void add_lives(int);

    bool ready_weapons(basic_object*, const int&);
    bool fire_weapon();
    void clear_weapons();
    int max_weapons();
    _pool& get_deployed_mines();

    int health();
    int max_health();
    void add_health(int);
    void take_hit(int);
    float percent_health();

    bool handle_event(ALLEGRO_EVENT &);
    void update() override;
    void move(const point_2d&);

};//end class fighter
#endif //!defined(FIGHTER_H)

