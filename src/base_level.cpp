/*! \file base_level.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-30
 */

#include "base_level.h"

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

