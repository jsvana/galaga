#include "enemy.h"

Enemy::Enemy() {
  _container.setX(0);
  _container.setY(0);
  _container.setW(30);
  _container.setH(24);
  _initialPosition.setX(0);
  _initialPosition.setY(0);
  _alive = true;
}

Enemy::Enemy(int x, int y, Rectangle bounds, ALLEGRO_BITMAP *texture, int enemyType, ALLEGRO_SAMPLE *sample) {
  _container.setX(x);
  _container.setY(y);
  _container.setW(30);
  _container.setH(24);
  _initialPosition.setX(x);
  _initialPosition.setY(y);
  _alive = true;

  _bounds = bounds;

  _texture = texture;
  _sample = sample;
  _enemyType = enemyType;
}

Enemy::~Enemy() {
}

bool Enemy::hitTest(std::list<Bullet> *bullets) {
  int i;

  for (Bullet& bullet : *bullets) {
    Rectangle bulletContainer = bullet.getContainer();
    if (bullet.isAlive() && _container.collidesWith(bulletContainer)) {
      bullet.kill();
      kill();
      al_play_sample(_sample, .8, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
      return true;
    }
  }
  return false;
}

bool Enemy::update(unsigned int ticks) {
  switch (_currentState) {
    case GALAGA_ENEMY_STATE_IDLE:
      _rotation += _rotationSpeed;

      if (_rotation >= 2 * GALAGA_PI || _rotation <= GALAGA_PI) {
        _rotationSpeed = -_rotationSpeed;
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
    if (_texture) {
      int xRotation = (float)_idleRadius * cos(_rotation);
      int yRotation = (float)_idleRadius * sin(_rotation);

      al_draw_bitmap_region(_texture, _container.getW() * _frame, _container.getH() * _enemyType,
        _container.getW(), _container.getH(),
        _container.getX() + xRotation, _container.getY() + yRotation, NULL);
    } else {
      al_draw_filled_rectangle(_container.getX(), _container.getY(),
        _container.getX() + _container.getW(), _container.getY() + _container.getH(),
        al_map_rgb(255, 0, 255));
    }
  }
}

void Enemy::moveTo(int x, int y) {
  _container.setX(x);
  _container.setY(y);
}
