#ifndef INVADER_GAME_H
#define INVADER_GAME_H

#include <cstdio>
//#include <allegro5/allegro.h>

#include "basic_object.h"
#include <vector>
using armada = std::vector<basic_object*>;

#include "fighter.h"
 
enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

class game {
  private:
    const float FPS = 60.0;
    //const int SCREEN_W = 640;
    //const int SCREEN_H = 480;

    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_TIMER *timer;
    fighter *hero;
    armada foes;

    int hits;
    int total_foes;

    bool key[4] = { false, false, false, false };

  public:
    game();
    game(float w, float h);
    ~game();

    bool init();
    bool init_foes(armada&, int max);
    bool activate_foe(armada&, float);

    void play();
    void print_score();
    void end();

    void redraw(float);

};//end game

#endif //!defined(INVADER_GAME_H)
