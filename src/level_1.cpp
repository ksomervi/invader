/*! \file level_1.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-29
 */

#include <allegro5/allegro.h>
#include "level_1.h"
//#include "enemy.h"
#include "enemy_controller.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <string>
using std::string;

level_1::level_1() {
}

level_1::~level_1() {
}

bool level_1::play(resource_manager *rm, level_configuration *lc) {
  _rm = rm;
  _cfg = lc;
  _log = _rm->get_logger();
  display = _rm->get_display();
  title_font = _rm->get_font(TITLE);
  textfont = _rm->get_font();
  hero = _rm->get_player();
  input = new game_controller();

  if (! this->init()) {
    return false;
  }

  while (hero->is_alive() and complete() == false and quit() == false) {
    intro(_cfg->level(), 0.4);
    play_level();

    if (hero->is_alive() == false) {
      hero->add_lives(-1);
    }

    show_stats();
  }
  end_level();

  return complete();
}//end level_1::play()


bool level_1::init() {
  complete(false);
  hits = 0;

  timer = al_create_timer(1.0/FPS);
  if(!timer) {
    _log->error("failed to create timer!");
    return false;
  }

  int screen_w = al_get_display_width(display);
  int screen_h = al_get_display_height(display);

  // Playable region
  float x_min = 4.0;
  float x_max = screen_w - SPRITE_SIZE - x_min;
  float y_min = 0.25 * screen_h;
  float y_max = screen_h - SPRITE_SIZE;

  _min_bounds = point_2d(x_min, y_min);
  _max_bounds = point_2d(x_max, y_max);

  hero->init(_rm);
  hero->bound(_min_bounds, _max_bounds);

  if (!init_foes(16)) {
    _log->error("failed to create foes!");
    return false;
  }

  // FIXME: Move sounds to associated object
  // DESCRIPTION: Collision (and other) sounds should be associated with the
  // objects
  hit_sound = _rm->get_sound("collision");
  if (!hit_sound) {
    _log->error("failed to load sound file");
  }

  // FIXME: Move background audio to game startup
  bg_sound = _rm->get_sound("bg_audio");
  if (bg_sound) {
    al_play_sample(bg_sound, 0.8, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
  }

  event_queue = al_create_event_queue();
  if(!event_queue) {
    _log->error("failed to create event_queue!");
    return false;
  }

  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_register_event_source(event_queue, al_get_keyboard_event_source());

  al_clear_to_color(al_map_rgb(0,0,0));

  al_flip_display();

  _overlay = new overlay(_rm);

  return true;
}//end level_1::init()


void level_1::play_level() {
  input->init();
  hero->controller()->init();

  size_t current_wave = 0;
  int foes_remaining = _cfg->enemy_wave(current_wave);
  random_generator *rg = _rm->get_random_generator();
  int delay_l = 20;
  int delay_h = 100;
  int next_foe = 120;

  float x_min = _min_bounds.x();
  float x_max = _max_bounds.x();

  bool playing = true;

  _log->debug(" - " + std::to_string(foes_remaining) + " foes in wave " 
      + std::to_string(current_wave));

  al_start_timer(timer);

  while(playing) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(event_queue, &ev);

    if(ev.type == ALLEGRO_EVENT_TIMER) {
      if (foes_remaining > 0) {
        if (next_foe > 0) {
          next_foe--;
        }
        else {
          if (_foes->deploy(rg->random_float(x_min, x_max), 0.0)) {
            foes_remaining--;
          }
          next_foe = rg->random_int(delay_l, delay_h) * (1.0 - current_wave*0.1);
          _log->debug("next foe in " + std::to_string(next_foe) + " cycles");
        }
      }//end if (foes_remaining > 0)

      _foes->update();

      if (hero->is_alive()) {
        hero->update();
        check_collisions();
      }

      redraw();
    }//end if(ev.type == ALLEGRO_EVENT_TIMER)
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      break;
    }//end else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    else if (input->handle_event(ev)) {
      if (input->quit()) {
        playing = false;
        quit(true);
      }
      else if (input->pause_event()) {
        if (al_get_timer_started(timer)) {
          pause_play();
        }
        else {
          resume_play();
        }
      }//end else if (input->pause_event())
    }
    else if (!hero->handle_event(ev)) {
      _log->error("unhandled event: " + std::to_string(ev.type));
    }//end else if (!hero->handle_event(ev))

    if (hero->is_alive() == false) {
      foes_remaining = 0;
    }

    if (foes_remaining == 0 and _foes->get_active().empty()) {
      playing = false;
      if (hero->is_alive()) {
        _log->debug("Completed wave: " + std::to_string(current_wave));
        current_wave++;
        if (current_wave < _cfg->enemy_waves().size()) {
          foes_remaining = _cfg->enemy_wave(current_wave);
          _log->debug(std::to_string(foes_remaining) + " foes in wave " 
              + std::to_string(current_wave));
          next_foe = 120;
          playing = true;
        }
        else {
          complete(true);
        }
      }
      //Clear the mines
      hero->clear_weapons();
    }//end if (foes_remaining == 0 and _foes->get_active().empty())
  }//end while(playing)

  al_stop_timer(timer);
  al_rest(1.0);
  if (al_is_event_queue_empty(event_queue) == false) {
    _log->debug("pending event ... flushing");
    al_flush_event_queue(event_queue);
  }

}//end level_1::play_level()


