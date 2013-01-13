#ifndef GALAGA_BOSS
	#define GALAGA_BOSS

	#include <cmath>
	#include <stdlib.h>

	#include "enemy.h"
	#include "ship.h"
	#include "utilities.h"

  class Boss : public Enemy {
    Point _destination;

    float _xSpeed;
    float _ySpeed;

  public:
    Boss();
    Boss(int x, int y, Rectangle bounds, ALLEGRO_BITMAP *texture, int difficulty);
    ~Boss();

    bool update(unsigned int ticks);
    void render();

    Rectangle getContainer() { return _container; }

    void recalculateSpeeds();

    void decideShot(Ship ship);
    std::string getSample() { return _sample; }

    int getPointsWorth();

    bool hitTest(std::list<Bullet> *bullets);
    bool hitTest(Ship *ship);
  };
#endif
