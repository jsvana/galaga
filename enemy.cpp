#include "enemy.h"

Enemy::Enemy() {
  _container.setX(0);
  _container.setY(0);
  _container.setW(30);
  _container.setH(24);
  _initialPosition.setX(0);
  _initialPosition.setY(0);
  _alive = true;

  _fireRate = rand() % 150;
}

Enemy::Enemy(int x, int y, ALLEGRO_BITMAP *texture, int enemyType, ALLEGRO_SAMPLE *sample) {
  _container.setX(x);
  _container.setY(y);
  _container.setW(30);
  _container.setH(24);
  _initialPosition.setX(x);
  _initialPosition.setY(y);
  _alive = true;

  _texture = texture;
  _sample = sample;
  _enemyType = enemyType;

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
      al_play_sample(_sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

      return true;
    }
  }
  return false;
}

bool Enemy::update(unsigned int ticks) {
  ++_stateTicks;

  switch (_currentState) {
    case GALAGA_ENEMY_STATE_MOVE:
      if (_stateTicks >= 200) {
        _previousState = _currentState;
        _currentState = GALAGA_ENEMY_STATE_GROW;

        _growRateX = (float)_container.getX() / 100.0;
        _growRateY = (float)_container.getY() / 117.0;

        if (_xSpeed < 0) {
          _xSpeed = -_xSpeed;
        }

        _stateTicks = 0;
      } else {
        _container.setX(_container.getX() + _xSpeed);

        if (_stateTicks % 100 == 0) {
          _xSpeed = -_xSpeed;
        }
      }
      break;
    case GALAGA_ENEMY_STATE_GROW:

      if (_stateTicks >= 100) {
        _previousState = _currentState;
        _currentState = GALAGA_ENEMY_STATE_MOVE;

        _container.setX(_initialPosition.getX());
        _container.setY(_initialPosition.getY());

        _stateTicks = 0;
      } else {
        _container.setX(_container.getX() + _growRateX);
        _container.setY(_container.getY() + _growRateY);

        if (_stateTicks % 50 == 0) {
          _growRateX = -_growRateX;
          _growRateY = -_growRateY;
        }
      }
      break;
  }

  if (ticks % 30 == 0) {
    _frame = (_frame + 1) % 2;
  }

  return true;
}

void Enemy::render() {
  if (_alive) {
    al_draw_bitmap_region(_texture, _container.getW() * _frame, _container.getH() * _enemyType,
      _container.getW(), _container.getH(),
      _container.getX() + (int)_growX, _container.getY() + (int)_growY, NULL);
  }
}

void Enemy::moveTo(int x, int y) {
  _container.setX(x);
  _container.setY(y);
}