void level_1::show_stats() {
  int screen_w = al_get_display_width(display);

  int kill_eff = 0;
  int total_foes = _foes->deployed();
  if (total_foes) {
    kill_eff = hits*100/total_foes;
  }
  _log->note("So how did you do?");
  _log->note("  Total foes: " + std::to_string(total_foes));
  _log->note("  Total hits: " + std::to_string(hits) + " (eff: "
    + std::to_string(kill_eff) + "%)");

  ALLEGRO_COLOR box_color = WHITE;
  ALLEGRO_COLOR text_color = WHITE;

  float textbox_h = al_get_font_line_height(title_font) +
    4 * al_get_font_line_height(textfont);
  float textbox_w = 0.8 * screen_w;

  ALLEGRO_BITMAP *textbox = al_create_bitmap(textbox_w, textbox_h);
  al_set_target_bitmap(textbox);
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_rounded_rectangle(0, 0, al_get_bitmap_width(textbox),
      al_get_bitmap_height(textbox), 10, 10, box_color, 4);

  float x_loc = al_get_bitmap_width(textbox)/2;
  float y_loc = TITLE_Y;

  if (quit()) {
    al_draw_textf(title_font, text_color, x_loc, y_loc,
        ALLEGRO_ALIGN_CENTRE, "Level %d Quit", _cfg->level());

    y_loc += al_get_font_line_height(title_font);

    al_draw_textf(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_CENTRE, "Total Foes: %d", total_foes);

  }
  else if (complete()) {
    al_draw_textf(title_font, text_color, x_loc, y_loc,
        ALLEGRO_ALIGN_CENTRE, "Level %d Complete", _cfg->level());

    y_loc += al_get_font_line_height(title_font);

    al_draw_textf(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_CENTRE, "Total Foes: %d", total_foes);

  }
  else {

    al_draw_text(title_font, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_CENTRE, "Player died!");

    y_loc += al_get_font_line_height(title_font);

    al_draw_textf(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_CENTRE, "Lives remaining: %d", hero->lives());

  }

  y_loc += al_get_font_line_height(textfont);

  al_draw_textf(textfont, text_color, x_loc, y_loc,
    ALLEGRO_ALIGN_CENTRE, "Hits: %d", hits);

  y_loc += al_get_font_line_height(textfont);

  al_draw_textf(textfont, text_color, x_loc, y_loc,
    ALLEGRO_ALIGN_CENTRE, "Efficiency: %d%%", kill_eff);

  al_set_target_backbuffer(display);
  al_clear_to_color(al_map_rgb(0, 0, 0));
  x_loc = (screen_w - textbox_w) / 2;
  y_loc = 100;
  al_draw_bitmap(textbox, x_loc, y_loc, 0);
  x_loc = screen_w / 2;
  y_loc += textbox_h;
  al_draw_text(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_CENTRE, "Any key to continue...");

  al_flip_display();

  bool waiting = true;
  int countdown = 300;
  al_start_timer(timer);
  while(waiting) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(event_queue, &ev);
    switch(ev.type) {
      case ALLEGRO_EVENT_TIMER:
        countdown--;
        break;

      case ALLEGRO_EVENT_DISPLAY_CLOSE:
      case ALLEGRO_EVENT_KEY_DOWN:
        waiting = false;
        break;
    }

    if (countdown == 0) {
      waiting = false;
    }
  }//end while(waiting)
}//end level_1::show_stats()

void level_1::end_level() {
  _log->debug("ending level...");

  al_stop_samples();

  delete _overlay;
  _overlay = nullptr;

  if (hero) {
    //Cleanup the bitmap we allocated
    ALLEGRO_BITMAP *bm = hero->bitmap();
    al_destroy_bitmap(bm);
    hero->bitmap(NULL);
  }

  if (foe_bm) {
    al_destroy_bitmap(foe_bm);
    foe_bm = nullptr;
  }
  for (auto &f: *_foes) {
    if (f) {
      f->bitmap(NULL);
      delete f;
      f = nullptr;
    }
  }

  delete _foes;

  if (timer) {
    _log->debug("   ... destroying timer");
    al_destroy_timer(timer);
  }

  if (event_queue) {
    _log->debug("   ... destroying event_queue");
    al_destroy_event_queue(event_queue);
  }

  delete input;

  return;
}//end level_1::end_level()

