#ifndef GALAGA_SHIP
  #define GALAGA_SHIP

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>
  #include <vector>

  #include "utilities.h"

  class Ship {
    Point *_position;
    std::vector<Bullet> _bullets;
    const int MAX_BULLETS = 10;

  public:
    Ship() { }
    Ship(int x, int y);
    ~Ship();

    bool move(int direction, int magnitude);
    bool moveTo(int x, int y);
    Point getPosition();

    bool fire();

    void update(unsigned int ticks);

    void render();
    void render(int x, int y);
  };
#endif
