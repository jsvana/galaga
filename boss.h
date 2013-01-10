#ifndef GALAGA_BOSS
	#define GALAGA_BOSS

	#include <cmath>
	#include <stdlib.h>

	#include "ship.h"
	#include "utilities.h"

	class Boss {
    Rectangle _container;
    Rectangle _bounds;

    Point _destination;

    ALLEGRO_BITMAP *_texture = NULL;

    std::string _sample;

    float _xSpeed;
    float _ySpeed;

    int _fireRate;

    bool _alive;
    int _frame = 0;

    bool _needsFire = false;

    unsigned int _stateTicks;

  public:
    Boss();
    Boss(int x, int y, Rectangle bounds, ALLEGRO_BITMAP *texture, int difficulty);
    ~Boss();

    bool update(unsigned int ticks);
    void render();

    Rectangle getContainer() { return _container; }

    void recalculateSpeeds();

    void decideShot(Ship ship);
    void trigger() { _needsFire = true; }
    bool needsFire() { return _needsFire; }
    void fire() { _needsFire = false; }

    bool isAlive() { return _alive; }
    std::string getSample() { return _sample; }

    int getPointsWorth();

    void kill() { _alive = false; }
    void vivify() { _alive = true; }

    bool hitTest(std::list<Bullet> *bullets);

    void moveTo(int x, int y);
  };
#endif
