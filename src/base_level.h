#ifndef BASE_LEVEL_H
#define BASE_LEVEL_H

#include "game.h"
#include "fighter.h"
#include <vector>
using armada = std::vector<basic_object*>;

class base_level {
  private:

  protected:
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_TIMER *timer;
    ALLEGRO_FONT *pfont;
    ALLEGRO_FONT *textfont;

    fighter *hero;
    armada foes;

    int hits;
    int total_foes;


    bool init();
    void end_level();
    void play_level();
    void print_stats();
    void redraw(float);

  public:
    base_level();
    virtual ~base_level() = 0;

    virtual bool play(game *) = 0;

};//end class base_level

#endif //!defined(BASE_LEVEL_H)
