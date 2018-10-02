#ifndef LEVEL_1_H
#define LEVEL_1_H 1

#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "base_level.h"

class level_1: protected base_level {
  private:
    ALLEGRO_BITMAP *mine_bm = NULL;
    ALLEGRO_BITMAP *foe_bm = NULL;
    ALLEGRO_SAMPLE *hit_sound = NULL;
    ALLEGRO_SAMPLE *bg_sound = NULL;

    point_2d _min_bounds;
    point_2d _max_bounds;

  protected:
    bool init();
    void end_level();
    void play_level();
    void restart_level();
    void show_stats();
    void check_collisions();
    void redraw();
    void update_score();
    bool init_foes(int);

  public:
    level_1();
    ~level_1();

    bool play(resource_manager *) override;

};//end class level_1

#endif //!defined(LEVEL_1_H)
