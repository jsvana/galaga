#include "ship.h"

Ship::Ship(int x, int y) {
  _position = new Point(x, y);
}

Ship::~Ship() {
  delete _position;
}

bool Ship::move(int direction, int magnitude) {
  switch (direction) {
    case GALAGA_LEFT:
      _position->setX(_position->getX() - magnitude);
      break;
    case GALAGA_RIGHT:
      _position->setX(_position->getX() + magnitude);
      break;
    case GALAGA_UP:
      _position->setY(_position->getY() - magnitude);
      break;
    case GALAGA_DOWN:
      _position->setY(_position->getY() + magnitude);
      break;
  }

  return true;
}
bool Ship::moveTo(int x, int y) {
  _position->setX(x);
  _position->setY(y);

  return true;
}

Point Ship::getPosition() {
  return *_position;
}

void Ship::render() {
  int x = _position->getX();
  int y = _position->getY();

  al_draw_filled_triangle(x - 10, y + 20, x, y, x + 10, y + 20, al_map_rgb(255, 0, 0));
}
