#ifndef GALAGA_SHIP
  #define GALAGA_SHIP

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>
  #include <allegro5/allegro_image.h>
  #include <list>

  #include "bullet.h"
  #include "powerup.h"
  #include "utilities.h"

  class Ship {
    ALLEGRO_BITMAP *_texture;

    Rectangle _container;

    std::list<ActivePowerup> _activePowerups;

    bool _alive = true;

    int _lives = 3;

  public:
    Ship();
    Ship(int x, int y, ALLEGRO_BITMAP *_texture);
    ~Ship();

    bool move(int direction, int magnitude);
    bool moveTo(int x, int y);
    void setTexture(ALLEGRO_BITMAP *texture) { _texture = texture; }
    Rectangle getContainer() { return _container; }

    void addPowerup(int type, int duration);

    bool fire();

    void kill();
    void vivify() { _alive = true; }
    bool isAlive() { return _alive; }

    int lifeCount() { return _lives; }

    bool hitTest(std::list<Powerup> *powerups);
    bool hitTest(std::list<Bullet> *bullets);

    std::list<ActivePowerup> getActivePowerups();
    void clearActivePowerups() { _activePowerups.clear(); }

    void update(unsigned int ticks);

    void render();
    void render(int x, int y);
  };
#endif
