#ifndef BASE_LEVEL_H
#define BASE_LEVEL_H

#include "game_controller.h"
#include "fighter.h"
#include "entity_store.h"
#include "level_configuration.h"
using armada = entity_store;

#include "overlay.h"

//enum Difficulty { EASY, NORMAL, HARD };

class base_level {
  private:
    bool _quit;
    bool _complete;

  protected:
    logger *_log;
    resource_manager *_rm;
    level_configuration * _cfg;
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_TIMER *timer;
    ALLEGRO_FONT *title_font;
    ALLEGRO_FONT *textfont;

    overlay *_overlay;

    game_controller *input;

    fighter *hero;
    armada *_foes;

    int hits;

    void intro(int, float);
    void quit(bool);

  public:
    base_level();
    virtual ~base_level() = 0;

    virtual bool play(resource_manager *, level_configuration *) = 0;
    bool complete();
    void complete(bool);

    bool is_paused();
    void toggle_pause();
    void pause_play();
    void resume_play();

    bool quit();
};//end class base_level

#endif //!defined(BASE_LEVEL_H)

