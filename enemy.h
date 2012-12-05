#ifndef GALAGA_ENEMY
  #define GALAGA_ENEMY

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>
  #include <iostream>
  #include <list>

  #include "bullet.h"
  #include "utilities.h"

  class Enemy {
    Rectangle _container;
    bool _alive;

  public:
    Enemy();
    Enemy(int x, int y);
    ~Enemy();

    bool update(unsigned int ticks);
    void render();

    bool isAlive() { return _alive; }

    void kill() { _alive = false; }
    void vivify() { _alive = false; }

    bool hitTest(std::list<Bullet> *bullets);

    void moveTo(int x, int y);
  };
#endif
