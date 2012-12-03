#include "bullet.h"

Bullet::Bullet(int x, int y) {
  position = new Point(x, y);
}

Bullet::~Bullet() {
  delete position;
}

int Bullet::move() {
  position->setY(position->getY() - 4);

  return position->getY();
}
