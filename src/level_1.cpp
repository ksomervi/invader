/*! \file level_1.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-29
 */

#include <allegro5/allegro.h>
#include "level_1.h"

#include <random>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <sstream>
using std::ostringstream;

level_1::level_1() {
  _healing_time = 0;
}

level_1::~level_1() {
}

bool level_1::play(game *g) {
  env = g;
  display = env->get_display();
  title_font = env->get_font(0);
  textfont = env->get_font(1);
  hero = env->get_player();

  if (! this->init()) {
    return false;
  }

  while (hero->health() > 0 and complete() == false and quit() == false) {
    intro(1);
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

  //TODO: we should get the hero from the game environment
  //hero = new fighter();
  if (!hero->create_bitmap(SPRITE_SIZE, SPRITE_SIZE)) {
  //if (!hero->create_bitmap("sprites/hero.png"))
    cerr << "failed to create hero bitmap!" << endl;
    al_destroy_timer(timer);
    return false;
  }

  al_set_target_bitmap(hero->bitmap());
  al_clear_to_color(al_map_rgb(255, 174, 0));

  if (!init_foes(foes, 16)) {
    cerr << "failed to create foes!" << endl;
    al_destroy_timer(timer);
    return false;
  }
  if (!init_weapons(mines, 5)) {
    cerr << "failed to create foes!" << endl;
  }

  hit_sound = env->get_sound("collision"); //("resources/sound/hit1.ogg" );
  if (!hit_sound) {
    cerr << "failed to load sound file" << endl;
  }

  deploy_sound = env->get_sound("mine"); //("resources/sound/hit1.ogg" );
  if (!deploy_sound) {
    cerr << "failed to load sound file" << endl;
  }

  al_set_target_bitmap(al_get_backbuffer(display));

  event_queue = al_create_event_queue();
  if(!event_queue) {
    cerr << "failed to create event_queue!" << endl;
    delete hero;
    al_destroy_timer(timer);
    return false;
  }

  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_register_event_source(event_queue, al_get_keyboard_event_source());

  al_clear_to_color(al_map_rgb(0,0,0));

#if 0
  al_draw_text(title_font, al_map_rgb(255,255,255),
      al_get_display_width(display)/2, TITLE_FONTSIZE, ALLEGRO_ALIGN_CENTRE,
      GAME_TITLE);
#endif

  al_flip_display();

  return true;
}//end level_1::init()


void level_1::play_level() {
  for (auto &k: key) {
    k = false;
  }

  //const float max_accel = 4;
  //const float max_vel = 8;
  float vel_x = 4.0;
  float vel_y = 4.0;
  point_2d gravity(0.0, 1.0);
  float hx = 0.0;
  float hy = 0.0;
  point_2d h_loc;
  point_2d h_delta;

  float x_min = 4.0;
  float x_max = SCREEN_W - SPRITE_SIZE - x_min;
  float y_min = 0.25 * SCREEN_H;
  float y_max = SCREEN_H - SPRITE_SIZE;

  int max_waves = 3;
  int current_wave = 0;
  //int max_foes[] = {32, 42, 60};
  int max_foes[] = {3, 3, 4};
  int foes_remaining = max_foes[current_wave];
  int active_foes = 0;
  int fire_delay = 0;

  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(20,100);
  int next_foe = 120;

  std::default_random_engine x_generator;
  std::uniform_real_distribution<float> x_distribution(x_min, x_max);

  bool redraw_needed = true;
  bool playing = true;

  al_start_timer(timer);

  while(playing) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(event_queue, &ev);

    if(ev.type == ALLEGRO_EVENT_TIMER) {
      if (fire_delay > 0) {
        fire_delay--;
      }
      if (_healing_time > 0) {
        _healing_time--;
      }
      else {
        _healing_time = 60;
        hero->add_health(2);
      }
      if (foes_remaining > 0) {
        if (next_foe > 0) {
          next_foe--;
        }
        else {
          if (activate_foe(foes, x_distribution(x_generator))) {
            total_foes++;
            foes_remaining--;
          }
          next_foe = distribution(generator) * (1 - current_wave*0.2);
          cout << "next foe in " << next_foe << " cycles" << endl;
        }
      }//end if (foes_remaining > 0)

      active_foes = 0;
      for (auto &f: foes) {
        if (f->active()) {

          if(f->y() <= y_max) {
            active_foes++;
          }
          else {
            //Deactivate foe by setting off screen
            f->active(false);
          }
          f->move(gravity);
        }//end if (f->active())
      }//end for (auto &f: foes)

      hx = hero->x();
      hy = hero->y();

      h_loc = hero->location();
      h_delta = point_2d(0.0, 0.0);

      if(key[KEY_UP]) {
        if(hy >= y_min) {
            //bouncer_y -= 4.0;
            //if (accel_y < max_accel) {
              //accel_y += 0.2;
              //printf("accel_y: %f\n", accel_y);
            //}

            //if (vel_y < max_vel) {
              //vel_y += accel_y;
              //printf("vel_y: %f\n", vel_y);
            //}
            //hero->y(hy-vel_y);//bouncer_y -= 4.0;
            h_delta.y(-vel_y);
        }
      }
      else {//key[KEY_UP] == false
            //if (accel_y > -(max_accel)) {
              //accel_y -= 0.02;
              //printf("accel_y: %f\n", accel_y);
            //}

            //if (vel_y > -8) {
              //vel_y += accel_y;
              //printf("vel_y: %f\n", vel_y);
            //}
        if(hy <= y_max) {
          h_delta.y(1.0);
        }
      }//end if(bouncer_y >= 4.0)

      if(key[KEY_DOWN] && hy <= y_max) {
        h_delta.y(vel_y);
      }

      if(key[KEY_LEFT] && hx >= x_min) {
        h_delta -= point_2d(vel_x, 0.0);
      }

      if(key[KEY_RIGHT] && hx <= x_max) {
        h_delta += point_2d(vel_x, 0.0);
      }

      if (key[KEY_SPACE]) {
        if (fire_delay == 0) {
          deploy_mine(mines, hx, hy);
          fire_delay = 30;
        }
      }

      hero->move(h_delta);
      redraw_needed = true;
    }//end if(ev.type == ALLEGRO_EVENT_TIMER)
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      break;
    }//end else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      switch(ev.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
          key[KEY_UP] = true;
          break;

        case ALLEGRO_KEY_DOWN:
          key[KEY_DOWN] = true;
          break;

        case ALLEGRO_KEY_LEFT:
          key[KEY_LEFT] = true;
          break;

        case ALLEGRO_KEY_RIGHT:
          key[KEY_RIGHT] = true;
          break;

        case ALLEGRO_KEY_SPACE:
          key[KEY_SPACE] = true;
          break;
      }
    }//end else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
    else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
      switch(ev.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
          key[KEY_UP] = false;
          break;

        case ALLEGRO_KEY_DOWN:
          key[KEY_DOWN] = false;
          break;

        case ALLEGRO_KEY_LEFT:
          key[KEY_LEFT] = false;
          break;

        case ALLEGRO_KEY_RIGHT:
          key[KEY_RIGHT] = false;
          break;

        case ALLEGRO_KEY_F1:
          cout << "Help" << endl;
          break;

        case ALLEGRO_KEY_SPACE:
          key[KEY_SPACE] = false;
          break;

        case ALLEGRO_KEY_ESCAPE:
          playing = false;
          quit(true);
          break;
      }
    }//end else if(ev.type == ALLEGRO_EVENT_KEY_UP)

    if(redraw_needed && al_is_event_queue_empty(event_queue)) {
      redraw_needed = false;
      this->redraw(y_max);
    }

    active_foes = 0;
    for (auto &f: foes) {
      if (f->active()) {
        active_foes++;
      }
    }

    if (hero->health() == 0) {
      foes_remaining = 0;
    }

    if (foes_remaining == 0 and active_foes == 0) {
      playing = false;
      if (hero->health()) {
        cout << "Completed wave: " << current_wave << endl;
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
        for (auto &m: mines) {
          if (m->active()) {
            m->active(false);
          }
        }
      }
    }
  }//end while(playing)

  al_stop_timer(timer);
  al_rest(1.0);
  if (al_is_event_queue_empty(event_queue) == false) {
    cout << "pending event ... flushing" << endl;
    al_flush_event_queue(event_queue);
  }

}//end level_1::play_level()


