#include "enemy.h"

Enemy::Enemy() {
  _container.setX(0);
  _container.setY(0);
  _container.setW(20);
  _container.setH(20);
  _alive = true;
}

Enemy::Enemy(int x, int y) {
  _container.setX(x);
  _container.setY(y);
  _container.setW(20);
  _container.setH(20);
  _alive = true;
}

Enemy::~Enemy() {
}

bool Enemy::hitTest(std::list<Bullet> *bullets) {
  int i;

  for (Bullet bullet : *bullets) {
    Rectangle bulletContainer = bullet.getContainer();
    if (_container.collidesWith(bulletContainer)) {
      bullet.kill();
      kill();
      return true;
    }
  }
  return false;
}

bool Enemy::update(unsigned int ticks) {
  return true;
}

void Enemy::render() {
  if (_alive) {
    al_draw_filled_rectangle(_container.getX(), _container.getY(),
      _container.getX() + _container.getW(), _container.getY() + _container.getH(),
      al_map_rgb(255, 0, 255));
  }
}

void Enemy::moveTo(int x, int y) {
  _container.setX(x);
  _container.setY(y);
}
