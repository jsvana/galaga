#ifndef GALAGA_BULLET
  #define GALAGA_BULLET

  #include "utilities.h"

  class Bullet {
    Point *position;

  public:
    Bullet(int x, int y);
    ~Bullet();
    int move();
    void render(/* camera *camera */);
  };
#endif
