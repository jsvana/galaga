#ifndef GALAGA_POWERUP
  #define GALAGA_POWERUP

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>
  #include <allegro5/allegro_image.h>

  #include "utilities.h"

  #define GALAGA_POWERUP_DOUBLE 0
  #define GALAGA_POWERUP_MORE_BULLETS 1

  class Powerup {
    Rectangle _container;
    int _type;
    int _frame = 0;
    ALLEGRO_BITMAP *_texture;
    Rectangle _bounds;

    bool _alive = true;
    bool _hit = false;

  public:
    Powerup(int x, int y, ALLEGRO_BITMAP *texture, int type, Rectangle bounds);

    bool isAlive() { return _alive; }
    bool didHit() { return _hit; }
    void hit() { _hit = true; }

    int getType() { return _type; }

    Rectangle getContainer() { return _container; }

    void kill() { _alive = false; }
    void vivify() { _alive = true; }

    bool update(unsigned int ticks);
    void render();
  };
#endif
