#ifndef GALAGA_BULLET
  #define GALAGA_BULLET

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>

  #include "utilities.h"

  class Bullet {
    Rectangle _container;
    int index;
    bool _alive;

  public:
    Bullet(int x, int y);
    ~Bullet();
    int move();
    Rectangle getContainer() { return _container; }
    bool isAlive() { return _alive; };
    void kill() { _alive = true; }
    void vivify() { _alive = false; }

    void update(unsigned int ticks);

    void render(/* camera *camera */);
    void render(int x, int y);
  };
#endif
