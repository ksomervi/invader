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
  _rm = nullptr;
  _log = nullptr;
  display = nullptr;
  event_queue = nullptr;
  timer = nullptr;

  _overlay = nullptr;

  hero = nullptr;
  _foes = nullptr;

  hits = 0;

  title_font = nullptr;
  textfont = nullptr;

  _complete = false;
  _quit = false;
};//end base_level::base_level()

base_level::~base_level() {

};//end base_level::~base_level()

void base_level::intro(int level, float delay) {
  int screen_w = al_get_display_width(display);
  float textbox_h = 4 * al_get_font_line_height(title_font);
  float textbox_w = 0.8 * screen_w;

  ALLEGRO_COLOR text_color = WHITE;
  ALLEGRO_BITMAP *textbox = al_create_bitmap(textbox_w, textbox_h);
  float x_base = al_get_bitmap_width(textbox)/2;
  float x_loc = x_base;
  float y_loc = al_get_font_line_height(title_font);

  int wait_delay = 3;

  cerr << "Level " << level << ":";

  while (wait_delay > 0) {
    cerr << "  " << wait_delay;

    al_set_target_bitmap(textbox);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_rounded_rectangle(0, 0, al_get_bitmap_width(textbox),
        al_get_bitmap_height(textbox), 10, 10, text_color, 4);

    x_loc = x_base;
    y_loc = al_get_font_line_height(title_font) / 2.0;
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
    x_loc = (screen_w - textbox_w) / 2;
    y_loc = 100;
    al_draw_bitmap(textbox, x_loc, y_loc, 0);

    al_flip_display();

    wait_delay--;

    al_rest(delay);
  }
  cerr << endl;
  if (al_is_event_queue_empty(event_queue) == false) {
    _log->debug("pending event ... flushing");
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

bool base_level::is_paused() {
  return al_get_timer_started(timer);
}//end base_level::is_paused()

void base_level::toggle_pause() {
  if (al_get_timer_started(timer)) {
    al_stop_timer(timer);
  }
  else {
    al_resume_timer(timer);
  }
}//end base_level::toggle_pause()

void base_level::pause_play() {
  if (al_get_timer_started(timer)) {
    al_stop_timer(timer);
  }
}//end base_level::pause_play()

void base_level::resume_play() {
  al_resume_timer(timer);
}//end base_level::resume_play()

