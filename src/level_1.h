#ifndef LEVEL_1_H
#define LEVEL_1_H 1

#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "base_level.h"

//#include "basic_object.h"
//#include <vector>
//using armada = std::vector<basic_object*>;
// FIXME: move this to the fighter
using weapons = std::vector<basic_object*>;

//#include "fighter.h"

// FIXME: move control input to its own class
enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE
};

class level_1: protected base_level {
  private:
    bool key[5] = { false, false, false, false, false };
    int _healing_time;

    ALLEGRO_BITMAP *mine_bm = NULL;
    ALLEGRO_BITMAP *foe_bm = NULL;
    weapons mines;
    ALLEGRO_SAMPLE *hit_sound = NULL;
    ALLEGRO_SAMPLE *deploy_sound = NULL;

  protected:
    bool init();
    void end_level();
    void play_level();
    void restart_level();
    void show_stats();
    void redraw(float);
    void update_score();
    bool init_foes(armada&, int max);
    bool activate_foe(armada&, float);
    bool deploy_mine(weapons&, int, int);
    bool init_weapons(weapons&, int);

  public:
    level_1();
    ~level_1();

    bool play(game *);

};//end level_1

#endif //!defined(LEVEL_1_H)
