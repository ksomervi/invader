#ifndef POINT_2D_H
#define POINT_2D_H

class point_2d {
  private:
    float _x;
    float _y;

  public:
    point_2d();
    point_2d(float, float);
    point_2d(const point_2d&);

    void x(float);
    float x(void) const;
    void y(float);
    float y(void) const;

    bool operator==(const point_2d&);
    bool operator!=(const point_2d&);

    point_2d& operator=(const point_2d&);
    point_2d& operator+=(const point_2d&);
    point_2d& operator-=(const point_2d&);

};//end class point_2d


point_2d operator+(point_2d, const point_2d&);
point_2d operator-(point_2d, const point_2d&);

#endif //!defined(POINT_2D_H)

