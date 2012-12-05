#ifndef GALAGA_PARTICLE
  #define GALAGA_PARTICLE

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>
  #include <stdlib.h>

  #include "utilities.h"

  class Particle {
    Rectangle _container;
    int _xSpeed;
    int _ySpeed;
    int _r;
    int _g;
    int _b;

    bool _alive = true;

  public:
    Particle(int x, int y);
    ~Particle();

    bool update(unsigned int ticks);
    void render();

    bool isAlive() { return _alive; }

    void kill() { _alive = false; }
    void vivify() { _alive = true; }
  };
#endif
