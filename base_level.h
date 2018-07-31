#ifndef BASE_LEVEL_H
#define BASE_LEVEL_H

#include "fighter.h"
#include <vector>
using armada = std::vector<basic_object*>;

class base_level {
  private:

    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_TIMER *timer;
    fighter *hero;
    armada foes;

    int hits;
    int total_foes;


  protected:
    bool init();
    void end_level();
    void play_level();
    void print_stats();
    void redraw(float);

  public:
    base_level();
    virtual ~base_level() = 0;

    virtual bool play(ALLEGRO_DISPLAY*) = 0;

};//end class base_level

#endif //!defined(BASE_LEVEL_H)

