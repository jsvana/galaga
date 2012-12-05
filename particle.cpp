#include "particle.h"

Particle::Particle(int x, int y) {
  _container.setX(x);
  _container.setY(y);

  _container.setW(rand() % 6 + 5);
  _container.setH(rand() % 6 + 5);

  _xSpeed = rand() % 11 - 5;
  _ySpeed = rand() % 11 - 5;

  _r = rand() % 156 + 100;
  _g = rand() % 156 + 100;
  _b = rand() % 156 + 100;
}

Particle::Particle(int x, int y, int ySpeed, Rectangle bounds) {
  _container.setX(x);
  _container.setY(y);

  _container.setW(rand() % 3 + 3);
  _container.setH(rand() % 3 + 3);

  _xSpeed = 0;
  _ySpeed = ySpeed;

  _r = rand() % 156 + 100;
  _g = rand() % 156 + 100;
  _b = rand() % 156 + 100;

  _decay = false;

  _bounds = bounds;
}

Particle::~Particle() {
}

bool Particle::update(unsigned int ticks) {
  _container.setX(_container.getX() + _xSpeed);
  _container.setY(_container.getY() + _ySpeed);
  if (_decay) {
    _container.setW(_container.getW() - 1);
    _container.setH(_container.getH() - 1);

    if (_container.getW() <= 0 || _container.getH() <= 0) {
      kill();
    }
  } else {
    _r = (_r + 1) % 255;
    _g = (_g + 1) % 255;
    _b = (_b + 1) % 255;

    if (_container.getY() >= _bounds.getY() + _bounds.getH()) {
      _container.setY(0);
    }
  }

  return true;
}

void Particle::render() {
  al_draw_filled_rectangle(_container.getX(), _container.getY(),
    _container.getX() + _container.getW(), _container.getY() + _container.getH(),
    al_map_rgb(_r, _g, _b));
}
