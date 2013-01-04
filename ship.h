#ifndef GALAGA_SHIP
  #define GALAGA_SHIP

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_primitives.h>
  #include <allegro5/allegro_image.h>
  #include <list>

  #include "asset_manager.h"
  #include "bullet.h"
  #include "powerup.h"
  #include "utilities.h"

  #define GALAGA_SHIP_STATE_MAIN 0
  #define GALAGA_SHIP_STATE_EXPLODING 1

  class Ship {
    ALLEGRO_BITMAP *_texture;
    ALLEGRO_BITMAP *_explodingTexture;

    Rectangle _container;

    int _screenWidth;
    int _screenHeight;

    std::list<Bullet> _bullets;
    std::list<ActivePowerup> _activePowerups;

    bool _alive = true;

    int _lives = 3;

    int _previousState = NULL;
    int _currentState = GALAGA_SHIP_STATE_MAIN;

    int _stateTicks = 0;

    int _frame = 0;

    int _direction = GALAGA_NONE;

    int _maxBullets = 2;
    int _bulletCount = 1;

    int _shotsFired = 0;

    const int MOVE_SPEED = 5;

  public:
    Ship();
    Ship(int x, int y, ALLEGRO_BITMAP *_texture, ALLEGRO_BITMAP *_explodingTexture);
    ~Ship();

    void reset();

    void setScreenBounds(int screenWidth, int screenHeight);

    bool move(int direction);
    void stopMovement() { _direction = GALAGA_NONE; }
    bool moveTo(int x, int y);
    void setTexture(ALLEGRO_BITMAP *texture) { _texture = texture; }
    void setExplodingTexture(ALLEGRO_BITMAP *explodingTexture) { _explodingTexture = explodingTexture; }
    Rectangle getContainer() { return _container; }

    int getDirection() { return _direction; }
    int getMoveSpeed() { return MOVE_SPEED; }

    void addPowerup(int type);

    bool fire();

    void kill();
    void vivify() { _alive = true; }
    bool isAlive() { return _alive; }
    bool isExploding() { return _currentState == GALAGA_SHIP_STATE_EXPLODING; }
    bool isDead() { return _lives == 0 && !isExploding(); }

    int lifeCount() { return _lives; }
    int getShotsFired() { return _shotsFired; }

    bool hitTest(std::list<Powerup> *powerups);
    bool hitTest(std::list<Bullet> *bullets);

    std::list<ActivePowerup> getActivePowerups();
    void clearActivePowerups();

    void update(unsigned int ticks);

    void render();
    void render(int x, int y);

    std::list<Bullet> *getBullets() { return &_bullets; }
    int getBulletCount() { return _bullets.size(); }
    void addBullet(Bullet bullet);

    int getShotCount() { return _shotsFired; }
  };
#endif
