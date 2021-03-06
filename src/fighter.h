/*! \file fighter.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-29
 */

#ifndef FIGHTER_H
#define FIGHTER_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

class fighter;

#include "entity.h"
#include "entity_store.h"
#include "logger.h"
#include "player_controller.h"
#include "resource_manager.h"
#include "weapon.h"

class fighter: public base_object {
  private:
    enum move_state { STILL, LEFT, RIGHT };
    move_state _m_st;
    float _rot;
    ALLEGRO_SAMPLE *_deploy_sound;

    int _lives;
    int _health;
    int _max_health;
    int _healing_time;
    bool _healing;

    weapon *_mines;
    weapon *_blaster;
    ALLEGRO_BITMAP *_blaster_bm;
    ALLEGRO_BITMAP *_mine_bm;
    int _sel_delay;

    logger *_log;

    base_object *clone() override {return nullptr;};

  public:
    fighter();
    ~fighter();
    bool init(resource_manager *);
    void redraw();

    int lives();
    void lives(int);
    void add_lives(int);

    enum weapon_select {primary, secondary};
    bool ready_weapons(resource_manager *);
    bool fire_weapon(const weapon_select &w=primary);
    void swap_weapons();
    void clear_weapons();
    int max_weapons();
    _pool& get_deployed_mines();
    _pool& get_deployed_blasts();

    bool is_alive();
    int health();
    int max_health();
    void max_health(const int&);
    void add_health(int);
    void take_hit(int);
    float percent_health();

    bool handle_event(ALLEGRO_EVENT &);
    void update() override;
    void move(const point_2d&);

    void set_logger(logger *);

  private:
    std::map<weapon_select, weapon*> _arsenal;

};//end class fighter
#endif //!defined(FIGHTER_H)

