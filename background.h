#ifndef GALAGA_BACKGROUND
  #define GALAGA_BACKGROUND

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>
  #include <stdlib.h>
  #include <list>

  #include "particle.h"
  #include "utilities.h"

  class Background {
    int _screenWidth;
    int _screenHeight;
    int _ySpeed;
    std::list<Particle> _particles;

  public:
    Background(int screenWidth, int screenHeight, int ySpeed);
    ~Background();

    bool update(unsigned int ticks);
    void render();
  };
#endif
