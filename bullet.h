#ifndef GALAGA_BULLET
  #define GALAGA_BULLET

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>
  #include <allegro5/allegro_image.h>

  #include "utilities.h"

  class Bullet {
    Rectangle _container;

    ALLEGRO_BITMAP *_texture;

    bool _alive;
    int _moveSpeed = 10;

  public:
    Bullet(int x, int y, ALLEGRO_BITMAP *texture);
    ~Bullet();
    int move();
    Rectangle getContainer() { return _container; }
    bool isAlive() { return _alive; };
    void kill() { _alive = false; }
    void vivify() { _alive = true; }

    void update(unsigned int ticks);

    void render(/* camera *camera */);
    void render(int x, int y);
  };
#endif
