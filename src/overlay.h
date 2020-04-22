/*! \file overlay.h
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-10-18
 */
#ifndef OVERLAY_H
#define OVERLAY_H 13

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "fighter.h"
#include "resource_manager.h"

class overlay {

  private:
    ALLEGRO_DISPLAY *_display;
    ALLEGRO_COLOR _text_color;
    ALLEGRO_COLOR _fill_color;
    ALLEGRO_FONT *_textfont;
    ALLEGRO_FONT *_title_font;

    int _alpha;

    overlay();

  public:
    overlay(resource_manager *);

    ~overlay();

    void init(resource_manager*);

    void redraw(int, int, fighter*);

};//end class overlay

#endif //!defined(OVERLAY_H)
