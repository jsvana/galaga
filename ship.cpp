#include "bullet.h"
#include "ship.h"

Ship::Ship() {
  _container.setX(0);
  _container.setY(0);
  _container.setW(32);
  _container.setH(32);
}

Ship::Ship(int x, int y, ALLEGRO_BITMAP *texture) {
  _container.setX(x);
  _container.setY(y);
  _container.setW(32);
  _container.setH(32);

  _texture = texture;
}

Ship::~Ship() {
  _activePowerups.clear();
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

bool Ship::hitTest(std::list<Powerup> *powerups) {
  for (Powerup& powerup : *powerups) {
    Rectangle powerupContainer = powerup.getContainer();
    if (powerup.isAlive() && _container.collidesWith(powerupContainer)) {
      powerup.kill();
      powerup.hit();
      return true;
    }
  }

  return false;
}

bool Ship::hitTest(std::list<Bullet> *bullets) {
  for (Bullet& bullet : *bullets) {
    Rectangle bulletContainer = bullet.getContainer();
    if (bullet.isAlive() && _container.collidesWith(bulletContainer)) {
      bullet.kill();
      kill();
      return true;
    }
  }

  return false;
}

void Ship::kill() {
  --_lives;

  if (_lives < 0) {
    _alive = false;
  }
}

void Ship::update(unsigned int ticks) {
  std::list<ActivePowerup>::iterator powerupIter = _activePowerups.begin();

  for (ActivePowerup& powerup : _activePowerups) {
    ++powerup.lifetime;

    if (powerup.lifetime >= powerup.duration) {
      powerup.complete = true;
    }
  }
}

void Ship::render() {
  int x = _container.getX();
  int y = _container.getY();

  if (_texture) {
    al_draw_bitmap(_texture, x, y, NULL);
  }
}

std::list<ActivePowerup> Ship::getActivePowerups() {
  std::list<ActivePowerup> oldPowerups(_activePowerups);

  std::list<ActivePowerup>::iterator powerupIter = _activePowerups.begin();

  while (powerupIter != _activePowerups.end()) {
    ++(*powerupIter).lifetime;

    if ((*powerupIter).complete) {
      _activePowerups.erase(powerupIter++);
    } else {
      ++powerupIter;
    }
  }

  return oldPowerups;
}

void Ship::addPowerup(int type, int duration) {
  ActivePowerup newPowerup;
  newPowerup.type = type;
  newPowerup.lifetime = 0;
  newPowerup.duration = duration;
  newPowerup.complete = false;

  _activePowerups.push_back(newPowerup);
}