void level_1::show_stats() {
  int kill_eff = 0;
  if (total_foes) {
    kill_eff = hits*100/total_foes;
  }
  cout << "So how did you do?" << endl;
  cout << "  Total foes: " << total_foes << endl;
  cout << "  Total hits: " << hits << " (eff: "
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
    if(ev.type == ALLEGRO_EVENT_TIMER) {
      countdown--;
    }
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      waiting = false;
    }//end else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      waiting = false;
    }

    if (countdown == 0) {
      waiting = false;
    }
  }
}//end level_1::show_stats()

void level_1::end_level() {
  cout << "ending level..." << endl;
  if (hero) {
    //delete hero;
  }

  if (foe_bm) {
    al_destroy_bitmap(foe_bm);
    foe_bm = NULL;
  }
  for (auto &f: foes) {
    if (f) {
      f->bitmap(NULL);
      delete f;
    }
  }

  if (mine_bm) {
    al_destroy_bitmap(mine_bm);
    mine_bm = NULL;
  }
  for (auto &m: mines) {
    if (m) {
      m->bitmap(NULL);
      delete m;
    }
  }

  if (timer) {
    cout << "   ... destroying timer" << endl;
    al_destroy_timer(timer);
  }

  if (event_queue) {
    cout << "   ... destroying event_queue" << endl;
    al_destroy_event_queue(event_queue);
  }

  return;
}//end level_1::end()

