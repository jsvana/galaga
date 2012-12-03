#ifndef GALAGA_SHIP
  #define GALAGA_SHIP

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>

  #include "utilities.h"

  class Ship {
    Point *_position;

  public:
    Ship(int x, int y);
    ~Ship();

    bool move(int direction, int magnitude);
    bool moveTo(int x, int y);
    Point getPosition();

    void render();
    void render(int x, int y);
  };
#endif
