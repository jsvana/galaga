#include "enemy.h"

Enemy::Enemy() {
  _container.setX(0);
  _container.setY(0);
  _container.setW(30);
  _container.setH(24);
  _alive = true;

  _basePosition.setX(0);
  _basePosition.setY(0);

  _fireRate = rand() % 150;
}

Enemy::Enemy(int x, int y, Rectangle bounds, ALLEGRO_BITMAP *texture, int enemyType, std::string sample, int difficulty) {
  _container.setX(x);
  _container.setY(y);
  _container.setW(30);
  _container.setH(24);
  _alive = true;

  _bounds = bounds;

  _basePosition.setX(x);
  _basePosition.setY(y);

  _texture = texture;
  _sample = sample;
  _enemyType = enemyType;

  int scale = round((difficulty / 6.0) * (difficulty / 6.0) + 1.0);

  _xSpeed += scale;

  _fireRate = (rand() % 50) + 100;
}

Enemy::~Enemy() {

}

void Enemy::decideShot(Ship ship) {
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

    int enemyMiddle = _container.getX() + _container.getW() / 4;

    if (newShipX <= enemyMiddle
      && newShipX + ship.getContainer().getW() >= enemyMiddle && rand() % 10 < 2) {
      trigger();
    } else if (rand() % 10 < 2) {
      trigger();
    }
  }
}

bool Enemy::hitTest(std::list<Bullet> *bullets) {
  int i;

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

bool Enemy::update(unsigned int ticks) {
  _container.setX(_container.getX() + _xSpeed);
  _basePosition.setX(_container.getX());
  _container.setY(_basePosition.getY() + 10 * sin(ticks / 8));

  if ((_basePosition.getX() < _bounds.getX() + PADDING && _xSpeed < 0)
    || (_basePosition.getX() + _container.getW() > _bounds.getW() - PADDING
    && _xSpeed > 0)) {
    _xSpeed = -_xSpeed;
  }

  if (ticks % 30 == 0) {
    _frame = (_frame + 1) % 2;
  }

  ++_stateTicks;

  return true;
}

void Enemy::render() {
  if (_alive) {
    al_draw_bitmap_region(_texture, _container.getW() * _frame, _container.getH() * _enemyType,
      _container.getW(), _container.getH(),
      _container.getX(), _container.getY(), NULL);
  }
}

void Enemy::moveTo(int x, int y) {
  _container.setX(x);
  _container.setY(y);
}

int Enemy::getPointsWorth() {
  return 80;
}
