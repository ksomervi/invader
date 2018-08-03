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

  pfont = NULL;
  textfont = NULL;


};//end base_level::base_level()

base_level::~base_level() {

};//end base_level::~base_level()

