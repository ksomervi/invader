#ifndef BASE_LEVEL_H
#define BASE_LEVEL_H

#include "game.h"
#include "controller.h"
#include "fighter.h"
#include <vector>
#include "entity_store.h"
using armada = entity_store;

enum Difficulty { EASY, NORMAL, HARD };

class base_level {
  private:
    bool _quit;
    bool _complete;
    static logger *_log;

  protected:
    game *env;
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_TIMER *timer;
    ALLEGRO_FONT *title_font;
    ALLEGRO_FONT *textfont;

    controller *input;

    fighter *hero;
    armada *_foes;

    int hits;

    bool init();
    void intro(int);
    void end_level();
    void play_level();
    void print_stats();
    void redraw(float);
    void quit(bool);

  public:
    base_level();
    virtual ~base_level() = 0;

    virtual bool play(game *) = 0;
    bool complete();
    void complete(bool);

    bool is_paused();
    void toggle_pause();
    void pause_play();
    void resume_play();

    bool quit();
};//end class base_level

#endif //!defined(BASE_LEVEL_H)

