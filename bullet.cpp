#include "bullet.h"

Bullet::Bullet(int x, int y, ALLEGRO_BITMAP *texture, bool shipBullet, Rectangle bounds) {
  _container.setX(x);
  _container.setY(y);
  _container.setW(10);
  _container.setH(20);

  _alive = true;

  _texture = texture;

  _shipBullet = shipBullet;

  _bounds = bounds;
}

Bullet::~Bullet() {
}

int Bullet::move() {
  if (_shipBullet) {
    _container.setY(_container.getY() - _moveSpeed);
  } else {
    _container.setY(_container.getY() + _moveSpeed / 2);
  }

  return _container.getY();
}

void Bullet::update(unsigned int ticks) {
  move();

  if ((_container.getY() < 0 && _shipBullet) || (_container.getY() > _bounds.getY() + _bounds.getH())) {
    _alive = false;
  }
}

void Bullet::render() {
  if (_alive) {
    al_draw_bitmap(_texture, _container.getX(), _container.getY(), NULL);
  }
}
