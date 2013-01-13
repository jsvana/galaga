#ifndef GALAGA_ENEMY
  #define GALAGA_ENEMY

  #include <allegro5/allegro.h>
  #include <allegro5/allegro_audio.h>
  #include <allegro5/allegro_image.h>
  #include <allegro5/allegro_primitives.h>
  #include <cmath>
  #include <iostream>
  #include <list>

  #include "bullet.h"
  #include "ship.h"
  #include "utilities.h"

  #define GALAGA_ENEMY_STATE_MOVE 0
  #define GALAGA_ENEMY_STATE_GROW 1
  #define GALAGA_ENEMY_STATE_STRIKE 2

  class Enemy {
  protected:
    Rectangle _container;
    Rectangle _bounds;
    Point _basePosition;

    ALLEGRO_BITMAP *_texture = NULL;

    std::string _sample;

    int _xSpeed = 2;

    int _fireRate;

    bool _alive;
    int _enemyType;
    int _frame = 0;

    int _previousState = NULL;
    int _currentState = GALAGA_ENEMY_STATE_MOVE;

    bool _needsFire = false;

    unsigned int _stateTicks;

    const int PADDING = 10;

  public:
    Enemy();
    Enemy(int x, int y, Rectangle bounds, ALLEGRO_BITMAP *texture, int enemyType, std::string sample, int difficulty);
    ~Enemy();

    virtual bool update(unsigned int ticks);
    virtual void render();

    Rectangle getContainer() { return _container; }

    virtual void decideShot(Ship ship);
    void trigger() { _needsFire = true; }
    bool needsFire() { return _needsFire; }
    void fire() { _needsFire = false; }

    bool isAlive() { return _alive; }
    std::string getSample() { return _sample; }

    virtual int getPointsWorth();

    void kill() { _alive = false; }
    void vivify() { _alive = true; }

    virtual bool hitTest(std::list<Bullet> *bullets);
    virtual bool hitTest(Ship *ship) { return false; }

    void moveTo(int x, int y);
  };
#endif
