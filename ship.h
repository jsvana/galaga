#ifndef GALAGA_SHIP
  #define GALAGA_SHIP

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>
  #include <allegro5/allegro_image.h>
  #include <list>

  #include "bullet.h"
  #include "powerup.h"
  #include "utilities.h"

  #define GALAGA_SHIP_STATE_MAIN 0
  #define GALAGA_SHIP_STATE_EXPLODING 1

  class Ship {
    ALLEGRO_BITMAP *_texture;
    ALLEGRO_BITMAP *_explodingTexture;

    Rectangle _container;

    std::list<ActivePowerup> _activePowerups;

    bool _alive = true;

    int _lives = 3;

    int _previousState = NULL;
    int _currentState = GALAGA_SHIP_STATE_MAIN;

    int _stateTicks = 0;

    int _frame = 0;

  public:
    Ship();
    Ship(int x, int y, ALLEGRO_BITMAP *_texture, ALLEGRO_BITMAP *_explodingTexture);
    ~Ship();

    void reset();

    bool move(int direction, int magnitude);
    bool moveTo(int x, int y);
    void setTexture(ALLEGRO_BITMAP *texture) { _texture = texture; }
    void setExplodingTexture(ALLEGRO_BITMAP *explodingTexture) { _explodingTexture = explodingTexture; }
    Rectangle getContainer() { return _container; }

    void addPowerup(int type, int duration);

    bool fire();

    void kill();
    void vivify() { _alive = true; }
    bool isAlive() { return _alive; }
    bool isExploding() { return _currentState == GALAGA_SHIP_STATE_EXPLODING; }

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
