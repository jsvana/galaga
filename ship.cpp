#include "bullet.h"
#include "ship.h"

Ship::Ship() {
  _container.setX(0);
  _container.setY(0);
  _container.setW(32);
  _container.setH(32);

  _texture = al_load_bitmap("assets/images/galaga.png");
}

Ship::Ship(int x, int y) {
  _container.setX(x);
  _container.setY(y);
  _container.setW(32);
  _container.setH(32);

  _texture = al_load_bitmap("assets/images/galaga.png");
  if (!_texture) {
    std::cout << "nope" << std::endl;
  }
}

Ship::~Ship() {
  al_destroy_bitmap(_texture);
}

bool Ship::move(int direction, int magnitude) {
  switch (direction) {
    case GALAGA_LEFT:
      _container.setX(_container.getX() - magnitude);
      break;
    case GALAGA_RIGHT:
      _container.setX(_container.getX() + magnitude);
      break;
    case GALAGA_UP:
      _container.setY(_container.getY() - magnitude);
      break;
    case GALAGA_DOWN:
      _container.setY(_container.getY() + magnitude);
      break;
  }

  return true;
}
bool Ship::moveTo(int x, int y) {
  _container.setX(x);
  _container.setY(y);

  return true;
}

void Ship::update(unsigned int ticks) {
}

void Ship::render() {
  int x = _container.getX();
  int y = _container.getY();
  int w = _container.getW();
  int h = _container.getH();

  // al_draw_filled_triangle(x, y + h, x + w / 2, y, x + w, y + h, al_map_rgb(255, 0, 0));

  if (_texture) {
    al_draw_bitmap(_texture, x, y, NULL);
  }
}
