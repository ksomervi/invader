#ifndef MINE_H
#define MINE_H

#include "base_object.h"

class mine: public base_object {
  private:
    int _age;

  public:
    mine();
    ~mine();

    void update() override;

    int age();
    void age(const int&);

};//end class mine
#endif //!defined(MINE_H)

