#include "background.h"

Background::Background(int screenWidth, int screenHeight, int ySpeed) {
  _screenWidth = screenWidth;
  _screenHeight = screenHeight;
  _ySpeed = ySpeed;

  Rectangle bounds(0, 0, _screenWidth, _screenHeight);

  for (int i = 0; i < 10; i++) {
    Particle particle(rand() % _screenWidth, rand() % _screenHeight, _ySpeed,
      bounds);
    _particles.push_back(particle);
  }
}

Background::~Background() {
  _particles.clear();
}

bool Background::update(unsigned int ticks) {
  for (Particle& particle : _particles) {
    particle.update(ticks);
  }

  return true;
}

void Background::render() {
  for (Particle particle : _particles) {
    particle.render();
  }
}
