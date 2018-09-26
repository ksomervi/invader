/*! \file level_1.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-29
 */

#include <allegro5/allegro.h>
#include "level_1.h"
#include "enemy.h"

#include <random>
#include <iostream>
using std::cerr;
using std::endl;

#include <string>
using std::string;

level_1::level_1() {
  // Playable region
  float x_min = 4.0;
  float x_max = SCREEN_W - SPRITE_SIZE - x_min;
  float y_min = 0.25 * SCREEN_H;
  float y_max = SCREEN_H - SPRITE_SIZE;

  _min_bounds = point_2d(x_min, y_min);
  _max_bounds = point_2d(x_max, y_max);
}

level_1::~level_1() {
}

bool level_1::play(game *g) {
  env = g;
  display = env->get_display();
  title_font = env->get_font(0);
  textfont = env->get_font(1);
  hero = env->get_player();
  input = new controller();

  if (! this->init()) {
    return false;
  }

  while (hero->health() > 0 and complete() == false and quit() == false) {
    intro(1, 0.4);
    play_level();

    if (hero->health() == 0) {
      hero->add_lives(-1);
    }

    show_stats();
  }
  end_level();

  return complete();
}//end level_1::play()


bool level_1::init() {

  timer = al_create_timer(1.0/FPS);
  if(!timer) {
    cerr << "failed to create timer!" << endl;
    return false;
  }

  ALLEGRO_BITMAP *h_bm = env->get_sprite("hero");
  if (!h_bm) {
    if (!hero->create_bitmap(SPRITE_SIZE, SPRITE_SIZE)) {
    cerr << "failed to create hero bitmap!" << endl;
    return false;
    }

    //Setup and mockup sprite
    al_set_target_bitmap(hero->bitmap());
    al_clear_to_color(al_map_rgb(255, 174, 0));
    al_set_target_backbuffer(display);
  }
  else {
    hero->bitmap(h_bm);
  }

  if (!hero->ready_weapons(5)) {
    cerr << "failed to ready weapons!" << endl;
  }

  if (!init_foes(16)) {
    cerr << "failed to create foes!" << endl;
    return false;
  }

  hit_sound = env->get_sound("collision");
  if (!hit_sound) {
    cerr << "failed to load sound file" << endl;
  }

  deploy_sound = env->get_sound("mine");
  if (!deploy_sound) {
    cerr << "failed to load sound file" << endl;
  }

  event_queue = al_create_event_queue();
  if(!event_queue) {
    cerr << "failed to create event_queue!" << endl;
    return false;
  }

  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_register_event_source(event_queue, al_get_keyboard_event_source());

  al_clear_to_color(al_map_rgb(0,0,0));

  al_flip_display();

  return true;
}//end level_1::init()


void level_1::play_level() {

  float vel_x = 4.0;
  float vel_y = 4.0;
  point_2d gravity(0.0, 1.0);
  point_2d h_delta;

  hero->bound(_min_bounds, _max_bounds);

  int max_waves = 3;
  int current_wave = 0;
  //int max_foes[] = {32, 42, 60};
  int max_foes[] = {3, 3, 4};
  int foes_remaining = max_foes[current_wave];

  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(20,100);
  int next_foe = 120;

  std::default_random_engine x_generator;
  std::uniform_real_distribution<float> x_distribution(_min_bounds.x(), _max_bounds.x());

  bool playing = true;

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
          if (_foes->deploy(x_distribution(x_generator), 0.0)) {
            foes_remaining--;
          }
          next_foe = distribution(generator) * (1 - current_wave*0.2);
          cerr << "next foe in " << next_foe << " cycles" << endl;
        }
      }//end if (foes_remaining > 0)

      _foes->update();

      h_delta = gravity;

      if (input->fire()) {
        hero->fire_weapon();
      }

      if (input->direction() != point_2d()) {
        h_delta.x(input->direction().x() * vel_x);
        h_delta.y(input->direction().y() * vel_y);
      }
      hero->move(h_delta);
      hero->update();

      check_collisions();

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

    if (hero->health() == 0) {
      foes_remaining = 0;
    }

    if (foes_remaining == 0 and _foes->get_active().empty()) {
      playing = false;
      if (hero->health()) {
        cerr << "Completed wave: " << current_wave << endl;
        current_wave++;
        if (current_wave < max_waves) {
          foes_remaining = max_foes[current_wave];
          next_foe = 120;
          playing = true;
        }
        else {
          complete(true);
        }
        //Clear the mines
        //hero->clear_weapons();
      }
    }
  }//end while(playing)

  al_stop_timer(timer);
  al_rest(1.0);
  if (al_is_event_queue_empty(event_queue) == false) {
    cerr << "pending event ... flushing" << endl;
    al_flush_event_queue(event_queue);
  }

}//end level_1::play_level()


