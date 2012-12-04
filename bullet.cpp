#include "bullet.h"

Bullet::Bullet(int x, int y, int index) {
  _position.setX(x);
  _position.setY(y);

  _isDead = false;
}

Bullet::~Bullet() {
}

int Bullet::move() {
  _position.setY(_position.getY() - 4);

  return _position.getY();
}

void Bullet::update(unsigned int ticks) {
  move();

  if (_position.getY() < 0) {
    _isDead = true;
  }
}

void Bullet::render() {
  al_draw_filled_rectangle(_position.getX() - 5, _position.getY() - 5, _position.getX() + 5, _position.getY() + 5, al_map_rgb(255, 255, 0));
}
