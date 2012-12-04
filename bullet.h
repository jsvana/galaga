#ifndef GALAGA_BULLET
  #define GALAGA_BULLET

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>

  #include "utilities.h"

  class Bullet {
    Point _position;
    int index;

  public:
    Bullet(int x, int y, int index);
    ~Bullet();
    int move();
    Point getPosition() { return _position; }
    void update(unsigned int ticks);
    void render(/* camera *camera */);
    void render(int x, int y);
  };
#endif
