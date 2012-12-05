#include "bullet.h"

Bullet::Bullet(int x, int y) {
  _container.setX(x);
  _container.setY(y);
  _container.setW(10);
  _container.setH(10);

  _alive = true;
}

Bullet::~Bullet() {
}

int Bullet::move() {
  _container.setY(_container.getY() - _moveSpeed);

  return _container.getY();
}

void Bullet::update(unsigned int ticks) {
  move();

  if (_container.getY() < 0) {
    _alive = false;
  }
}

void Bullet::render() {
  if (_alive) {
    al_draw_filled_rectangle(_container.getX(), _container.getY(),
      _container.getX() + _container.getW(), _container.getY() +
      _container.getH(), al_map_rgb(255, 255, 0));
  }
}
