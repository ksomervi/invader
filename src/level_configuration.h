/*! \file level_configuration.h
 * \brief
 * \author Kevin Somervill < kevin @ somervill dot org >
 * \date 2018-10-18
 */

#include <vector>
using std::vector;

enum Difficulty { EASY, NORMAL, HARD };

class level_configuration {
  private:
    int _id;
    vector<int> _waves;
    Difficulty _diff;

  public:
    level_configuration();
    level_configuration(const int&, const vector<int>&, const Difficulty& = NORMAL);
    int level();
    void level(const int &);
    vector<int>& enemy_waves();
    void enemy_waves(const vector<int> &);
    int enemy_wave(const size_t &);
    Difficulty difficulty();
    void difficulty(const Difficulty &);


};//end level_configuration


