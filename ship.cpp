#include "bullet.h"
#include "ship.h"

Ship::Ship() {
  _container.setX(0);
  _container.setY(0);
  _container.setW(32);
  _container.setH(32);
}

Ship::Ship(int x, int y, ALLEGRO_BITMAP *texture, ALLEGRO_BITMAP *explodingTexture) {
  _container.setX(x);
  _container.setY(y);
  _container.setW(32);
  _container.setH(32);

  _texture = texture;
  _explodingTexture = explodingTexture;
}

Ship::~Ship() {
  _activePowerups.clear();
}

void Ship::reset() {
  _previousState = NULL;
  _currentState = GALAGA_SHIP_STATE_MAIN;
  _stateTicks = 0;
  _frame = 0;

  _lives = 3;

  clearActivePowerups();
}

bool Ship::move(int direction) {
  if (direction == GALAGA_LEFT) {
    _container.setX(_container.getX() - MOVE_SPEED);
    _direction = GALAGA_LEFT;
  } else if (direction == GALAGA_RIGHT) {
    _container.setX(_container.getX() + MOVE_SPEED);
    _direction = GALAGA_RIGHT;
  }

  return true;
}

bool Ship::moveTo(int x, int y) {
  _container.setX(x);
  _container.setY(y);

  return true;
}

bool Ship::hitTest(std::list<Powerup> *powerups) {
  if (_currentState == GALAGA_SHIP_STATE_MAIN) {
    for (Powerup& powerup : *powerups) {
      Rectangle powerupContainer = powerup.getContainer();
      if (powerup.isAlive() && _container.collidesWith(powerupContainer)) {
        powerup.kill();
        powerup.hit();
        return true;
      }
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

  _previousState = _currentState;
  _currentState = GALAGA_SHIP_STATE_EXPLODING;
  _stateTicks = 0;

  _activePowerups.clear();

  if (_lives < 0) {
    _alive = false;
  }
}

void Ship::update(unsigned int ticks) {
  std::list<ActivePowerup>::iterator powerupIter = _activePowerups.begin();

  switch (_currentState) {
    case GALAGA_SHIP_STATE_MAIN:
      for (ActivePowerup& powerup : _activePowerups) {
        ++powerup.lifetime;

        if (powerup.lifetime >= powerup.duration) {
          powerup.complete = true;
        }
      }

      break;
    case GALAGA_SHIP_STATE_EXPLODING:
      if (_stateTicks != 0 && _stateTicks % 15 == 0) {
        ++_frame;
      }

      if (_frame == 4) {
        _frame = 0;

        _previousState = _currentState;
        _currentState = GALAGA_SHIP_STATE_MAIN;
        _stateTicks = 0;
      }

      break;
  }

  ++_stateTicks;
}

void Ship::render() {
  int x = _container.getX();
  int y = _container.getY();

  if (_currentState == GALAGA_SHIP_STATE_MAIN) {
    al_draw_bitmap(_texture, x, y, NULL);
  } else {
    al_draw_bitmap_region(_explodingTexture, 32 * _frame, 0, 32, 32, x, y, NULL);
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