bool level_1::init_foes(armada &foes, int max) {
  foes.clear();
  foes.resize(max);
  cout << "Initializing foes..." << endl;

  int cnt = 0;

  foe_bm = al_load_bitmap(env->option("SPRITES", "creeper"));
  cout << "Loaded foe bitmap: " << env->option("SPRITES", "creeper") << endl;

  if (!foe_bm) {
    cout << "failed to load bitmap for foe" << endl;
    foe_bm = al_create_bitmap(SPRITE_SIZE, SPRITE_SIZE);

    al_set_target_bitmap(foe_bm);
    al_clear_to_color(al_map_rgb(37, 196, 23));
    al_set_target_backbuffer(display);
  }

  for (auto& f: foes) {
    cout << ".";
    cnt++;
    f = new basic_object();
    f->bitmap(foe_bm);
    f->y(SCREEN_H); // default to off screen
  }
  cout << endl;
  return true;
}//end level_1::init_foes()

bool level_1::init_weapons(weapons& mines, int max) {
  mines.clear();
  mines.resize(max);

  cout << "Initializing weapons..." << endl;

  mine_bm = al_create_bitmap(SPRITE_SIZE, SPRITE_SIZE);

  if (!mine_bm) {
    cout << "failed to create bitmap for mine" << endl;
    return false;
  }

  al_set_target_bitmap(mine_bm);
  al_clear_to_color(LIGHT_YELLOW);

  float line_width = 8.2;
  al_draw_circle(SPRITE_SIZE/2, SPRITE_SIZE/2, (SPRITE_SIZE - line_width)/2, RED, line_width);
  al_draw_filled_triangle(5, 5, SPRITE_SIZE-5, 5, SPRITE_SIZE/2, SPRITE_SIZE-5, BLUE);
  al_draw_filled_triangle(5, SPRITE_SIZE-5, SPRITE_SIZE-5, SPRITE_SIZE-5, SPRITE_SIZE/2, 5, BLUE);

  al_convert_mask_to_alpha(mine_bm, LIGHT_YELLOW);

  int cnt = 0;

  for (auto& m: mines) {
    cnt++;
    m = new basic_object();
    m->bitmap(mine_bm);
    m->y(SCREEN_H); // default to off screen
    m->active(false);
  }

  al_set_target_backbuffer(display);

  return true;
}//end level_1::init_weapons()


bool level_1::activate_foe(armada &foes, float x) {
  for (auto &f: foes) {
    if (f->active() == false) {
      f->y(0);
      f->x(x);
      cout << "foe starting at " << x << endl;
      f->active(true);
      return true;
    }
  }

  cout << "all foes active" << endl;
  return false;
}

bool level_1::deploy_mine(weapons& mines, int x, int y) {

  cout << "deploy mine..." << endl;

  for (auto &m: mines) {
    if (m->active() == false) {
      m->x(x);
      m->y(y);
      cout << "mine deployed starting at " << x << ", " << y << endl;
      m->active(true);
      al_play_sample(deploy_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE,
                     NULL);
      return true;
    }
  }
  cout << "out of mines" << endl;
  return false;
}

void level_1::redraw(float y_max) {
  al_clear_to_color(al_map_rgb(0,0,0));

  for (auto &f: foes) {
    if (f->active()) {
      //Check mines
      for (auto &m: mines) {
        if (m->active()) {
          if (f->collides(m)) {
            cout << "BOOM" << endl;
            al_play_sample(hit_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE,
                NULL);
            hits++;
            m->active(false);
            f->active(false);
          }
        }
      }

      if (f->collides(hero)) {
        hits++;
        al_play_sample(hit_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        cout << "hit: " << hits << endl;
        f->y(SCREEN_H);
        f->active(false);
        hero->add_health(-20);
        if (hero->health() > 0) {
          _healing_time = 60;
        }
      }
      else {
        f->redraw();
      }
    }
  }

  for (auto &m: mines) {
    if (m->active()) {
      m->redraw();
    }
  }//end for (auto &m: mines)

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
  //float line_height = SECONDARY_FONTSIZE + 4;
  float y_loc = TITLE_Y;// + line_height;

  ALLEGRO_COLOR text_color = WHITE;

  int kill_eff = 0;
  float y_step = al_get_font_line_height(textfont);

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

  //x_loc -= 100;
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
    al_draw_filled_rounded_rectangle(x_loc, y_loc, x_loc+h_bar_fill, y_loc+y_step,
      5, 5, fill_color);
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
  int ready = 0;
  for (auto& m: mines) {
    if (m->active() == false) {
      ready++;
    }
  }
  al_draw_textf(textfont, text_color, x_loc, y_loc,
      ALLEGRO_ALIGN_RIGHT, "%d", ready);

  x_loc += 14;
  al_draw_text(textfont, al_map_rgb(255,255,255), x_loc, y_loc,
      ALLEGRO_ALIGN_CENTRE, ":");

  x_loc += 14;
  al_draw_textf(textfont, al_map_rgb(255,255,255), x_loc, y_loc,
      ALLEGRO_ALIGN_LEFT, "%d", mines.size());


}//end level_1::update_score()

