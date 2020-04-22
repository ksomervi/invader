/*! \file overlay.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-29
 */

#include <allegro5/allegro.h>
#include "overlay.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <cstdlib>

overlay::overlay(resource_manager *rm) {
  // Playable region
  _display = rm->get_display();
  _title_font = rm->get_font(TITLE);
  _textfont = rm->get_font();

  _alpha = 255 * atof(rm->option("DISPLAY", "overlay transparency"));
  _text_color = al_premul_rgba(255, 255, 255, _alpha);
  _fill_color = al_premul_rgba(153, 204, 0, _alpha);
}

overlay::~overlay() {
}

// Redraws the overlay game statistics overlay.
// Input needed:
// - total foes deployed
// - hits
// - hero statistics (lives, health, mines deployed, max_weapons)
//
void overlay::redraw(int foes, int hits, fighter* hero) {
  float x_base = al_get_display_width(_display) - 220;
  float x_loc = x_base;
  float y_loc = TITLE_Y;

  float y_step = al_get_font_line_height(_textfont);

  int kill_eff = 0;
  if (foes > 0) {
    kill_eff = hits*100/foes;
  }

  al_draw_text(_title_font, _text_color,
      al_get_display_width(_display)/2, TITLE_Y, ALLEGRO_ALIGN_RIGHT,
      GAME_TITLE);

  al_draw_text(_textfont, _text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "Lives:");

  x_loc += 100;
  al_draw_textf(_textfont, _text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_RIGHT, "%d", hero->lives());

  x_loc = x_base;
  y_loc += y_step;
  al_draw_text(_textfont, _text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "Health:");

  x_loc += 100;
  float h_bar_len = 100;
  float h_bar_fill = h_bar_len * hero->percent_health();
  _fill_color = al_premul_rgba(153, 204, 0, _alpha);
  if (hero->percent_health() < 0.4) {
    //FIXME: red should also have same alpha
    _fill_color = RED;
  }
  if (h_bar_fill > 0.0) {
    al_draw_filled_rounded_rectangle(x_loc, y_loc,
        x_loc+h_bar_fill, y_loc+y_step, 5, 5, _fill_color);
  }

  al_draw_rounded_rectangle(x_loc, y_loc, x_loc+h_bar_len, y_loc+y_step,
      5, 5, _text_color, 4);

  al_draw_textf(_textfont, _text_color, x_loc+h_bar_len/2.0, y_loc,
      ALLEGRO_ALIGN_CENTRE, "%d", hero->max_health());

  x_loc = x_base;
  y_loc += y_step;
  al_draw_text(_textfont, _text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "Kills:");

  x_loc += 100;
  al_draw_textf(_textfont, _text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_RIGHT, "%d", hits);

  x_loc += 14;
  al_draw_text(_textfont, _text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_CENTRE, ":");

  x_loc += 14;
  al_draw_textf(_textfont, _text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "%d%%", kill_eff);

  x_loc = x_base;
  y_loc += y_step;
  al_draw_text(_textfont, _text_color, x_loc, y_loc, ALLEGRO_ALIGN_LEFT, "Mines:");

  x_loc += 100;
  int ready = hero->get_deployed_mines().size();
  al_draw_textf(_textfont, _text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_RIGHT, "%d", ready);

  x_loc += 14;
  al_draw_text(_textfont, _text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_CENTRE, ":");

  x_loc += 14;
  al_draw_textf(_textfont, _text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "%d", hero->max_weapons());

}//end overlay::redraw(int, int, fighter*)

