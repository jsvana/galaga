#include "boss.h"

Boss::Boss() {
  _container.setX(0);
  _container.setY(0);
  _container.setW(30);
  _container.setH(24);
  _alive = true;

  _fireRate = rand() % 150;

  recalculateSpeeds();
}

Boss::Boss(int x, int y, Rectangle bounds, ALLEGRO_BITMAP *texture, int difficulty) {
  _container.setX(x);
  _container.setY(y);
  _container.setW(30);
  _container.setH(24);
  _alive = true;

  _bounds = bounds;

  _texture = texture;

  _fireRate = (rand() % 50) + 100;

  recalculateSpeeds();
}

Boss::~Boss() {

}

void Boss::decideShot(Ship ship) {
  if (_stateTicks % _fireRate == 0) {
    int ticksToImpact = (ship.getContainer().getY() - _container.getY()) / 5;
    int shipDirection = ship.getDirection();
    int shipSpeed = ship.getMoveSpeed();
    int shipX = ship.getContainer().getX();
    int shipY = ship.getContainer().getY();
    int newShipX;

    if (shipDirection == GALAGA_LEFT) {
      newShipX = shipX - shipSpeed * ticksToImpact;
    } else if (shipDirection == GALAGA_RIGHT) {
      newShipX = shipX + shipSpeed * ticksToImpact;
    } else {
      newShipX = shipX;
    }

    int bossMiddle = _container.getX() + _container.getW() / 4;

    if (newShipX <= bossMiddle
      && newShipX + ship.getContainer().getW() >= bossMiddle && rand() % 10 < 2) {
      trigger();
    } else if (rand() % 10 < 2) {
      trigger();
    }
  }
}

bool Boss::hitTest(std::list<Bullet> *bullets) {
  for (Bullet& bullet : *bullets) {
    Rectangle bulletContainer = bullet.getContainer();

    if (bullet.isAlive() && _container.collidesWith(bulletContainer)) {
      bullet.kill();
      kill();

      return true;
    }
  }

  return false;
}

bool Boss::hitTest(Ship *ship) {
  if (_container.collidesWith(ship->getContainer())) {
    ship->kill();

    return true;
  }

  return false;
}

bool Boss::update(unsigned int ticks) {
  float remainingXDistance = _destination.getX() - _container.getX();
  float remainingYDistance = _destination.getY() - _container.getY();

  if (remainingXDistance < 5 && remainingXDistance > -5
    && remainingYDistance < 5 && remainingYDistance > -5) {
    recalculateSpeeds();
  }

  _container.setX(_container.getX() + _xSpeed);
  _container.setY(_container.getY() + _ySpeed);

  ++_stateTicks;

  return true;
}

void Boss::recalculateSpeeds() {
  _destination.setX((rand() % (int)_bounds.getW() - _bounds.getX()
    - _container.getW()) + _bounds.getX());
  _destination.setY((rand() % (int)_bounds.getH() - _bounds.getY()
    - _container.getH()) + _bounds.getY());

  float xDistance = sqrt((_destination.getX() - _container.getX())
    * (_destination.getX() - _container.getX()));
  float yDistance = sqrt((_destination.getY() - _container.getY())
    * (_destination.getY() - _container.getY()));

  float frames = sqrt(xDistance * xDistance + yDistance * yDistance) / 5;

  _xSpeed = xDistance / frames;
  _ySpeed = yDistance / frames;

  if (_destination.getX() < _container.getX()) {
    _xSpeed = -_xSpeed;
  }

  if (_destination.getY() < _container.getY()) {
    _ySpeed = -_ySpeed;
  }
}

void Boss::render() {
  if (_alive) {
    al_draw_bitmap_region(_texture, 0,
      0, _container.getW(), _container.getH(),
      _container.getX(), _container.getY(), NULL);
  }
}

int Boss::getPointsWorth() {
  return 200;
}
