/* \file sandbox.cpp
 *
 * Scrap program to test features and objects.
 *
 * Completely ripped from the keyboard example at:
 *   https://wiki.allegro.cc/index.php?title=Basic_Keyboard_Example
 */
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <random>
#include <iostream>
using std::cout;
using std::endl;

#include "basic_object.h"
using armada = std::vector<basic_object*>;

#include "fighter.h"

bool init_foes(armada &, int max);
bool activate_foe(armada&, float);
 
//const float FPS = 60.0;
//const int SCREEN_W = 640;
//const int SCREEN_H = 480;
enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

int main(int argc, char **argv) {
  ALLEGRO_DISPLAY *display = NULL;
  ALLEGRO_EVENT_QUEUE *event_queue = NULL;
  ALLEGRO_TIMER *timer = NULL;
  fighter *hero = NULL;
  armada foes;

  int hits = 0;
  int total_foes = 0;

  bool key[4] = { false, false, false, false };
  bool redraw = true;
  bool playing = true;

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

  //float fx = 0.0;
  float fy = (SCREEN_W - SPRITE_SIZE) / 2;

  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(20,100);
  int next_foe = 0;

  std::default_random_engine x_generator;
  std::uniform_real_distribution<float> x_distribution(4.0, x_max);

  //float accel_x = 0.0;
  //float accel_y = 0.0;

  if(!al_init()) {
    fprintf(stderr, "failed to initialize allegro!\n");
    return -1;
  }
  
  if(!al_install_keyboard()) {
    fprintf(stderr, "failed to initialize the keyboard!\n");
    return -1;
  }

  timer = al_create_timer(1.0/FPS);
  if(!timer) {
    fprintf(stderr, "failed to create timer!\n");
    return -1;
  }
 
  display = al_create_display(SCREEN_W, SCREEN_H);
  if(!display) {
    fprintf(stderr, "failed to create display!\n");
    al_destroy_timer(timer);
    return -1;
  }

  al_init_primitives_addon();
 
  hero = new fighter();
  if (!hero->create_bitmap(SPRITE_SIZE, SPRITE_SIZE)) {
  //if (!hero->create_bitmap("sprites/hero.png")) {
    fprintf(stderr, "failed to create hero bitmap!\n");
    al_destroy_display(display);
    al_destroy_timer(timer);
    return -1;
  }

  al_set_target_bitmap(hero->bitmap());
  al_clear_to_color(al_map_rgb(255, 174, 0));
  
  if (!init_foes(foes, 16)) {
    fprintf(stderr, "failed to create foes!\n");
    delete hero;
    al_destroy_display(display);
    al_destroy_timer(timer);
    return -1;
  }

  //al_set_target_bitmap(foe->bitmap());
  //al_clear_to_color(al_map_rgb(255, 127, 255));
 
  al_set_target_bitmap(al_get_backbuffer(display));
 
  event_queue = al_create_event_queue();
  if(!event_queue) {
    fprintf(stderr, "failed to create event_queue!\n");
    //al_destroy_bitmap(bouncer);
    delete hero;
    al_destroy_display(display);
    al_destroy_timer(timer);
    return -1;
  }
 
  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_register_event_source(event_queue, al_get_keyboard_event_source());

  al_clear_to_color(al_map_rgb(0,0,0));
 
  al_flip_display();
 
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
          //Deactivate foe
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

      redraw = true;
    }//end if(ev.type == ALLEGRO_EVENT_TIMER)
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      break;
    }
     
    else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
      switch(ev.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
        case ALLEGRO_KEY_W:
          key[KEY_UP] = true;
          break;

        case ALLEGRO_KEY_DOWN:
        case ALLEGRO_KEY_S:
          key[KEY_DOWN] = true;
          break;
            
        case ALLEGRO_KEY_LEFT: 
	case ALLEGRO_KEY_A:
          key[KEY_LEFT] = true;
          break;
            
        case ALLEGRO_KEY_RIGHT:
	case ALLEGRO_KEY_D:
          key[KEY_RIGHT] = true;
          break;
      }
    }
    else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
      switch(ev.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
	case ALLEGRO_KEY_W:
          key[KEY_UP] = false;
          break;

        case ALLEGRO_KEY_DOWN:
	case ALLEGRO_KEY_S:
          key[KEY_DOWN] = false;
          break;

        case ALLEGRO_KEY_LEFT: 
	case ALLEGRO_KEY_A:
          key[KEY_LEFT] = false;
          break;

        case ALLEGRO_KEY_RIGHT:
	case ALLEGRO_KEY_D:
          key[KEY_RIGHT] = false;
          break;

        case ALLEGRO_KEY_F1:
          cout << "Help" << endl;
          break;

        case ALLEGRO_KEY_P:
          if (al_get_timer_started(timer)) {
            al_stop_timer(timer);
          }
          else {
            al_resume_timer(timer);
          }
          continue;
          break;

        case ALLEGRO_KEY_ESCAPE:
          playing = false;
          break;
      }
    }
 
    if(redraw && al_is_event_queue_empty(event_queue)) {
      redraw = false;
 
      al_clear_to_color(al_map_rgb(0,0,0));
      ALLEGRO_COLOR line_color = LIGHT_YELLOW;

      float hx = hero->x() + hero->w()/2;
      float hy = hero->y() + hero->h()/2;

      for (auto &f: foes) {
        if (f->y() <= y_max) {
          if (f->collides(hero)) {
            hits++;
            cout << "hit: " << hits << endl;
            f->y(SCREEN_H);
          } 
          else {
            al_draw_line(hx, hy, f->x()+f->w()/2, f->y()+f->h()/2, line_color, 2.0);

            f->redraw();
            //foe->draw(fx, fy);
          }
        }
      }
      hero->redraw(); //al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);
      al_flip_display();
    }
  }
 
  //al_destroy_bitmap(hero->bitmap);
  delete hero;
  al_destroy_timer(timer);
  al_destroy_display(display);
  al_destroy_event_queue(event_queue);

  float kill_eff = float(hits)*100.0/total_foes;
  cout << "Total foes: " << total_foes << endl;
  cout << "Total hits: " << hits << " (eff: ";
  cout.precision(2);
  cout << std::fixed << kill_eff << "%)" << endl;

  return 0;
}

bool init_foes(armada &foes, int max) {
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
}

bool activate_foe(armada &foes, float x) {
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

void draw_foes(armada &foes) {

}
