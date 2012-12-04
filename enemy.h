#ifndef GALAGA_ENEMY
  #define GALAGA_ENEMY

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>
  #include <vector>

  #include "bullet.h"
  #include "utilities.h"

  class Enemy {
    Point _position;

  public:
    Enemy();
    Enemy(int x, int y);
    ~Enemy();

    bool update(unsigned int ticks);
    void render();

    bool hitTest(std::vector<Bullet> *bullets);

    void moveTo(int x, int y);
  };
#endif