bool level_1::init_foes(int max) {
  _log->debug("Initializing foes...");
  cerr.flush();

  point_2d init_vel(0.0, 1.0);

  _foes = new armada(); //entity_store();
  _foes->set_logger(_log);

  foe_bm = _rm->get_sprite("creeper");

  if (!foe_bm) {
    _log->error("failed to load bitmap for foe");
    foe_bm = al_create_bitmap(SPRITE_SIZE, SPRITE_SIZE);

    al_set_target_bitmap(foe_bm);
    al_clear_to_color(al_map_rgb(37, 196, 23));
    al_set_target_backbuffer(display);
  }

  entity *f = nullptr;
  for (int i=0; i<max; i++) {
    f = new entity();
    f->controller(new enemy_controller());
    f->bitmap(foe_bm);
    f->velocity(init_vel);
    f->bound(point_2d(), _max_bounds);
    _foes->add(f);
  }
  _log->debug("Active foes: " + std::to_string(_foes->get_active().size()));
  return true;
}//end level_1::init_foes()

void level_1::check_collisions() {
  //check for collisions
  int mine_hits = _foes->check_collisions(&(hero->get_deployed_mines()));
  mine_hits += _foes->check_collisions(&(hero->get_deployed_blasts()));

  if (mine_hits > 0) {
    hits += mine_hits;
    al_play_sample(hit_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE,
          NULL);
  }

  if (_foes->collides(hero)) {
    hits++;
    al_play_sample(hit_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    _log->debug("hit: " + std::to_string(hits));
    hero->take_hit(20);
  }
}//end level_1::check_collisions()


void level_1::redraw() {
  al_clear_to_color(BLACK);

  _foes->redraw();

  hero->redraw();

  //update_score();
  _overlay->redraw(_foes->deployed(), hits, hero);

  al_flip_display();
}//end level_1::redraw()

/*
void level_1::update_score() {
  float x_base = al_get_display_width(display) - 220;
  float x_loc = x_base;
  float y_loc = TITLE_Y;

  ALLEGRO_COLOR text_color = al_premul_rgba(255, 255, 255, 107);

  int kill_eff = 0;
  float y_step = al_get_font_line_height(textfont);

  int total_foes = _foes->deployed();
  if (total_foes > 0) {
    kill_eff = hits*100/total_foes;
  }

  al_draw_text(title_font, text_color,
      al_get_display_width(display)/2, TITLE_Y, ALLEGRO_ALIGN_RIGHT,
      GAME_TITLE);

  al_draw_text(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "Lives:");

  x_loc += 100;
  al_draw_textf(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_RIGHT, "%d", hero->lives());

  x_loc = x_base;
  y_loc += y_step;
  al_draw_text(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "Health:");

  x_loc += 100;
  float h_bar_len = 100;
  float h_bar_fill = h_bar_len * hero->percent_health();
  ALLEGRO_COLOR fill_color = al_premul_rgba(153, 204, 0, 107);
  if (hero->percent_health() < 0.4) {
    fill_color = RED;
  }
  if (h_bar_fill > 0.0) {
    al_draw_filled_rounded_rectangle(x_loc, y_loc,
        x_loc+h_bar_fill, y_loc+y_step, 5, 5, fill_color);
  }

  al_draw_rounded_rectangle(x_loc, y_loc, x_loc+h_bar_len, y_loc+y_step,
      5, 5, text_color, 4);

  al_draw_textf(textfont, text_color, x_loc+h_bar_len/2.0, y_loc,
      ALLEGRO_ALIGN_CENTRE, "%d", hero->max_health());

  x_loc -= 100;
  x_loc = x_base;
  y_loc += y_step;
  al_draw_text(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "Kills:");

  x_loc += 100;
  al_draw_textf(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_RIGHT, "%d", hits);

  x_loc += 14;
  al_draw_text(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_CENTRE, ":");

  x_loc += 14;
  al_draw_textf(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "%d%%", kill_eff);

  x_loc = x_base;
  y_loc += y_step;
  al_draw_text(textfont, text_color, x_loc, y_loc, ALLEGRO_ALIGN_LEFT, "Mines:");

  x_loc += 100;
  int ready = hero->get_deployed_mines().size();
  al_draw_textf(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_RIGHT, "%d", ready);

  x_loc += 14;
  al_draw_text(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_CENTRE, ":");

  x_loc += 14;
  al_draw_textf(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "%d", hero->max_weapons());

}//end level_1::update_score()
*/

