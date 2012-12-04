#include "bullet.h"
#include "ship.h"

Ship::Ship(int x, int y) {
  _position = new Point(x, y);
}

Ship::~Ship() {
  delete _position;

  _bullets.erase(_bullets.begin(), _bullets.end());
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

void Ship::update(unsigned int ticks) {
  int i;

  if (_bullets.size() > 0 && _bullets[0].getPosition().getY() < 0) {
    _bullets.erase(_bullets.begin());
  }

  for (i = 0; i < _bullets.size(); i++) {
    _bullets[i].update(ticks);
  }
}

void Ship::render() {
  int x = _position->getX();
  int y = _position->getY();
  int i;

  al_draw_filled_triangle(x - 10, y + 20, x, y, x + 10, y + 20, al_map_rgb(255, 0, 0));

  for (i = 0; i < _bullets.size(); i++) {
    _bullets[i].render();
  }
}

bool Ship::fire() {
  if (_bullets.size() == MAX_BULLETS) {
    return false;
  }

  Bullet newBullet(_position->getX(), _position->getY(), _bullets.size());

  _bullets.push_back(newBullet);

  return true;
}
