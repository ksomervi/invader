#ifndef LEVEL_1_H
#define LEVEL_1_H 1

#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "base_level.h"

#include "basic_object.h"
#include <vector>
using armada = std::vector<basic_object*>;

#include "fighter.h"
 
enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

class level_1: protected base_level {
  private:
    bool key[4] = { false, false, false, false };
    int _healing_time;

  protected:
    bool init();
    void end_level();
    void play_level();
    void show_stats();
    void redraw(float);
    void update_score();
    bool init_foes(armada&, int max);
    bool activate_foe(armada&, float);

  public:
    level_1();
    ~level_1();

    bool play(game *);

};//end level_1

#endif //!defined(LEVEL_1_H)
