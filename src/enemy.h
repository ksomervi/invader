#ifndef ENEMY_H
#define ENEMY_H

#include "base_object.h"
#include "enemy_controller.h"

class enemy : public base_object {
  private:

  public:
    enemy();
    enemy(const enemy&);
    ~enemy();

    base_object* clone() override;
    void update() override;

}; //end class enemy

#endif //!defined(ENEMY_H)

