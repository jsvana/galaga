#ifndef GALAGA_SHIP
  #define GALAGA_SHIP

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>
  #include <allegro5/allegro_image.h>
  #include <list>

  #include "powerup.h"
  #include "utilities.h"

  class Ship {
    ALLEGRO_BITMAP *_texture;

    Rectangle _container;

    std::list<ActivePowerup> _activePowerups;

  public:
    Ship();
    Ship(int x, int y);
    ~Ship();

    bool move(int direction, int magnitude);
    bool moveTo(int x, int y);
    Rectangle getContainer() { return _container; }

    void addPowerup(int type, int duration);

    bool fire();

    bool hitTest(std::list<Powerup> *powerups);

    std::list<ActivePowerup> getActivePowerups();
    void clearActivePowerups() { _activePowerups.clear(); }

    void update(unsigned int ticks);

    void render();
    void render(int x, int y);
  };
#endif
