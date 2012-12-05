#include "bullet.h"

Bullet::Bullet(int x, int y, ALLEGRO_BITMAP *texture) {
  _container.setX(x);
  _container.setY(y);
  _container.setW(10);
  _container.setH(20);

  _alive = true;

  _texture = texture;
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
    // al_draw_filled_rectangle(_container.getX(), _container.getY(),
    //   _container.getX() + _container.getW(), _container.getY() +
    //   _container.getH(), al_map_rgb(255, 255, 0));
    al_draw_bitmap(_texture, _container.getX(), _container.getY(), NULL);
  }
}
