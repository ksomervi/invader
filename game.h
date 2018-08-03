#ifndef INVADER_GAME_H
#define INVADER_GAME_H

#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

//#include "basic_object.h"
//#include <vector>
//using armada = std::vector<basic_object*>;

#include "fighter.h"
 
class game {
  private:
    ALLEGRO_DISPLAY *display;
    //ALLEGRO_EVENT_QUEUE *event_queue;
    //ALLEGRO_TIMER *timer;
    ALLEGRO_FONT *title_font;
    ALLEGRO_FONT *font;

    fighter *hero;
    //armada foes;

    int hits;
    int total_foes;

  public:
    game();
    ~game();
    ALLEGRO_DISPLAY *get_display();
    ALLEGRO_FONT *get_font(int);

    bool init();
    void play();
    void print_score();
    void end();

};//end game

#endif //!defined(INVADER_GAME_H)
