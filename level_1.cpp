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

level_1::level_1() {
  display = NULL;
  event_queue = NULL;
  timer = NULL;
  
  hero = NULL;

  hits = 0;
  total_foes = 0;
}

level_1::~level_1() {

}

bool level_1::play(ALLEGRO_DISPLAY *d) {
  display = d;

  if (! this->init()) {
    return false;
  }
  this->play_level();
  print_stats();
  end_level();

  return true;
}//end level_1::play()


bool level_1::init() {

  timer = al_create_timer(1.0/FPS);
  if(!timer) {
    cerr << "failed to create timer!" << endl;
    return false;
  }
 
  hero = new fighter();
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
    delete hero;
    al_destroy_timer(timer);
    return false;
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
 
  al_flip_display();

  return true;
}//end level_1::init()
 

void level_1::play_level() {
  //const float max_accel = 4;
  //const float max_vel = 8;
  float vel_x = 4.0;
  float vel_y = 4.0;
  float hx = 0.0;
  float hy = 0.0;

  float x_min = 4.0;
  float x_max = SCREEN_W - SPRITE_SIZE - x_min;
  float y_min = 4.0;
  float y_max = SCREEN_H - SPRITE_SIZE - y_min;

  //float fx = 0.0; // currently unused
  float fy = (SCREEN_W - SPRITE_SIZE) / 2;

  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(20,100);
  int next_foe = 0;

  std::default_random_engine x_generator;
  std::uniform_real_distribution<float> x_distribution(x_min, x_max);

  bool redraw_needed = true;
  bool playing = true;

  al_start_timer(timer);
 
  while(playing) {
    ALLEGRO_EVENT ev;
    al_wait_for_event(event_queue, &ev);
 
    if(ev.type == ALLEGRO_EVENT_TIMER) {
      if (next_foe > 0) {
        next_foe--;
      }
      else {
        if (activate_foe(foes, x_distribution(x_generator))) {
          total_foes++;
        }
        next_foe = distribution(generator);
        cout << "next foe in " << next_foe << " cycles" << endl;
      }
      hx = hero->x();
      hy = hero->y();

      for (auto &f: foes) {
        fy = f->y();
     
        if(fy <= y_max) {
          //foe->y(hy + 1.0);
          f->y(fy+1.0);
        }
        else {
          //Deactivate foe by setting off screen
          f->y(SCREEN_H);
        }
      }

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
            hero->y(hy-vel_y);//bouncer_y -= 4.0;
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
          hero->y(hy + 1.0);
        }
      }//end if(bouncer_y >= 4.0)
     
      if(key[KEY_DOWN] && hy <= y_max) {
        hero->y(hy+vel_y); //bouncer_y += 4.0;
      }

      if(key[KEY_LEFT] && hx >= x_min) {
        hero->x(hx-vel_x); //bouncer_x -= 4.0;
      }

      if(key[KEY_RIGHT] && hx <= x_max) {
        hero->x(hx+vel_x); //bouncer_x += 4.0;
      }

      redraw_needed = true;
    }//end if(ev.type == ALLEGRO_EVENT_TIMER)
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      break;
    }
     
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
      }
    }
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

        case ALLEGRO_KEY_ESCAPE:
          playing = false;
          break;
      }
    }
 
    if(redraw_needed && al_is_event_queue_empty(event_queue)) {
      redraw_needed = false;
      this->redraw(y_max);
    }
  }//end while(playing)
}//end level_1::play_level()

void level_1::print_stats() {
  float kill_eff = float(hits)*100.0/total_foes;
  cout << "So how did you do?" << endl;
  cout << "  Total foes: " << total_foes << endl;
  cout << "  Total hits: " << hits << " (eff: ";
  cout.precision(2);
  cout << std::fixed << kill_eff << "%)" << endl;
}//end level_1::print_stats()
 
void level_1::end_level() {
  if (hero) {
    delete hero;
  }

  for (auto &f: foes) {
    if (f) {
      delete f;
    }
  }

  if (timer) {
    al_destroy_timer(timer);
  }

  if (event_queue) {
    al_destroy_event_queue(event_queue);
  }

  return;
}//end level_1::end()

bool level_1::init_foes(armada &foes, int max) {
  foes.clear();
  foes.resize(max);
  cout << "initializing foes" << endl;

  int cnt = 0;
  
  for (auto& f: foes) {
    cnt++;
    f = new basic_object();
    if (!f->create_bitmap(SPRITE_SIZE, SPRITE_SIZE)) {
      cout << "failed to create foe" << endl;
      return false;
    }
    f->y(SCREEN_H); // default to off screen
    al_set_target_bitmap(f->bitmap());
    al_clear_to_color(al_map_rgb(37, 196, 23));
  }
  return true;
}//end level_1::init_foes()

bool level_1::activate_foe(armada &foes, float x) {
  for (auto &f: foes) {
    if (f->y() == SCREEN_H) {
      f->y(0);
      f->x(x);
      cout << "foe starting at " << x << endl;
      return true;
    }
  }

  cout << "all foes active" << endl;
  return false;
}

void level_1::redraw(float y_max) {
  al_clear_to_color(al_map_rgb(0,0,0));
  hero->redraw(); //al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);
  for (auto &f: foes) {
    if (f->y() <= y_max) {
      if (f->collides(hero)) {
        hits++;
        cout << "hit: " << hits << endl;
        f->y(SCREEN_H);
      } 
      else {
        f->redraw();
      }
    }
  }
  al_flip_display();
}//end level_1::redraw()


