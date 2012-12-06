#include "powerup.h"

Powerup::Powerup(int x, int y, ALLEGRO_BITMAP *texture, int type, Rectangle bounds) {
  _container.setX(x);
  _container.setY(y);
  _container.setW(32);
  _container.setH(32);

  _texture = texture;

  _type = type;

  _bounds = bounds;
}

bool Powerup::update(unsigned int ticks) {
  if (ticks % 30 == 0) {
    _frame = (_frame + 1) % 2;
  }

  _container.setY(_container.getY() + 3);

  if (_container.getY() >= _bounds.getY() + _bounds.getH()) {
    kill();
  }

  return true;
}

void Powerup::render() {
  if (_alive) {
    al_draw_bitmap_region(_texture, _container.getW() * _frame, _container.getH() * _type,
      _container.getW(), _container.getH(),
      _container.getX(), _container.getY(), NULL);
  }
}
