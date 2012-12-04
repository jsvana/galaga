#include "bullet.h"

Bullet::Bullet(int x, int y, int index) {
  _position.setX(x);
  _position.setY(y);
}

Bullet::~Bullet() {
}

int Bullet::move() {
  _position.setY(_position.getY() - 2);

  return _position.getY();
}

void Bullet::update(unsigned int ticks) {
  move();
}

void Bullet::render() {
  al_draw_filled_circle(_position.getX(), _position.getY(), 5, al_map_rgb(255, 255, 0));
}