void level_1::show_stats() {
  int kill_eff = 0;
  int total_foes = _foes->deployed();
  if (total_foes) {
    kill_eff = hits*100/total_foes;
  }
  cerr << "So how did you do?" << endl;
  cerr << "  Total foes: " << total_foes << endl;
  cerr << "  Total hits: " << hits << " (eff: "
    << kill_eff << "%)" << endl;

  ALLEGRO_COLOR box_color = WHITE;
  ALLEGRO_COLOR text_color = WHITE;

  float textbox_h = al_get_font_line_height(title_font) +
    4 * al_get_font_line_height(textfont);
  float textbox_w = 0.8 * SCREEN_W;

  ALLEGRO_BITMAP *textbox = al_create_bitmap(textbox_w, textbox_h);
  al_set_target_bitmap(textbox);
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_rounded_rectangle(0, 0, al_get_bitmap_width(textbox),
      al_get_bitmap_height(textbox), 10, 10, box_color, 4);

  float x_loc = al_get_bitmap_width(textbox)/2;
  float y_loc = TITLE_Y;

  if (quit()) {
    al_draw_textf(title_font, text_color, x_loc, y_loc,
        ALLEGRO_ALIGN_CENTRE, "Level %d Quit", 1);

    y_loc += al_get_font_line_height(title_font);

    al_draw_textf(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_CENTRE, "Total Foes: %d", total_foes);

  }
  else if (complete()) {
    al_draw_textf(title_font, text_color, x_loc, y_loc,
        ALLEGRO_ALIGN_CENTRE, "Level %d Complete", 1);

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
  x_loc = (SCREEN_W - textbox_w) / 2;
  y_loc = 100;
  al_draw_bitmap(textbox, x_loc, y_loc, 0);
  x_loc = SCREEN_W / 2;
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
  cerr << "ending level..." << endl;
  if (hero) {
    //Cleanup the bitmap we allocated
    ALLEGRO_BITMAP *bm = hero->bitmap();
    al_destroy_bitmap(bm);
    hero->bitmap(NULL);
  }

  if (foe_bm) {
    al_destroy_bitmap(foe_bm);
    foe_bm = NULL;
  }
  for (auto &f: *_foes) {
    if (f) {
      f->bitmap(NULL);
      delete f;
      f = NULL;
    }
  }

  delete _foes;

  if (timer) {
    cerr << "   ... destroying timer" << endl;
    al_destroy_timer(timer);
  }

  if (event_queue) {
    cerr << "   ... destroying event_queue" << endl;
    al_destroy_event_queue(event_queue);
  }

  delete input;
  return;
}//end level_1::end_level()

bool level_1::init_foes(int max) {
  cerr << "Initializing foes..." << endl;

  point_2d init_vel(0.0, 1.0);

  _foes = new armada(); //entity_store();

  foe_bm = env->get_sprite("creeper");

  if (!foe_bm) {
    cerr << "failed to load bitmap for foe" << endl;
    foe_bm = al_create_bitmap(SPRITE_SIZE, SPRITE_SIZE);

    al_set_target_bitmap(foe_bm);
    al_clear_to_color(al_map_rgb(37, 196, 23));
    al_set_target_backbuffer(display);
  }

  enemy *f = NULL;
  for (int i=0; i<max; i++) {
    cerr << ".";
    f = new enemy();
    f->bitmap(foe_bm);
    f->velocity(init_vel);
    f->bound(point_2d(), _max_bounds);
    _foes->add(f);
  }
  cerr << endl;
  cerr << "Active foes: " << _foes->get_active().size() << endl;
  return true;
}//end level_1::init_foes()

void level_1::check_collisions() {
  //check for collisions
  int mine_hits = _foes->check_collisions(&(hero->get_deployed_mines()));

  if (mine_hits > 0) {
    hits += mine_hits;
    cerr << "BOOM" << endl;
    al_play_sample(hit_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE,
          NULL);
  }

  if (_foes->collides(hero)) {
    hits++;
    al_play_sample(hit_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    cerr << "hit: " << hits << endl;
    hero->take_hit(20);
  }
}//end level_1::check_collisions()


void level_1::redraw() {
  al_clear_to_color(al_map_rgb(0,0,0));

  _foes->redraw();

  hero->redraw();

  update_score();

  al_flip_display();
}//end level_1::redraw()


/*
void level_1::draw_text(const char *s, float x, float y, flag) {
  al_draw_text(textfont, al_map_rgb(255,255,255), x, y, flag, s);
}
*/


void level_1::update_score() {
  float x_base = al_get_display_width(display) - 220;
  float x_loc = x_base;
  float y_loc = TITLE_Y;

  ALLEGRO_COLOR text_color = WHITE;

  int kill_eff = 0;
  float y_step = al_get_font_line_height(textfont);

  int total_foes = _foes->deployed();
  if (total_foes > 0) {
    kill_eff = hits*100/total_foes;
  }

  al_draw_text(title_font, text_color,
      al_get_display_width(display)/2, TITLE_Y, ALLEGRO_ALIGN_RIGHT,
      GAME_TITLE);

  al_draw_text(textfont, al_map_rgb(255,255,255), x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "Lives:");

  x_loc += 100;
  al_draw_textf(textfont, al_map_rgb(255,255,255), x_loc, y_loc,
      ALLEGRO_ALIGN_RIGHT, "%d", hero->lives());

  x_loc = x_base;
  y_loc += y_step;
  al_draw_text(textfont, al_map_rgb(255,255,255), x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "Health:");

  x_loc += 100;
  float h_bar_len = 100;
  float h_bar_fill = h_bar_len * hero->percent_health();
  ALLEGRO_COLOR fill_color = GREEN;
  if (hero->percent_health() < 0.4) {
    fill_color = RED;
  }
  if (h_bar_fill > 0.0) {
    al_draw_filled_rounded_rectangle(x_loc, y_loc,
        x_loc+h_bar_fill, y_loc+y_step, 5, 5, fill_color);
  }

  al_draw_rounded_rectangle(x_loc, y_loc, x_loc+h_bar_len, y_loc+y_step,
      5, 5, al_map_rgb(255, 255, 255), 4);

  al_draw_textf(textfont, al_map_rgb(255,255,255), x_loc+h_bar_len/2.0, y_loc,
      ALLEGRO_ALIGN_CENTRE, "%d", hero->max_health());

  x_loc -= 100;
  x_loc = x_base;
  y_loc += y_step;
  al_draw_text(textfont, al_map_rgb(255,255,255), x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "Kills:");

  x_loc += 100;
  al_draw_textf(textfont, al_map_rgb(255,255,255), x_loc, y_loc,
      ALLEGRO_ALIGN_RIGHT, "%d", hits);

  x_loc += 14;
  al_draw_text(textfont, al_map_rgb(255,255,255), x_loc, y_loc,
      ALLEGRO_ALIGN_CENTRE, ":");

  x_loc += 14;
  al_draw_textf(textfont, al_map_rgb(255,255,255), x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "%d%%", kill_eff);

  x_loc = x_base;
  y_loc += y_step;
  al_draw_text(textfont, al_map_rgb(255,255,255), x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "Mines:");

  x_loc += 100;
  int ready = hero->get_deployed_mines().size();
  al_draw_textf(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_RIGHT, "%d", ready);

  x_loc += 14;
  al_draw_text(textfont, al_map_rgb(255,255,255), x_loc, y_loc,
      ALLEGRO_ALIGN_CENTRE, ":");

  x_loc += 14;
  al_draw_textf(textfont, al_map_rgb(255,255,255), x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "%d", hero->max_weapons());

}//end level_1::update_score()

