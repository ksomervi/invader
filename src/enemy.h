#ifndef ENEMY_H
#define ENEMY_H

#include "basic_object.h"
#include "enemy_controller.h"

class enemy : public basic_object {
  private:

  public:
    enemy();
    ~enemy();

    void update() override;

}; //end class enemy

#endif //!defined(ENEMY_H)

