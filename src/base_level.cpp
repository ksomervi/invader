/*! \file base_level.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-30
 */

#include "base_level.h"
#include <iostream>
using std::cerr;
using std::endl;

base_level::base_level() {
  display = NULL;
  event_queue = NULL;
  timer = NULL;
  
  hero = NULL;

  hits = 0;
  total_foes = 0;

  title_font = NULL;
  textfont = NULL;

  _complete = false;
  _quit = false;
};//end base_level::base_level()

base_level::~base_level() {

};//end base_level::~base_level()

void base_level::intro(int level) {
  float textbox_h = al_get_font_line_height(title_font) +
    4 * al_get_font_line_height(textfont);
  float textbox_w = 0.8 * SCREEN_W;

  ALLEGRO_COLOR text_color = WHITE;
  ALLEGRO_BITMAP *textbox = al_create_bitmap(textbox_w, textbox_h);
  float x_base = al_get_bitmap_width(textbox)/2;
  float x_loc = x_base;
  float y_loc = TITLE_Y;

  int wait_delay = 3;
  int count_delay = 0;

  cerr << "Level " << level << ":";

  while (wait_delay > 0) {
    if (count_delay) {
      count_delay--;
    }
    else {
      count_delay = 60;
      cerr << "  " << wait_delay;

      al_set_target_bitmap(textbox);
      al_clear_to_color(al_map_rgb(0, 0, 0));
      al_draw_rounded_rectangle(0, 0, al_get_bitmap_width(textbox),
          al_get_bitmap_height(textbox), 10, 10, text_color, 4);


      x_loc = x_base;
      y_loc = TITLE_Y;
      al_draw_text(title_font, text_color, x_loc, y_loc,
          ALLEGRO_ALIGN_CENTRE, GAME_TITLE);

      y_loc += al_get_font_line_height(title_font);
      al_draw_textf(title_font, text_color, x_loc, y_loc,
          ALLEGRO_ALIGN_CENTRE, "Level %d", level);

      y_loc += al_get_font_line_height(title_font);
      al_draw_textf(title_font, text_color, x_loc, y_loc,
          ALLEGRO_ALIGN_CENTRE, "%d", wait_delay);

      al_set_target_backbuffer(display);
      al_clear_to_color(al_map_rgb(0, 0, 0));
      x_loc = (SCREEN_W - textbox_w) / 2;
      y_loc = 100;
      al_draw_bitmap(textbox, x_loc, y_loc, 0);

      al_flip_display();

      wait_delay--;

      al_rest(1.0);
    }
  }
  cerr << endl;
  if (al_is_event_queue_empty(event_queue) == false) {
    cerr << "pending event ... flushing" << endl;
    al_flush_event_queue(event_queue);
  }

};//end base_level::intro(int)

bool base_level::complete() {
  return _complete;
};//end base_level::complete()

void base_level::complete(bool b) {
  _complete = b;
};//end base_level::complete()

bool base_level::quit() {
  return _quit;
};//end base_level::quit()

void base_level::quit(bool b) {
  _quit = b;
};//end base_level::quit()

