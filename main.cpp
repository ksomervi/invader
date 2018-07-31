/*! \file main.cpp
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-07-30
 */

#include <cstdio>
#include <allegro5/allegro.h>
#include <iostream>
using std::cout;
using std::endl;

#include "game.h"

int main(int argc, char **argv) {
  game *g = new game();

  if (g->init()) {
    g->play();
    //g->print_score();
  }

  g->end();

  return 0;
}//end main()

